#ifndef VM_H
#define VM_H

#include "memory.h"
#include "run_context.h"
typedef struct virtual_machine {
	run_context run_context;
	memory memory;
} virtual_machine;

// Creates a new, empty virtual_machine
virtual_machine vm_new(void);

#endif
