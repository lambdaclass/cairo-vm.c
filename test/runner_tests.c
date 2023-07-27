#include "runner_tests.h"
#include "cairo_runner.h"
#include "collectc/cc_array.h"
#include "collectc/cc_common.h"
#include "memory.h"
#include "program.h"
#include "relocatable.h"
#include "run_context.h"
#include "utils.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>

void initialize_runner_no_builtins_no_proof_mode_non_empty_program(void) {
	CC_Array *program_data;
	cc_array_new(&program_data);
	felt_t felt_one;
	one(felt_one);
	maybe_relocatable *elem = malloc(sizeof(maybe_relocatable));
	*elem = maybe_relocatable_from_felt_limbs(felt_one);
	cc_array_add(program_data, elem);
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
	CC_Array *program_data;
	cc_array_new(&program_data);
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

void relocate_runner_memory(void) {
	CC_Array *program_data;
	cc_array_new(&program_data);
	struct program program = {0, program_data};
	cairo_runner runner = runner_new(program);
	runner_initialize(&runner);

	relocatable ptr_0_0 = {0, 0};
	relocatable ptr_0_1 = {0, 1};
	relocatable ptr_0_2 = {0, 2};
	relocatable ptr_1_0 = {1, 0};
	relocatable ptr_1_1 = {1, 1};
	relocatable ptr_1_5 = {1, 5};
	felt_t value_0_0;
	felt_t value_0_1;
	felt_t value_0_2;
	relocatable value_1_0 = {2, 0};
	relocatable value_1_1 = {3, 0};
	felt_t value_1_5;
	from(value_0_0, 4613515612218425347);
	from(value_0_1, 5);
	from(value_0_2, 2345108766317314046);
	from(value_1_5, 5);
	maybe_relocatable elem_0_0 = maybe_relocatable_from_felt_limbs(value_0_0);
	maybe_relocatable elem_0_1 = maybe_relocatable_from_felt_limbs(value_0_1);
	maybe_relocatable elem_0_2 = maybe_relocatable_from_felt_limbs(value_0_2);
	maybe_relocatable elem_1_0 = {.value = {.relocatable = value_1_0}, .is_felt = false};
	maybe_relocatable elem_1_1 = {.value = {.relocatable = value_1_1}, .is_felt = false};
	maybe_relocatable elem_1_5 = maybe_relocatable_from_felt_limbs(value_1_5);
	memory_insert(&runner.vm.memory, ptr_0_0, elem_0_0);
	memory_insert(&runner.vm.memory, ptr_0_1, elem_0_1);
	memory_insert(&runner.vm.memory, ptr_0_2, elem_0_2);
	memory_insert(&runner.vm.memory, ptr_1_0, elem_1_0);
	memory_insert(&runner.vm.memory, ptr_1_1, elem_1_1);
	memory_insert(&runner.vm.memory, ptr_1_5, elem_1_5);

	runner_relocate_memory(&runner);

	relocated_memory_cell *relocated_memory_0 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 0, (void **)&relocated_memory_0) == CC_OK);
	assert(!relocated_memory_0->is_some);
	relocated_memory_cell *relocated_memory_1 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 1, (void **)&relocated_memory_1) == CC_OK);
	felt_t expected_1;
	from(expected_1, 4613515612218425347);
	assert(relocated_memory_1->is_some);
	assert(felt_equal(relocated_memory_1->memory_value.value, expected_1));
	relocated_memory_cell *relocated_memory_2 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 2, (void **)&relocated_memory_2) == CC_OK);
	felt_t expected_2;
	from(expected_2, 5);
	assert(relocated_memory_2->is_some);
	assert(felt_equal(relocated_memory_2->memory_value.value, expected_2));
	relocated_memory_cell *relocated_memory_3 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 3, (void **)&relocated_memory_3) == CC_OK);
	felt_t expected_3;
	from(expected_3, 2345108766317314046);
	assert(relocated_memory_3->is_some);
	assert(felt_equal(relocated_memory_3->memory_value.value, expected_3));
	relocated_memory_cell *relocated_memory_4 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 4, (void **)&relocated_memory_4) == CC_OK);
	felt_t expected_4;
	from(expected_4, 10);
	assert(relocated_memory_4->is_some);
	assert(felt_equal(relocated_memory_4->memory_value.value, expected_4));
	relocated_memory_cell *relocated_memory_5 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 5, (void **)&relocated_memory_5) == CC_OK);
	felt_t expected_5;
	from(expected_5, 10);
	assert(relocated_memory_5->is_some);
	assert(felt_equal(relocated_memory_5->memory_value.value, expected_5));
	relocated_memory_cell *relocated_memory_6 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 6, (void **)&relocated_memory_6) == CC_OK);
	assert(!relocated_memory_6->is_some);
	relocated_memory_cell *relocated_memory_7 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 7, (void **)&relocated_memory_7) == CC_OK);
	assert(!relocated_memory_7->is_some);
	relocated_memory_cell *relocated_memory_8 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 8, (void **)&relocated_memory_8) == CC_OK);
	assert(!relocated_memory_8->is_some);
	relocated_memory_cell *relocated_memory_9 = NULL;
	assert(cc_array_get_at(runner.relocated_memory, 9, (void **)&relocated_memory_9) == CC_OK);
	felt_t expected_9;
	from(expected_9, 5);
	assert(relocated_memory_9->is_some);
	assert(felt_equal(relocated_memory_9->memory_value.value, expected_9));
}

void runner_tests(void) {
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_non_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_non_empty_program();
	printf("--------------------------------- \n");
	printf("Test: relocate_runner_memory \n");
	relocate_runner_memory();
}
