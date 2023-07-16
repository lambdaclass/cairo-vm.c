#include "decoder.h"

uint64_t decode_offset(uint64_t offset)
{
	uint8_t *vectorized_offset = u64_to_le_bytes(offset);
	uint8_t to_encode[2] = {vectorized_offset[0], vectorized_offset[1]};
	uint16_t offset_16b_encoded = u16_from_le_bytes(to_encode);
	uint16_t complement_const = 0x8000;

	return 1;
}

ResultInstruction decode_instruction(uint64_t encoded_instr)
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

	if ((encoded_instr & HIGH_BIT) != 0)
	{
		ResultInstruction error = {.is_error = true, .value = InstructionNonZeroHighBit};
		return error;
	}

	uint64_t off0 = decode_offset(encoded_instr >> OFF0_OFF & OFFX_MASK);
	uint64_t off1 = decode_offset(encoded_instr >> OFF1_OFF & OFFX_MASK);
	uint64_t off2 = decode_offset(encoded_instr >> OFF2_OFF & OFFX_MASK);
}
