#include "runner_tests.h"
#include "clist.h"
#include "memory.h"
#include "program.h"
#include "vm.h"
#include <stdio.h>

void initialize_runner_no_builtins_no_proof_mode_empty_program(void)
{
	struct CList *program_data = CList_init(sizeof(maybe_relocatable));
	struct program program = {1, program_data};
	cairo_runner runner = runner_new(program);
	runner_initialize(&runner);
}

void runner_tests(void)
{
	printf("--------------------------------- \n");
	printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
	initialize_runner_no_builtins_no_proof_mode_empty_program();
}
