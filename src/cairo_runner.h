#ifndef RUNNER_H
#define RUNNER_H

#include "clist.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"

typedef struct cairo_runner {
	struct program program;
	virtual_machine vm;
	relocatable program_base;
	relocatable execution_base;
	relocatable initial_pc;
	relocatable initial_fp;
	relocatable final_pc;

} cairo_runner;

// Error Handling

union ResultRunnerValue {
	enum MemoryError memory_error;
	relocatable ptr;
	int none;
};

typedef struct ResultRunner {
	union ResultRunnerValue value;
	bool is_error;
} ResultRunner;

cairo_runner runner_new(struct program program);

void runner_free(cairo_runner *runner);

ResultRunner runner_initialize(cairo_runner *runner);

#endif
