#ifndef UTILS_H
#define UTILS_H

#include "../lambdaworks/lib/lambdaworks.h"
#include "stdint.h"
#include "stdio.h"
#include <stdbool.h>

bool felt_equal(felt_t a, felt_t b);
int64_t absolute(int64_t a);

uint8_t *u64_to_le_bytes(uint64_t value, uint8_t *buffer_bytes);
uint8_t *u64_to_be_bytes(uint64_t value, uint8_t *buffer_bytes);
uint64_t u64_from_le_bytes(uint8_t *bytes);
uint64_t u64_from_be_bytes(uint8_t *bytes);

uint8_t *u32_to_le_bytes(uint32_t value, uint8_t *buffer_bytes);
uint8_t *u32_to_be_bytes(uint32_t value, uint8_t *buffer_bytes);
uint32_t u32_from_le_bytes(uint8_t *bytes);
uint32_t u32_from_be_bytes(uint8_t *bytes);

uint8_t *u16_to_le_bytes(uint16_t value, uint8_t *buffer_bytes);
uint8_t *u16_to_be_bytes(uint16_t value, uint8_t *buffer_bytes);
uint16_t u16_from_le_bytes(uint8_t *bytes);
uint16_t u16_from_be_bytes(uint8_t *bytes);

#endif
