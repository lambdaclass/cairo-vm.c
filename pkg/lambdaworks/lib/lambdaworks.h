#include <stdint.h>

typedef uint64_t limb_t;
typedef limb_t felt_t[4];

int number();
void from(felt_t result, uint64_t value);
void zero(felt_t result);
void one(felt_t result);
void add(felt_t a, felt_t b, felt_t result);
void sub(felt_t a, felt_t b, felt_t result);
void mul(felt_t a, felt_t b, felt_t result);
void div(felt_t a, felt_t b, felt_t result);
