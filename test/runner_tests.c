#include "runner_tests.h"
#include "clist.h"
#include "memory.h"
#include "program.h"
#include "run_context.h"
#include "utils.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>

void initialize_runner_no_builtins_no_proof_mode_non_empty_program(void) {
	struct CList *program_data = CList_init(sizeof(maybe_relocatable));
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	program_data->add(program_data, &elem);
	struct program program = {0, program_data};
	cairo_runner runner = runner_new(program);
	runner_initialize(&runner);
	// Check runner data
	assert(runner.program_base.segment_index == 0 && runner.program_base.offset == 0);
	assert(runner.execution_base.segment_index == 1 && runner.execution_base.offset == 0);
	assert(runner.initial_pc.segment_index == 0 && runner.initial_pc.offset == 0);
	assert(runner.initial_fp.segment_index == 1 && runner.initial_fp.offset == 2);
	assert(runner.final_pc.segment_index == 3 && runner.final_pc.offset == 0);
	// Check vm data
	run_context run_context = runner.vm.run_context;
	assert(run_context.pc.segment_index == 0 && run_context.pc.offset == 0);
	assert(run_context.ap.segment_index == 1 && run_context.ap.offset == 2);
	assert(run_context.fp.segment_index == 1 && run_context.fp.offset == 2);
	// Check memory
	memory memory = runner.vm.memory;
	assert(memory.num_segments == 4);
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&memory, ptr);
	assert(!result.is_error);
	assert(felt_equal(result.value.memory_value.value.felt, felt_one));
	runner_free(&runner);
	printf("OK!\n");
}

void initialize_runner_no_builtins_no_proof_mode_empty_program(void) {
	struct CList *program_data = CList_init(sizeof(maybe_relocatable));
	struct program program = {0, program_data};
	cairo_runner runner = runner_new(program);
	runner_initialize(&runner);
	// Check runner data
	assert(runner.program_base.segment_index == 0 && runner.program_base.offset == 0);
	assert(runner.execution_base.segment_index == 1 && runner.execution_base.offset == 0);
	assert(runner.initial_pc.segment_index == 0 && runner.initial_pc.offset == 0);
	assert(runner.initial_fp.segment_index == 1 && runner.initial_fp.offset == 2);
	assert(runner.final_pc.segment_index == 3 && runner.final_pc.offset == 0);
	// Check vm data
	run_context run_context = runner.vm.run_context;
	assert(run_context.pc.segment_index == 0 && run_context.pc.offset == 0);
	assert(run_context.ap.segment_index == 1 && run_context.ap.offset == 2);
	assert(run_context.fp.segment_index == 1 && run_context.fp.offset == 2);
	// Check memory
	memory memory = runner.vm.memory;
	assert(memory.num_segments == 4);
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&memory, ptr);
	assert(result.is_error);
	runner_free(&runner);
	printf("OK!\n");
}

void runner_tests(void) {
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_non_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
}
