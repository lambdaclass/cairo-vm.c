#ifndef RUN_CONTEXT_H
#define RUN_CONTEXT_H

#include "instruction.h"
#include "relocatable.h"
#include "utils.h"

typedef struct run_context {
	relocatable pc;
	relocatable ap;
	relocatable fp;
} run_context;

relocatable compute_dst_addr(run_context run_ctx, Instruction instr);

#endif
