#include "instruction.h"

unsigned int instruction_size(Instruction ins) {
	if (ins.op1_addr == Imm)
		return 2;
	else
		return 1;
}