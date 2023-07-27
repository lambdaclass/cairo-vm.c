#ifndef RUNNER_H
#define RUNNER_H

#include "collectc/cc_array.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"

typedef union relocated_memory_value {
	felt_t value;
	int none;
} relocated_memory_value;

typedef struct relocated_memory_cell {
	relocated_memory_value memory_value;
	bool is_some;
} relocated_memory_cell;

typedef struct cairo_runner {
	struct program program;
	virtual_machine vm;
	CC_Array *relocated_memory;
	relocatable program_base;
	relocatable execution_base;
	relocatable initial_pc;
	relocatable initial_fp;
	relocatable final_pc;

} cairo_runner;

// Creates a new, empty cairo_runner
cairo_runner runner_new(struct program program);

// Frees resources used by the cairo_runner struct
void runner_free(cairo_runner *runner);

// Performs the intialization step, leaving the runner ready to run a loaded cairo program from a main entrypoint
relocatable runner_initialize(cairo_runner *runner);

void runner_relocate_memory(cairo_runner *runner);

#endif
