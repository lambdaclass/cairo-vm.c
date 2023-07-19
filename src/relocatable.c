#include "relocatable.h"

bool felt_equal(felt_t *a, felt_t *b) { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]; }

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	return felt_equal(a.felt, b.felt) && a.relocatable.segment_index == b.relocatable.segment_index &&
	       a.relocatable.offset == b.relocatable.offset;
}
