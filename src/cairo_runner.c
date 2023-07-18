#include "cairo_runner.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"

cairo_runner runner_new(struct program program) {
	virtual_machine vm = vm_new();
	cairo_runner runner = {program, vm, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
	return runner;
}

void runner_initialize_segments(cairo_runner *runner) {
	// Program Segment
	runner->program_base = memory_add_segment(&runner->vm.memory);
	// Execution Segment
	runner->execution_base = memory_add_segment(&runner->vm.memory);
	// Initialize builtin segments
}

void runner_initialize_state(cairo_runner *runner, unsigned int entrypoint) {
	runner->initial_pc = runner->program_base;
	runner->initial_pc.offset += entrypoint;
	memory_load_data(&runner->vm.memory, runner->program_base, runner->program.data);
	// Mark data segment as accessed
}

relocatable runner_initialize_function_entrypoint(cairo_runner *runner, unsigned int entrypoint, CList *stack,
                                                  relocatable return_fp) {
	relocatable end = memory_add_segment(&runner->vm.memory);
	stack->add(stack, &return_fp);
	stack->add(stack, &end);
	runner->initial_fp.segment_index = runner->execution_base.segment_index;
	runner->initial_fp.offset = stack->count(stack);
	runner_initialize_state(runner, entrypoint);
	runner->final_pc = end;
	return end;
}

relocatable runner_initialize_main_entrypoint(cairo_runner *runner) {
	CList *stack = CList_init(sizeof(union maybe_relocatable));
	// Handle builtin initial stack
	// Handle proof-mode specific behaviour
	relocatable return_fp = memory_add_segment(&runner->vm.memory);
	return runner_initialize_function_entrypoint(runner, runner->program.main, stack, return_fp);
}

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
