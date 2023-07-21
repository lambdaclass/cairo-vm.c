#include "relocatable.h"

bool felt_equal(felt_t *a, felt_t *b) { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]; }

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	// both are felts
	if (a.is_felt && b.is_felt)
		return felt_equal(a.value.felt, b.value.felt);
	// one is a relocatable and the other a pointer
	else if (!a.is_felt || !b.is_felt)
		return false;
	else
		return a.value.relocatable.segment_index == b.value.relocatable.segment_index &&
		       a.value.relocatable.offset == b.value.relocatable.offset;
}
