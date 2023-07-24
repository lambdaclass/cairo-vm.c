#include "cairo_runner.h"
#include <collectc/cc_array.h>
#include "program.h"
#include "relocatable.h"
#include "vm.h"

cairo_runner runner_new(struct program program) {
	virtual_machine vm = vm_new();
	cairo_runner runner = {program, vm, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	return runner;
}

void runner_free(cairo_runner *runner) {
	memory_free(&runner->vm.memory);
	program_free(&runner->program);
}

// Creates program, execution and builtin segments
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
	cc_array_remove_all_free(stack);
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

relocatable runner_initialize(cairo_runner *runner) {
	// runner_initialize_builtins
	runner_initialize_segments(runner);
	relocatable end = runner_initialize_main_entrypoint(runner);
	runner_initialize_vm(runner);
	return end;
}
