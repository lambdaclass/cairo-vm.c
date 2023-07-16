#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"
#include "stdio.h"

uint8_t *u64_to_le_bytes(uint64_t value);
uint8_t *u64_to_be_bytes(uint64_t value);
uint64_t u64_from_le_bytes(uint8_t *bytes);
uint64_t u64_from_be_bytes(uint8_t *bytes);

uint8_t *u16_to_le_bytes(uint16_t value);
uint16_t u16_from_le_bytes(uint8_t *bytes);

#endif