#include "decoder.h"

int64_t decode_offset(uint64_t offset) {
	uint8_t buffer_bytes[8];
	uint8_t *vectorized_offset = u64_to_le_bytes(offset, buffer_bytes);
	uint8_t offset_bytes[2] = {vectorized_offset[0], vectorized_offset[1]};
	uint16_t offset_16b_encoded = u16_from_le_bytes(offset_bytes);
	uint16_t complement_const = 0x8000;

	// overflow wrap around is standard behaviour of C uints
	uint16_t offset_16b = offset_16b_encoded - complement_const;

	return (int64_t)offset_16b;
}

ResultInstruction decode_instruction(uint64_t encoded_instr) {
	const uint64_t HIGH_BIT = 1ULL << 63;
	const uint64_t DST_REG_MASK = 0x0001;
	const uint64_t DST_REG_OFF = 0;
	const uint64_t OP0_REG_MASK = 0x0002;
	const uint64_t OP0_REG_OFF = 1;
	const uint64_t OP1_SRC_MASK = 0x001C;
	const uint64_t OP1_SRC_OFF = 2;
	const uint64_t RES_LOGIC_MASK = 0x0060;
	const uint64_t RES_LOGIC_OFF = 5;
	const uint64_t PC_UPDATE_MASK = 0x0380;
	const uint64_t PC_UPDATE_OFF = 7;
	const uint64_t AP_UPDATE_MASK = 0x0C00;
	const uint64_t AP_UPDATE_OFF = 10;
	const uint64_t OPCODE_MASK = 0x7000;
	const uint64_t OPCODE_OFF = 12;

	// Flags start on the 48th bit.
	const uint64_t FLAGS_OFFSET = 48;
	const uint64_t OFF0_OFF = 0;
	const uint64_t OFF1_OFF = 16;
	const uint64_t OFF2_OFF = 32;
	const uint64_t OFFX_MASK = 0xFFFF;

	if ((encoded_instr & HIGH_BIT) != 0) {
		ResultInstruction error = {.is_error = true, .value = {InstructionNonZeroHighBit}};
		return error;
	}

	int64_t off0 = decode_offset(encoded_instr >> OFF0_OFF & OFFX_MASK);
	int64_t off1 = decode_offset(encoded_instr >> OFF1_OFF & OFFX_MASK);
	int64_t off2 = decode_offset(encoded_instr >> OFF2_OFF & OFFX_MASK);

	uint64_t flags = encoded_instr >> FLAGS_OFFSET;

	uint64_t dst_reg_num = (flags & DST_REG_MASK) >> DST_REG_OFF;
	uint64_t op0_reg_num = (flags & OP0_REG_MASK) >> OP0_REG_OFF;
	uint64_t op1_src_num = (flags & OP1_SRC_MASK) >> OP1_SRC_OFF;
	uint64_t res_logic_num = (flags & RES_LOGIC_MASK) >> RES_LOGIC_OFF;
	uint64_t pc_update_num = (flags & PC_UPDATE_MASK) >> PC_UPDATE_OFF;
	uint64_t ap_update_num = (flags & AP_UPDATE_MASK) >> AP_UPDATE_OFF;
	uint64_t opcode_num = (flags & OPCODE_MASK) >> OPCODE_OFF;

	Register dst_register;
	if (dst_reg_num == 1)
		dst_register = REG_FP;
	else
		dst_register = REG_AP;

	Register op0_register;
	if (op0_reg_num == 1)
		op0_register = REG_FP;
	else
		op0_register = REG_AP;

	//-------------
	//   Op1Addr
	//-------------

	Op1Addr op1_addr;

	switch (op1_src_num) {
	case 0:
		op1_addr = Op0;
		break;
	case 1:
		op1_addr = Imm;
		break;
	case 2:
		op1_addr = OP_FP;
		break;
	case 4:
		op1_addr = OP_AP;
		break;

	default: {
		ResultInstruction error = {.is_error = true, .value = {InvalidOp1Reg}};
		return error;

	} break;
	}

	//-------------
	//   PcUpdate
	//-------------

	PcUpdate pc_update;

	switch (pc_update_num) {
	case 0:
		pc_update = PC_Regular;
		break;
	case 1:
		pc_update = Jump;
		break;
	case 2:
		pc_update = JumpRel;
		break;
	case 4:
		pc_update = Jnz;
		break;

	default: {
		ResultInstruction error = {.is_error = true, .value = {InvalidPcUpdate}};
		return error;

	} break;
	}

	//-------------
	//     Res
	//-------------

	Res res;

	switch (res_logic_num) {
	case 0: {
		if (pc_update == Jnz)
			res = Unconstrained;
		else
			res = Op1;
	} break;
	case 1:
		res = Res_Add;
		break;
	case 2:
		res = Mul;
		break;

	default: {
		ResultInstruction error = {.is_error = true, .value = {InvalidRes}};
		return error;

	} break;
	}

	//-------------
	//   Opcode
	//-------------

	Opcode opcode;

	switch (opcode_num) {
	case 0:
		opcode = NOp;
		break;
	case 1:
		opcode = Call;
		break;
	case 2:
		opcode = Ret;
		break;
	case 4:
		opcode = AssertEq;
		break;

	default: {
		ResultInstruction error = {.is_error = true, .value = {InvalidOpcode}};
		return error;

	} break;
	}

	//-------------
	//   ApUpdate
	//-------------

	ApUpdate ap_update;

	switch (ap_update_num) {
	case 0: {
		if (opcode == Call)
			ap_update = Add2;
		else
			ap_update = AP_Regular;
	} break;
	case 1:
		ap_update = Add;
		break;
	case 2:
		ap_update = Add1;
		break;

	default: {
		ResultInstruction error = {.is_error = true, .value = {InvalidApUpdate}};
		return error;

	} break;
	}

	//-------------
	//   FpUpdate
	//-------------

	FpUpdate fp_update;

	switch (opcode) {
	case Call:
		fp_update = APPlus2;
		break;
	case Ret:
		fp_update = Dst;
		break;
	default:
		fp_update = FP_Regular;
		break;
	}

	//----------------
	//   Instruction
	//----------------
	Instruction instr = {
	    .off0 = off0,
	    .off1 = off1,
	    .off2 = off2,
	    .dst_register = dst_register,
	    .op0_register = op0_register,
	    .op1_addr = op1_addr,
	    .res = res,
	    .pc_update = pc_update,
	    .ap_update = ap_update,
	    .fp_update = fp_update,
	    .opcode = opcode,
	};

	ResultInstructionValue val = {.instruction = instr};
	ResultInstruction ret = {.is_error = false, .value = val};
	return ret;
}
