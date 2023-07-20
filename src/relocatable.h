#ifndef RELOCATABLE_H
#define RELOCATABLE_H

#include <stdbool.h>

typedef struct relocatable {
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

union maybe_relocatable_value {
	struct relocatable relocatable;
	unsigned int felt; // TODO use a felt type
};

typedef struct maybe_relocatable {
	union maybe_relocatable_value value;
	bool is_felt;
} maybe_relocatable;

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b);

#endif
