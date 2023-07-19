#ifndef RELOCATABLE_H
#define RELOCATABLE_H

#include "../lambdaworks/lib/lambdaworks.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct relocatable {
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

typedef union maybe_relocatable {
	struct relocatable relocatable;
	felt_t *felt;
} maybe_relocatable;

bool felt_equal(felt_t *a, felt_t *b);
bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b);

#endif
