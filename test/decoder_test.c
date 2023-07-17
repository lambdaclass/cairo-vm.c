#include "decoder_test.h"
#include "decoder.h"

void decoding_tests(void) {

	printf("--------------------------------- \n");
	printf("Test: non_zero_high_bit \n");

	ResultInstruction err = decode_instruction(0x94A7800080008000);
	VirtualMachineError vm_error = err.value.error;
	assert(err.is_error);
	assert(vm_error == InstructionNonZeroHighBit);

	printf("OK! \n");

	printf("--------------------------------- \n");
	printf("Test: invalid_op1_reg \n");

	ResultInstruction err1 = decode_instruction(0x294F800080008000);
	VirtualMachineError vm_error1 = err1.value.error;
	assert(err1.is_error);
	assert(vm_error1 == InvalidOp1Reg);

	printf("OK! \n");

	printf("--------------------------------- \n");
	printf("Test: invalid_pc_update \n");

	ResultInstruction err2 = decode_instruction(0x29A8800080008000);
	VirtualMachineError vm_error2 = err2.value.error;
	assert(err2.is_error);
	assert(vm_error2 == InvalidPcUpdate);

	printf("OK! \n");

	printf("--------------------------------- \n");
	printf("Test: invalid_res_logic \n");

	ResultInstruction err3 = decode_instruction(0x2968800080008000);
	VirtualMachineError vm_error3 = err3.value.error;
	assert(err3.is_error);
	assert(vm_error3 == InvalidRes);

	printf("OK! \n");

	printf("--------------------------------- \n");
	printf("Test: invalid_res_logic \n");

	ResultInstruction err4 = decode_instruction(0x3948800080008000);
	VirtualMachineError vm_error4 = err4.value.error;
	assert(err4.is_error);
	assert(vm_error4 == InvalidOpcode);

	printf("OK! \n");

	printf("--------------------------------- \n");
	printf("Test: invalid_res_logic \n");

	ResultInstruction err5 = decode_instruction(0x2D48800080008000);
	VirtualMachineError vm_error5 = err5.value.error;
	assert(err5.is_error);
	assert(vm_error5 == InvalidApUpdate);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |    CALL|      ADD|     JUMP|      ADD|    IMM|     FP|     FP
	//  0  0  0  1      0  1   0  0  1      0  1 0  0  1       1       1
	//  0001 0100 1010 0111 = 0x14A7; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_flags_call_add_jmp_add_imm_fp_fp\n");
	ResultInstruction res = decode_instruction(0x14A7800080008000);
	Instruction instr = res.value.instruction;

	assert(instr.dst_register == REG_FP);
	assert(instr.op0_register == REG_FP);
	assert(instr.op1_addr == Imm);
	assert(instr.res == Res_Add);
	assert(instr.pc_update == Jump);
	assert(instr.ap_update == Add);
	assert(instr.opcode == Call);
	assert(instr.fp_update == APPlus2);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     RET|     ADD1| JUMP_REL|      MUL|     FP|     AP|     AP
	//  0  0  1  0      1  0   0  1  0      1  0 0  1  0       0       0
	//  0010 1001 0100 1000 = 0x2948; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_flags_ret_add1_jmp_rel_mul_fp_ap_ap\n");
	ResultInstruction res2 = decode_instruction(0x2948800080008000);
	Instruction instr2 = res2.value.instruction;

	assert(instr2.dst_register == REG_AP);
	assert(instr2.op0_register == REG_AP);
	assert(instr2.op1_addr == OP_FP);
	assert(instr2.res == Mul);
	assert(instr2.pc_update == JumpRel);
	assert(instr2.ap_update == Add1);
	assert(instr2.opcode == Ret);
	assert(instr2.fp_update == Dst);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |ASSRT_EQ|      ADD|      JNZ|      MUL|     AP|     AP|     AP
	//  0  1  0  0      1  0   1  0  0      1  0 1  0  0       0       0
	//  0100 1010 0101 0000 = 0x4A50; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_flags_assrt_add_jnz_mul_ap_ap_ap\n");
	ResultInstruction res3 = decode_instruction(0x4A50800080008000);
	Instruction instr3 = res3.value.instruction;

	assert(instr3.dst_register == REG_AP);
	assert(instr3.op0_register == REG_AP);
	assert(instr3.op1_addr == OP_AP);
	assert(instr3.res == Mul);
	assert(instr3.pc_update == Jnz);
	assert(instr3.ap_update == Add1);
	assert(instr3.opcode == AssertEq);
	assert(instr3.fp_update == FP_Regular);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |ASSRT_EQ|     ADD2|      JNZ|UNCONSTRD|    OP0|     AP|     AP
	//  0  1  0  0      0  0   1  0  0      0  0 0  0  0       0       0
	//  0100 0010 0000 0000 = 0x4200; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_flags_assrt_add2_jnz_uncon_op0_ap_ap\n");
	ResultInstruction res4 = decode_instruction(0x4200800080008000);
	Instruction instr4 = res4.value.instruction;

	assert(instr4.dst_register == REG_AP);
	assert(instr4.op0_register == REG_AP);
	assert(instr4.op1_addr == Op0);
	assert(instr4.res == Unconstrained);
	assert(instr4.pc_update == Jnz);
	assert(instr4.ap_update == AP_Regular);
	assert(instr4.opcode == AssertEq);
	assert(instr4.fp_update == FP_Regular);
	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     NOP|  REGULAR|  REGULAR|      OP1|    OP0|     AP|     AP
	//  0  0  0  0      0  0   0  0  0      0  0 0  0  0       0       0
	//  0000 0000 0000 0000 = 0x0000; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_flags_nop_regu_regu_op1_op0_ap_ap\n");
	ResultInstruction res5 = decode_instruction(0x0000800080008000);
	Instruction instr5 = res5.value.instruction;

	assert(instr5.dst_register == REG_AP);
	assert(instr5.op0_register == REG_AP);
	assert(instr5.op1_addr == Op0);
	assert(instr5.res == Op1);
	assert(instr5.pc_update == PC_Regular);
	assert(instr5.ap_update == AP_Regular);
	assert(instr5.opcode == NOp);
	assert(instr5.fp_update == FP_Regular);

	printf("OK! \n");

	//  0|  opcode|ap_update|pc_update|res_logic|op1_src|op0_reg|dst_reg
	// 15|14 13 12|    11 10|  9  8  7|     6  5|4  3  2|      1|      0
	//   |     NOP|  REGULAR|  REGULAR|      OP1|    OP0|     AP|     AP
	//  0  0  0  0      0  0   0  0  0      0  0 0  0  0       0       0
	//  0000 0000 0000 0000 = 0x0000; offx = 0
	printf("--------------------------------- \n");
	printf("Test: decode_offset_negative \n");
	ResultInstruction res6 = decode_instruction(0x0000800180007FFF);
	Instruction instr6 = res6.value.instruction;

	assert(instr6.off0 == -1);
	assert(instr6.off1 == 0);
	assert(instr6.off2 == 1);

	printf("OK! \n");
}
