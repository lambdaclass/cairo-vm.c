#include "utils.h"
#include "relocatable.h"



maybe_relocatable add_maybe_relocatable(maybe_relocatable a, maybe_relocatable b){
	if(a.is_felt && b.is_felt) {
		
			felt_t f1 = {a.value.felt[0], a.value.felt[1], a.value.felt[2], a.value.felt[3]};
			felt_t f2 = {b.value.felt[0], b.value.felt[1], b.value.felt[2], b.value.felt[3]};
			felt_t result;
			add(f1, f2, result);
			maybe_relocatable res = {.is_felt = true, .value = {.felt = {result[0], result[1], result[2], result[3]}}};
		return res;
	}
}



int64_t absolute(int64_t a) {
	if (a < 0)
		return (-1) * a;
	else
		return a;
}

bool felt_equal(felt_t a, felt_t b) { return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]); }

// ------------
//   u64 bits
// ------------

uint8_t *u64_to_le_bytes(uint64_t value, uint8_t *buffer_bytes) {
	for (int i = 0; i < 8; i++) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint8_t *u64_to_be_bytes(uint64_t value, uint8_t *buffer_bytes) {
	for (int i = 7; i >= 0; i--) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint64_t u64_from_le_bytes(uint8_t *bytes) {
	uint64_t value = 0;
	for (int i = 7; i >= 0; i--) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}

uint64_t u64_from_be_bytes(uint8_t *bytes) {
	uint64_t value = 0;
	for (int i = 0; i < 8; i++) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}

// ------------
//   u32 bits
// ------------

uint8_t *u32_to_le_bytes(uint32_t value, uint8_t *buffer_bytes) {
	for (int i = 0; i < 4; i++) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint8_t *u32_to_be_bytes(uint32_t value, uint8_t *buffer_bytes) {
	for (int i = 3; i >= 0; i--) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint32_t u32_from_le_bytes(uint8_t *bytes) {
	uint16_t value = 0;
	for (int i = 3; i >= 0; i--) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}

uint32_t u32_from_be_bytes(uint8_t *bytes) {
	uint64_t value = 0;
	for (int i = 0; i < 4; i++) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}

// ------------
//   u16 bits
// ------------

uint8_t *u16_to_le_bytes(uint16_t value, uint8_t *buffer_bytes) {
	for (int i = 0; i < 2; i++) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint8_t *u16_to_be_bytes(uint16_t value, uint8_t *buffer_bytes) {
	for (int i = 1; i >= 0; i--) {
		buffer_bytes[i] = value >> (8 * i) & 0xFF;
	}
	return buffer_bytes;
}

uint16_t u16_from_le_bytes(uint8_t *bytes) {
	uint16_t value = 0;
	for (int i = 1; i >= 0; i--) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}

uint16_t u16_from_be_bytes(uint8_t *bytes) {
	uint16_t value = 0;
	for (int i = 0; i < 2; i++) {
		value |= bytes[i] << (8 * i);
	}
	return value;
}
