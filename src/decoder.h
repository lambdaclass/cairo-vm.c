#ifndef DECO_H
#define DECO_H
#include "instruction.h"
#include "stdint.h"
#include "stdio.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int64_t decode_offset(uint64_t offset);
ResultInstruction decode_instruction(uint64_t encoded_instr);

#ifdef __cplusplus
} // extern "C"
#endif
#endif
