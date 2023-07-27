#include "runner_tests.h"
#include "clist.h"
#include "memory.h"
#include "run_context.h"
#include "utils.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void initialize_runner_no_builtins_no_proof_mode_non_empty_program(void) {
	Program program = get_empty_program();
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable elem = maybe_relocatable_from_felt_limbs(felt_one);
	program.data->add(program.data, &elem);
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
	// program segment
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&memory, ptr);
	assert(!result.is_error);
	assert(felt_equal(result.value.memory_value.value.felt, felt_one));
	// execution segment
	relocatable ptr_b = {1, 0};
	ResultMemory result_b = memory_get(&memory, ptr_b);
	assert(!result_b.is_error);
	relocatable value_b = result_b.value.memory_value.value.relocatable;
	assert(value_b.segment_index == 2 && value_b.offset == 0);

	relocatable ptr_c = {1, 1};
	ResultMemory result_c = memory_get(&memory, ptr_c);
	assert(!result_c.is_error);
	relocatable value_c = result_c.value.memory_value.value.relocatable;
	assert(value_c.segment_index == 3 && value_c.offset == 0);

	runner_free(&runner);
	printf("OK!\n");
}

void initialize_runner_no_builtins_no_proof_mode_empty_program(void) {
	Program empty_program = get_empty_program();
	cairo_runner runner = runner_new(empty_program);
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
	// program segment
	memory memory = runner.vm.memory;
	assert(memory.num_segments == 4);
	relocatable ptr = {0, 0};
	ResultMemory result = memory_get(&memory, ptr);
	assert(result.is_error);
	// execution segment
	relocatable ptr_b = {1, 0};
	ResultMemory result_b = memory_get(&memory, ptr_b);
	assert(!result_b.is_error);
	relocatable value_b = result_b.value.memory_value.value.relocatable;
	assert(value_b.segment_index == 2 && value_b.offset == 0);

	relocatable ptr_c = {1, 1};
	ResultMemory result_c = memory_get(&memory, ptr_c);
	assert(!result_c.is_error);
	relocatable value_c = result_c.value.memory_value.value.relocatable;
	assert(value_c.segment_index == 3 && value_c.offset == 0);
	runner_free(&runner);
	printf("OK!\n");
}

void runner_tests(void) {
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_non_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_non_empty_program();
}
