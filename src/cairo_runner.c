#include "cairo_runner.h"
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

// Initializes the program segment & initial pc
void runner_initialize_state(cairo_runner *runner, unsigned int entrypoint) {
	runner->initial_pc = runner->program_base;
	runner->initial_pc.offset += entrypoint;
	memory_load_data(&runner->vm.memory, runner->program_base, runner->program.data);
	// Mark data segment as accessed
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from a given pc offset
// (entrypoint)
relocatable runner_initialize_function_entrypoint(cairo_runner *runner, unsigned int entrypoint, CList *stack,
                                                  relocatable return_fp) {
	relocatable end = memory_add_segment(&runner->vm.memory);
	maybe_relocatable return_fp_mr = {.is_felt = false, .value = {.relocatable = return_fp}};
	maybe_relocatable end_mr = {.is_felt = false, .value = {.relocatable = end}};
	stack->add(stack, &return_fp_mr);
	stack->add(stack, &end_mr);
	runner->initial_fp.segment_index = runner->execution_base.segment_index;
	runner->initial_fp.offset = stack->count(stack);
	runner_initialize_state(runner, entrypoint);
	runner->final_pc = end;
	return end;
}

// Initializes memory, initial register values & returns the end pointer (final pc) to run from the main entrypoint
relocatable runner_initialize_main_entrypoint(cairo_runner *runner) {
	CList *stack = CList_init(sizeof(maybe_relocatable));
	// Handle builtin initial stack
	// Handle proof-mode specific behaviour
	relocatable return_fp = memory_add_segment(&runner->vm.memory);
	return runner_initialize_function_entrypoint(runner, runner->program.main, stack, return_fp);
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
relocatable runner_initialize(cairo_runner *runner) {
	// runner_initialize_builtins
	runner_initialize_segments(runner);
	relocatable end = runner_initialize_main_entrypoint(runner);
	runner_initialize_vm(runner);
	return end;
}
