#include "cairo_runner.h"
#include "collectc/cc_common.h"
#include "collectc/cc_hashtable.h"
#include "memory.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"
#include <assert.h>
#include <collectc/cc_array.h>
#include <stdio.h>
#include <stdlib.h>

cairo_runner runner_new(struct program program) {
	virtual_machine vm = vm_new();
	CC_Array *relocated_memory;
	cc_array_new(&relocated_memory);
	CC_Array *relocation_table;
	cc_array_new(&relocation_table);
	cairo_runner runner = {program, vm, relocated_memory, relocation_table, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	return runner;
}

void runner_free(cairo_runner *runner) {
	memory_free(&runner->vm.memory);
	program_free(&runner->program);
	cc_array_remove_all_free(runner->relocated_memory);
	cc_array_destroy(runner->relocated_memory);
	cc_array_remove_all_free(runner->relocation_table);
	cc_array_destroy(runner->relocation_table);
}

// Creates program, execution and builtin segments.
void runner_initialize_segments(cairo_runner *runner) {
	// Program Segment
	runner->program_base = memory_add_segment(&runner->vm.memory);
	// Execution Segment
	runner->execution_base = memory_add_segment(&runner->vm.memory);
	// Initialize builtin segments
}

// Initializes the program segment & initial pc
// Warning: Can fail if used outside of runner_initialize
void runner_initialize_state(cairo_runner *runner, unsigned int entrypoint, CC_Array *stack) {
	runner->initial_pc = runner->program_base;
	runner->initial_pc.offset += entrypoint;
	// We ignore the error cases when loading data as these cases are unreachable when called after
	// runner_initialize_segments We can safely ignore them as long as any public function that calls this function
	// calls runner_initialize_segments before
	memory_load_data(&runner->vm.memory, runner->program_base, runner->program.data);
	memory_load_data(&runner->vm.memory, runner->execution_base, stack);
	// Mark data segment as accessed
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from a given pc offset
// (entrypoint)
// Warning: Can fail if runner_initialize_segments is not called before
relocatable runner_initialize_function_entrypoint(cairo_runner *runner, unsigned int entrypoint, CC_Array *stack,
                                                  relocatable return_fp) {
	relocatable end = memory_add_segment(&runner->vm.memory);
	maybe_relocatable return_fp_mr = {.is_felt = false, .value = {.relocatable = return_fp}};
	maybe_relocatable end_mr = {.is_felt = false, .value = {.relocatable = end}};
	cc_array_add(stack, &return_fp_mr);
	cc_array_add(stack, &end_mr);
	runner->initial_fp.segment_index = runner->execution_base.segment_index;
	runner->initial_fp.offset = cc_array_size(stack);
	runner_initialize_state(runner, entrypoint, stack);
	runner->final_pc = end;
	return end;
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from the main entrypoint
// Warning: Can fail if runner_initialize_segments is not called before
relocatable runner_initialize_main_entrypoint(cairo_runner *runner) {
	CC_Array *stack;
	cc_array_new(&stack);
	// Handle builtin initial stack
	// Handle proof-mode specific behaviour
	relocatable return_fp = memory_add_segment(&runner->vm.memory);
	relocatable end = runner_initialize_function_entrypoint(runner, runner->program.main, stack, return_fp);
	cc_array_destroy(stack);
	return end;
}

// Initializes the vm's run_context, adds builtin validation rules & validates memory
void runner_initialize_vm(cairo_runner *runner) {
	runner->vm.run_context.ap = runner->initial_fp;
	runner->vm.run_context.fp = runner->initial_fp;
	runner->vm.run_context.pc = runner->initial_pc;
	// Add validation rules
	// Apply validation rules to memory
}

void runner_get_segment_sizes(cairo_runner *runner, CC_Array *segment_sizes) {
	int num_segments = runner->vm.memory.num_segments;
	unsigned int *zero;
	for (int i = 0; i < num_segments; i++) {
		zero = malloc(sizeof(unsigned int));
		*zero = 0;
		cc_array_add(segment_sizes, zero);
	}

	unsigned int *new_size;
	CC_HashTableIter memory_iter;
	cc_hashtable_iter_init(&memory_iter, runner->vm.memory.data);
	TableEntry *entry;
	while (cc_hashtable_iter_next(&memory_iter, &entry) != CC_ITER_END) {
		relocatable *ptr = entry->key;
		unsigned int *segment_size;
		assert(cc_array_get_at(segment_sizes, ptr->segment_index, (void **)&segment_size) == CC_OK);

		if (*segment_size < ptr->offset + 1) {
			new_size = malloc(sizeof(unsigned int));
			*new_size = ptr->offset + 1;
			cc_array_replace_at(segment_sizes, new_size, ptr->segment_index, NULL);
		}
	}
}

// Fills relocation table with the first relocated address of each memory segment
// Warning: Can fail if runner_initialize_main_entrypoint is not called before
void runner_fill_relocation_table(cairo_runner *runner, CC_Array *segment_sizes) {
	int *first_addr = malloc(sizeof(int));
	*first_addr = 1;
	cc_array_add(runner->relocation_table, first_addr);
	int *last_segment_size_added;
	assert(cc_array_get_at(runner->relocation_table, 0, (void **)&last_segment_size_added) == CC_OK);

	int relocation_table_size = runner->vm.memory.num_segments + 1;
	for (int i = 1; i < relocation_table_size; i++) {
		int segment_idx = i - 1;
		int *segment_size;
		assert(cc_array_get_at(segment_sizes, segment_idx, (void **)&segment_size) == CC_OK);
		int *address = malloc(sizeof(int));
		*address = *last_segment_size_added + *segment_size;
		cc_array_add(runner->relocation_table, address);
		assert(cc_array_get_at(runner->relocation_table, i, (void **)&last_segment_size_added) == CC_OK);
	}
}

relocatable runner_initialize(cairo_runner *runner) {
	// runner_initialize_builtins
	runner_initialize_segments(runner);
	relocatable end = runner_initialize_main_entrypoint(runner);
	runner_initialize_vm(runner);
	return end;
}

int runner_get_relocated_address(cairo_runner *runner, relocatable *relocatable) {
	int *address;
	assert(cc_array_get_at(runner->relocation_table, relocatable->segment_index, (void **)&address) == CC_OK);
	return *address + relocatable->offset;
}

void runner_get_relocated_value(cairo_runner *runner, maybe_relocatable *value, felt_t out) {
	if (value->is_felt) {
		zero(out);
		add(out, value->value.felt, out);
	} else {
		int out_number = runner_get_relocated_address(runner, &value->value.relocatable);
		from(out, out_number);
	}
}

void runner_relocate_memory(cairo_runner *runner) {
	assert(cc_array_size(runner->relocated_memory) == 0);
	relocated_memory_cell *first_cell = malloc(sizeof(relocated_memory_cell));
	first_cell->is_some = false;
	first_cell->memory_value.none = 0;
	cc_array_add(runner->relocated_memory, first_cell);

	CC_Array *segment_sizes;
	cc_array_new(&segment_sizes);
	runner_get_segment_sizes(runner, segment_sizes);
	runner_fill_relocation_table(runner, segment_sizes);

	int num_segment_sizes = cc_array_size(segment_sizes);

	for (int i = 0; i < num_segment_sizes; i++) {
		int *segment_size;
		assert(cc_array_get_at(segment_sizes, i, (void **)&segment_size) == CC_OK);

		for (int j = 0; j < *segment_size; j++) {
			relocatable ptr = {i, j};
			maybe_relocatable *cell;
			if (cc_hashtable_get(runner->vm.memory.data, &ptr, (void **)&cell) == CC_OK) {
				int relocated_addr = runner_get_relocated_address(runner, &ptr);
				relocated_memory_cell *relocated_value = malloc(sizeof(relocated_memory_cell));
				relocated_value->is_some = true;
				runner_get_relocated_value(runner, cell, relocated_value->memory_value.value);
				cc_array_add_at(runner->relocated_memory, relocated_value, relocated_addr);
			} else {
				relocated_memory_cell *none = malloc(sizeof(relocated_memory_cell));
				none->is_some = false;
				none->memory_value.none = 0;
				cc_array_add(runner->relocated_memory, none);
			}
		}
	}
}
