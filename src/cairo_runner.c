#include "cairo_runner.h"
#include "memory.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"

// Creates a new, empty cairo_runner
cairo_runner runner_new(struct program program) {
	virtual_machine vm = vm_new();
	cairo_runner runner = {program, vm, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	return runner;
}

// Frees resources used by the cairo_runner struct
void runner_free(cairo_runner *runner) { memory_free(&runner->vm.memory); }

// Creates program, execution and builtin segments
void runner_initialize_segments(cairo_runner *runner) {
	// Program Segment
	runner->program_base = memory_add_segment(&runner->vm.memory);
	// Execution Segment
	runner->execution_base = memory_add_segment(&runner->vm.memory);
	// Initialize builtin segments
}

// Initializes the program segment & initial pc, returns none if ok or memory_error if failure
ResultRunner runner_initialize_state(cairo_runner *runner, unsigned int entrypoint, CList *stack) {
	runner->initial_pc = runner->program_base;
	runner->initial_pc.offset += entrypoint;
	ResultMemory load_program_result =
	    memory_load_data(&runner->vm.memory, runner->program_base, runner->program.data);
	if (load_program_result.is_error) {
		ResultRunner error = {.is_error = true, .value = {.memory_error = load_program_result.value.error}};
		return error;
	}
	ResultMemory load_execution_result = memory_load_data(&runner->vm.memory, runner->execution_base, stack);
	if (load_execution_result.is_error) {
		ResultRunner error = {.is_error = true, .value = {.memory_error = load_execution_result.value.error}};
		return error;
	}
	// Mark data segment as accessed
	ResultRunner ok = {.is_error = false, .value = {.none = 0}};
	return ok;
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from a given pc offset
// (entrypoint)
// Returns ptr if ok, or memory_error if failure
ResultRunner runner_initialize_function_entrypoint(cairo_runner *runner, unsigned int entrypoint, CList *stack,
                                                   relocatable return_fp) {
	relocatable end = memory_add_segment(&runner->vm.memory);
	maybe_relocatable return_fp_mr = {.is_felt = false, .value = {.relocatable = return_fp}};
	maybe_relocatable end_mr = {.is_felt = false, .value = {.relocatable = end}};
	stack->add(stack, &return_fp_mr);
	stack->add(stack, &end_mr);
	runner->initial_fp.segment_index = runner->execution_base.segment_index;
	runner->initial_fp.offset = stack->count(stack);
	runner->final_pc = end;
	ResultRunner result = runner_initialize_state(runner, entrypoint, stack);
	if (result.is_error) {
		return result;
	} else {
		ResultRunner ok = {.is_error = false, .value = {.ptr = end}};
		return ok;
	}
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from the main entrypoint
// Returns ptr if ok, or memory_error if failure
ResultRunner runner_initialize_main_entrypoint(cairo_runner *runner) {
	CList *stack = CList_init(sizeof(maybe_relocatable));
	// Handle builtin initial stack
	// Handle proof-mode specific behaviour
	relocatable return_fp = memory_add_segment(&runner->vm.memory);
	ResultRunner end = runner_initialize_function_entrypoint(runner, runner->program.main, stack, return_fp);
	stack->free(stack);
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

// Performs the intialization step, leaving the runner ready to run a loaded cairo program from a main entrypoint
// Returns ptr if ok, or memory_error if failure
ResultRunner runner_initialize(cairo_runner *runner) {
	// runner_initialize_builtins
	runner_initialize_segments(runner);
	ResultRunner end = runner_initialize_main_entrypoint(runner);
	runner_initialize_vm(runner);
	return end;
}
