#ifndef RUNNER_H
#define RUNNER_H

#include "clist.h"
#include "relocatable.h"
#include "vm.h"
#include "parser.h"

typedef struct cairo_runner {
	Program program;
	virtual_machine vm;
	relocatable program_base;
	relocatable execution_base;
	relocatable initial_pc;
	relocatable initial_fp;
	relocatable final_pc;

} cairo_runner;

// Creates a new, empty cairo_runner
cairo_runner runner_new(Program program);

// Frees resources used by the cairo_runner struct
void runner_free(cairo_runner *runner);

// Performs the intialization step, leaving the runner ready to run a loaded cairo program from a main entrypoint
relocatable runner_initialize(cairo_runner *runner);

#endif
