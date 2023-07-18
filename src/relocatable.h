#ifndef RELOCATABLE_H
#define RELOCATABLE_H

typedef struct relocatable {
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

typedef union maybe_relocatable {
	struct relocatable relocatable;
	unsigned int felt; // TODO use a felt type
} maybe_relocatable;

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	return a.felt == b.felt && a.relocatable.segment_index == b.relocatable.segment_index &&
	       a.relocatable.offset == b.relocatable.offset;
}

#endif
