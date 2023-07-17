#include "runner_tests.h"
#include "program.h"
#include "vm.h"
#include "clist.h"
#include <stdio.h>

void initialize_runner_no_builtins_no_proof_mode_empty_program(void) {
    struct CList * program_data = CList_init(sizeof(maybe_relocatable));
    struct program program = {1, program_data};
    cairo_runner runner = {program, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    struct CList * mem_data = CList_init(10 * sizeof(struct CList *));
    printf("%i\n", mem_data);
    virtual_machine vm = {{{0,0},{0,0},{0,0}}, {mem_data}};
    relocatable end = runner_initialize(&runner, &vm);

}

void runner_tests(void) {
    printf("--------------------------------- \n");
    printf("Test: initialize_runner_no_builtins_no_proof_mode_empty_program \n");
    initialize_runner_no_builtins_no_proof_mode_empty_program();
}

