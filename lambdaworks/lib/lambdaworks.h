#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/* A single limb (unsigned integer with 64 bits). */
typedef uint64_t limb_t;

/* A 256 bit prime field element (felt), represented as four limbs (integers). */
typedef limb_t felt_t[4];

/* Gets a felt_t representing the "value" number, in montgomery format. */
void from(felt_t result, uint64_t value);

/* Gets a felt_t representing 0 */
void zero(felt_t result);

/* Gets a felt_t representing 1 */
void one(felt_t result);

/* Writes the result variable with the sum of a and b felts. */
void add(felt_t a, felt_t b, felt_t result);

/* Writes the result variable with a - b. */
void sub(felt_t a, felt_t b, felt_t result);

/* Writes the result variable with a * b. */
void mul(felt_t a, felt_t b, felt_t result);

/* Writes the result variable with a / b. */
void lw_div(felt_t a, felt_t b, felt_t result);
