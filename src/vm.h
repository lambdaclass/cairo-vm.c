#ifndef VM_H
#define VM_H

#include "instruction.h"
#include "memory.h"
#include "run_context.h"

typedef struct {
	enum VirtualMachineError error;
	bool is_ok;
} vm_result;

typedef struct {
	run_context run_context;
	memory memory;
} virtual_machine;

// Creates a new, empty virtual_machine
virtual_machine vm_new(void);

#endif
