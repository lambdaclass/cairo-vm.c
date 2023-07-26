#include "relocatable.h"
#include "utils.h"

bool maybe_relocatable_equal(maybe_relocatable *a, maybe_relocatable *b) {
	if (a->is_felt && b->is_felt) {
		return felt_equal(a->value.felt, b->value.felt);
	} else if (!a->is_felt && !b->is_felt) {
		return a->value.relocatable.segment_index == b->value.relocatable.segment_index &&
		       a->value.relocatable.offset == b->value.relocatable.offset;
	} else {
		return false;
	}
}

maybe_relocatable maybe_relocatable_from_felt_limbs(limb_t *felt) {
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = {felt[0], felt[1], felt[2], felt[3]}}};
	return elem;
}

void print_maybe_relocatable(maybe_relocatable *v) {
	if (v->is_felt) {
		limb_t *f = v->value.felt;
		printf("MAYBE_RELOCATABLE:FELT [%llu, %llu, %llu, %llu]\n", f[0], f[1], f[2], f[3]);
	} else {
		relocatable r = v->value.relocatable;
		printf("MAYBE_RELOCATABLE:RELOCATABLE %i:%i\n", r.segment_index, r.offset);
	}
}
