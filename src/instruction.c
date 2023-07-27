#include "instruction.h"

uint64_t instruction_size(Instruction instruction) {
	if (instruction.op1_addr == Imm) {
		return 2;
	}
	return 1;
}
