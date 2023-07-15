#include "decoder.h"

ResultInstruction decode_instruction(uint64_t encoded_ins)
{
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

	if ((encoded_ins & HIGH_BIT) != 0)
	{
		ResultInstruction ret = {.is_error = true, .value = InstructionNonZeroHighBit};
		return ret;
	}
}
