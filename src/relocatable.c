#include "relocatable.h"

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	if (a.is_felt && b.is_felt) {
		return a.value.felt == b.value.felt;
	} else if (!a.is_felt && !b.is_felt) {
		return a.value.relocatable.segment_index == b.value.relocatable.segment_index &&
		       a.value.relocatable.offset == b.value.relocatable.offset;
	} else {
		return false;
	}
}
