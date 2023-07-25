#ifndef RUN_CONTEXT_H
#define RUN_CONTEXT_H

#include "relocatable.h"

typedef struct run_context {
	relocatable pc;
	relocatable ap;
	relocatable fp;
} run_context;

#endif
