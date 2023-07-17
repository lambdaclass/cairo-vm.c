#ifndef RUNNER_H
#define RUNNER_H

#include "clist.h"
#include "program.h"
#include "relocatable.h"
#include "vm.h"

typedef struct cairo_runner
{
	struct program program;
	relocatable program_base;
	relocatable execution_base;
	relocatable initial_pc;
	relocatable initial_fp;
	relocatable final_pc;

} cairo_runner;

#endif
