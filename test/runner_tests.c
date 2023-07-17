#include "runner_tests.h"
#include "clist.h"
#include "memory.h"
#include "run_context.h"
#include "program.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>

void initialize_runner_no_builtins_no_proof_mode_empty_program(void)
{
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
    assert(memory.data->count(memory.data) == 4);
    // check individual segments
    struct CList * segment_0 = memory.data->at(memory.data, 0);
    assert(segment_0->count(segment_0) == 0);

}

void runner_tests(void)
{
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
}
