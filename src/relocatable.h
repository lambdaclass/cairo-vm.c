#ifndef RELOCATABLE_H
#define RELOCATABLE_H

#include "../lambdaworks/lib/lambdaworks.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct relocatable {
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

union maybe_relocatable_value {
	struct relocatable relocatable;
	felt_t felt;
};

typedef struct maybe_relocatable {
	union maybe_relocatable_value value;
	bool is_felt;
} maybe_relocatable;

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b);
relocatable sub_relocatable(relocatable a, uint64_t other);
relocatable add_relocatable(relocatable a, uint64_t other);
maybe_relocatable maybe_relocatable_from_felt_limbs(limb_t *felt);
bool relocatable_equal(relocatable a, relocatable b);

#endif
