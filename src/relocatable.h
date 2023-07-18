#ifndef RELOCATABLE_H
#define RELOCATABLE_H

#include <stdbool.h>

typedef struct relocatable {
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

typedef union maybe_relocatable {
	struct relocatable relocatable;
	unsigned int felt; // TODO use a felt type
} maybe_relocatable;

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b);

#endif
