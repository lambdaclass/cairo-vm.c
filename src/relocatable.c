#include "relocatable.h"

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	return a.felt == b.felt && a.relocatable.segment_index == b.relocatable.segment_index &&
	       a.relocatable.offset == b.relocatable.offset;
}
