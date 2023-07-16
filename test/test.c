#include "decoder.h"
#include <assert.h>
#include <stdio.h>

void decoding_tests() {
	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |    CALL|      ADD|     JUMP|      ADD|    IMM|     FP|     FP
	//  0  0  0  1      0  1   0  0  1      0  1 0  0  1       1       1
	//  0001 0100 1010 0111 = 0x14A7; offx = 0

	printf("Test: decode_flags_call_add_jmp_add_imm_fp_fp\n");
	ResultInstruction res = decode_instruction(0x14A7800080008000);
	Instruction instr = res.value.instruction;

	if (instr.dst_register == REG_FP)
		assert(1);
	if (instr.op0_register == REG_FP)
		assert(1);
	if (instr.op1_addr == Imm)
		assert(1);
	if (instr.res == Res_Add)
		assert(1);
	if (instr.pc_update == Jump)
		assert(1);
	if (instr.ap_update == Add)
		assert(1);
	if (instr.opcode == Call)
		assert(1);
	if (instr.fp_update == APPlus2)
		assert(1);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     RET|     ADD1| JUMP_REL|      MUL|     FP|     AP|     AP
	//  0  0  1  0      1  0   0  1  0      1  0 0  1  0       0       0
	//  0010 1001 0100 1000 = 0x2948; offx = 0

	printf("Test: decode_flags_ret_add1_jmp_rel_mul_fp_ap_ap\n");
	ResultInstruction res2 = decode_instruction(0x2948800080008000);
	Instruction instr2 = res2.value.instruction;

	if (instr2.dst_register == REG_AP)
		assert(1);
	if (instr2.op0_register == REG_AP)
		assert(1);
	if (instr2.op1_addr == OP_FP)
		assert(1);
	if (instr2.res == Mul)
		assert(1);
	if (instr2.pc_update == JumpRel)
		assert(1);
	if (instr2.ap_update == Add1)
		assert(1);
	if (instr2.opcode == Ret)
		assert(1);
	if (instr2.fp_update == Dst)
		assert(1);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |ASSRT_EQ|      ADD|      JNZ|      MUL|     AP|     AP|     AP
	//  0  1  0  0      1  0   1  0  0      1  0 1  0  0       0       0
	//  0100 1010 0101 0000 = 0x4A50; offx = 0

	printf("Test: decode_flags_assrt_add_jnz_mul_ap_ap_ap\n");
	ResultInstruction res3 = decode_instruction(0x4A50800080008000);
	Instruction instr3 = res3.value.instruction;

	if (instr3.dst_register == REG_AP)
		assert(1);
	if (instr3.op0_register == REG_AP)
		assert(1);
	if (instr3.op1_addr == OP_AP)
		assert(1);
	if (instr3.res == Mul)
		assert(1);
	if (instr3.pc_update == Jnz)
		assert(1);
	if (instr3.ap_update == Add1)
		assert(1);
	if (instr3.opcode == AssertEq)
		assert(1);
	if (instr3.fp_update == FP_Regular)
		assert(1);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |ASSRT_EQ|     ADD2|      JNZ|UNCONSTRD|    OP0|     AP|     AP
	//  0  1  0  0      0  0   1  0  0      0  0 0  0  0       0       0
	//  0100 0010 0000 0000 = 0x4200; offx = 0
	printf("Test: decode_flags_assrt_add2_jnz_uncon_op0_ap_ap\n");
	ResultInstruction res4 = decode_instruction(0x4200800080008000);
	Instruction instr4 = res4.value.instruction;

	if (instr4.dst_register == REG_AP)
		assert(1);
	if (instr4.op0_register == REG_AP)
		assert(1);
	if (instr4.op1_addr == Op0)
		assert(1);
	if (instr4.res == Unconstrained)
		assert(1);
	if (instr4.pc_update == Jnz)
		assert(1);
	if (instr4.ap_update == AP_Regular)
		assert(1);
	if (instr4.opcode == AssertEq)
		assert(1);
	if (instr4.fp_update == FP_Regular)
		assert(1);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     NOP|  REGULAR|  REGULAR|      OP1|    OP0|     AP|     AP
	//  0  0  0  0      0  0   0  0  0      0  0 0  0  0       0       0
	//  0000 0000 0000 0000 = 0x0000; offx = 0
	printf("Test: decode_flags_nop_regu_regu_op1_op0_ap_ap\n");
	ResultInstruction res5 = decode_instruction(0x0000800080008000);
	Instruction instr5 = res5.value.instruction;

	if (instr5.dst_register == REG_AP)
		assert(1);
	if (instr5.op0_register == REG_AP)
		assert(1);
	if (instr5.op1_addr == Op0)
		assert(1);
	if (instr5.res == Op1)
		assert(1);
	if (instr5.pc_update == PC_Regular)
		assert(1);
	if (instr5.ap_update == AP_Regular)
		assert(1);
	if (instr5.opcode == NOp)
		assert(1);
	if (instr5.fp_update == FP_Regular)
		assert(1);
		
	printf("OK! \n");
}

int main(int argc, char **argv) {
	decoding_tests();

	printf("Tests passed!\n");
	return 0;
}
