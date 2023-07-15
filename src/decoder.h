#ifndef DECO_H
#define DECO_H
#include "instruction.h"
#include "stdint.h"
#include "stdio.h"

ResultInstruction decode_instruction(uint64_t encoded_ins);

#endif