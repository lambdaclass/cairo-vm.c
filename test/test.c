#include "decoder.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {

	// tests of decoding

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |    CALL|      ADD|     JUMP|      ADD|    IMM|     FP|     FP
	//  0  0  0  1      0  1   0  0  1      0  1 0  0  1       1       1
	//  0001 0100 1010 0111 = 0x14A7; offx = 0
	ResultInstruction res = decode_instruction(0x14A7800080008000);
	Instruction instr = res.value.instruction;

	if (instr.dst_register == REG_FP) {
		assert(1);
	}
	if (instr.op0_register == REG_FP) {
		assert(1);
	}
	if (instr.op1_addr == Imm) {
		assert(1);
	}
	if (instr.res == Res_Add) {
		assert(1);
	}
	if (instr.pc_update == Jump) {
		assert(1);
	}
	if (instr.ap_update == Add) {
		assert(1);
	}
	if (instr.opcode == Call) {
		assert(1);
	}
	if (instr.fp_update == APPlus2) {
		assert(1);
	}

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     RET|     ADD1| JUMP_REL|      MUL|     FP|     AP|     AP
	//  0  0  1  0      1  0   0  1  0      1  0 0  1  0       0       0
	//  0010 1001 0100 1000 = 0x2948; offx = 0
	ResultInstruction res2 = decode_instruction(0x2948800080008000);
	Instruction instr2 = res2.value.instruction;

	if (instr2.dst_register == REG_AP) {
		assert(1);
	}
	if (instr2.op0_register == REG_AP) {
		assert(1);
	}
	if (instr2.op1_addr == OP_FP) {
		assert(1);
	}
	if (instr2.res == Mul) {
		assert(1);
	}
	if (instr2.pc_update == JumpRel) {
		assert(1);
	}
	if (instr2.ap_update == Add1) {
		assert(1);
	}
	if (instr2.opcode == Ret) {
		assert(1);
	}
	if (instr2.fp_update == Dst) {
		assert(1);
	}
	printf("Tests passed!\n");

	return 0;
}
