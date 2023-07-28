#include "relocatable.h"
#include "utils.h"

// --------------------------------
// Sign operations for relocatable
// --------------------------------

// TODO: fix this to thrown an error if off < other
relocatable sub_relocatable(relocatable a, uint64_t other) {
	unsigned int offset = a.offset;
	unsigned int new_offset = offset - other;
	relocatable result = {.segment_index = a.segment_index, .offset = new_offset};

	return result;
}

uint64_t checked_add(uint64_t a, uint64_t b) {
	uint64_t res = a + b;
	if (res <= a || res <= b)
		return 0;
	else
		return res;
}

// TODO: fix this to thrown an error. if add overflows
relocatable add_relocatable(relocatable a, uint64_t other) {
	unsigned int off = a.offset;
	uint64_t check_sum = checked_add(off, other);
	relocatable res = {.segment_index = a.segment_index, .offset = check_sum};
	return res;
}

// -----------------------------------
//  operations for maybe_relocatable
// -----------------------------------

bool maybe_relocatable_equal(maybe_relocatable a, maybe_relocatable b) {
	if (a.is_felt && b.is_felt) {
		return felt_equal(a.value.felt, b.value.felt);
	} else if (!a.is_felt || !b.is_felt)
		return false;
	else {
		return relocatable_equal(a.value.relocatable, b.value.relocatable);
	}
}

bool relocatable_equal(relocatable a, relocatable b) {
	return (a.segment_index == b.segment_index && a.offset == b.offset);
}

maybe_relocatable maybe_relocatable_from_felt_limbs(limb_t *felt) {
	maybe_relocatable elem = {.is_felt = true, .value = {.felt = {felt[0], felt[1], felt[2], felt[3]}}};
	return elem;
}
