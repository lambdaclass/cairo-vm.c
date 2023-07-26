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

// ---------------------
// execution structures
// ---------------------

typedef struct {
	maybe_relocatable dst;
	memory_cell res;
	maybe_relocatable op0;
	maybe_relocatable op1;
} operands;

typedef struct {
	relocatable dst_addr;
	relocatable op0_addr;
	relocatable op1_addr;
} operands_address;

typedef struct {
	operands oprs;
	operands_address op_addrs;
	uint8_t deduced_operands;
} computed_operands;

typedef union {
	computed_operands ops;
	VirtualMachineError error;
} computed_operands_value;

typedef struct {
	computed_operands_value value;
	bool is_error;
} computed_operands_res;

// ---------------------
//    VM functions
// ---------------------

// Creates a new, empty virtual_machine
virtual_machine vm_new(void);
// executes an instruction in the vm
vm_result run_instruction(virtual_machine vm, Instruction instr);
// Creates a new, empty virtual_machine

#endif
