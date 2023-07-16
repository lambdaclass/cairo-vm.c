#include "utils.h"
// ------------
//   u64 bits
// ------------

uint8_t *u64_to_le_bytes(uint64_t value)
{
	uint8_t bytes[8];
	for (int i = 0; i < 8; i++)
	{
		bytes[i] = value >> (8 * i) & 0xFF;
	}
	return bytes;
}

uint8_t *u64_to_be_bytes(uint64_t value)
{
	uint8_t bytes[8];
	for (int i = 7; i >= 0; i--)
	{
		bytes[i] = value >> (8 * i) & 0xFF;
	}
	return bytes;
}

uint64_t u64_from_le_bytes(uint8_t *bytes)
{
	uint64_t value = 0;
	for (int i = 7; i >= 0; i--)
	{
		value |= bytes[i];
		value = value << (8 * i);
	}
	return value;
}

// TODO: needs fix
uint64_t u64_from_be_bytes(uint8_t *bytes)
{
	uint64_t value = 0;
	for (int i = 0; i < 8; i++)
	{
		value |= bytes[i];
		value = value << (8 * i);
	}
	return value;
}

// ------------
//   u16 bits
// ------------

uint8_t *u16_to_le_bytes(uint16_t value)
{
	uint8_t bytes[2];
	for (int i = 0; i < 2; i++)
	{
		bytes[i] = value >> (8 * i) & 0xFF;
	}
	return bytes;
}

uint16_t u16_from_le_bytes(uint8_t *bytes)
{
	uint16_t value = 0;
	for (int i = 1; i >= 0; i--)
	{
		value |= bytes[i];
		value = value << (8 * i);
	}
	return value;
}

// ------------
//   u32 bits
// ------------

uint8_t *u32_to_le_bytes(uint32_t value)
{
	uint8_t bytes[4];
	for (int i = 0; i < 4; i++)
	{
		bytes[i] = value >> (8 * i) & 0xFF;
	}
	return bytes;
}

uint32_t u32_from_le_bytes(uint8_t *bytes)
{
	uint16_t value = 0;
	for (int i = 3; i >= 0; i--)
	{
		value |= bytes[i];
		value = value << (8 * i);
	}
	return value;
}