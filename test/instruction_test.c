#include "instruction_test.h"

void instruction_size_test(void) {

	Instruction instr_1 = {
	    .off0 = 0,
	    .off1 = 0,
	    .off2 = 0,
	    .dst_register = REG_AP,
	    .op0_register = REG_FP,
	    .op1_addr = Imm,
	    .res = Op1,
	    .pc_update = PC_Regular,
	    .ap_update = AP_Regular,
	    .fp_update = FP_Regular,
	    .opcode = NOp,
	};

	uint64_t a = instruction_size(instr_1);
	assert(a == 2);

	Instruction instr_2 = {
	    .off0 = 0,
	    .off1 = 0,
	    .off2 = 0,
	    .dst_register = REG_AP,
	    .op0_register = REG_FP,
	    .op1_addr = OP_FP,
	    .res = Op1,
	    .pc_update = PC_Regular,
	    .ap_update = AP_Regular,
	    .fp_update = FP_Regular,
	    .opcode = NOp,
	};

	uint64_t b = instruction_size(instr_2);
	assert(b == 1);
}
