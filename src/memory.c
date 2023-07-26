#include "memory.h"
#include "collectc/cc_array.h"
#include "collectc/cc_hashtable.h"
#include "relocatable.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int key_compare(const void *key1, const void *key2) {
	relocatable r1 = *((relocatable *)key1);
	relocatable r2 = *((relocatable *)key2);
	// printf("KEY CMP: %i:%i vs %i:%i\n", r1.segment_index, r1.offset, r2.segment_index, r2.offset);
	return !(r1.segment_index == r2.segment_index && r1.offset == r2.offset);
}

memory memory_new(void) {
	CC_HashTableConf config;
	cc_hashtable_conf_init(&config);
	config.key_length = sizeof(relocatable);
	config.hash = GENERAL_HASH;
	config.key_compare = key_compare;
	CC_HashTable *data;
	cc_hashtable_new_conf(&config, &data);
	memory mem = {0, data};
	return mem;
}

ResultMemory memory_get(memory *mem, relocatable ptr) {
	maybe_relocatable *value = NULL;
	if (cc_hashtable_get(mem->data, &ptr, (void *)&value) == CC_OK) {
		ResultMemory ok = {.is_error = false, .value = {.memory_value = *value}};
		return ok;
	}
	ResultMemory error = {.is_error = true, .value = {.error = Get}};
	return error;
}

ResultMemory memory_insert(memory *mem, relocatable ptr, maybe_relocatable value) {
	// Guard out of bounds writes
	if (ptr.segment_index >= mem->num_segments) {
		ResultMemory error = {.is_error = true, .value = {.error = Insert}};
		return error;
	}
	// Guard overwrites
	maybe_relocatable *prev_value = NULL;
	if (cc_hashtable_get(mem->data, &ptr, (void *)&prev_value) == CC_OK &&
	    !maybe_relocatable_equal(prev_value, &value)) {
		ResultMemory error = {.is_error = true, .value = {.error = Insert}};
		return error;
	}
	// Write new value
	// Allocate new values
	relocatable *ptr_alloc = malloc(sizeof(relocatable));
	*ptr_alloc = ptr;
	maybe_relocatable *value_alloc = malloc(sizeof(maybe_relocatable));
	*value_alloc = value;
	if (cc_hashtable_add(mem->data, ptr_alloc, value_alloc) == CC_OK) {
		ResultMemory ok = {.is_error = false, .value = {.none = 0}};
		return ok;
	}
	ResultMemory error = {.is_error = true, .value = {.error = Insert}};
	return error;
}

relocatable memory_add_segment(memory *memory) {
	relocatable rel = {memory->num_segments, 0};
	memory->num_segments += 1;
	return rel;
}

ResultMemory memory_load_data(memory *mem, relocatable ptr, CC_Array *data) {
	// Load each value sequentially
	int size = cc_array_size(data);
	for (int i = 0; i < size; i++) {
		// Fetch value from data array
		maybe_relocatable *value = NULL;
		if (cc_array_get_at(data, i, (void *)&value) != CC_OK) {
			ResultMemory error = {.is_error = true, .value = {.error = LoadData}};
			return error;
		}
		// Insert Value
		if (memory_insert(mem, ptr, *value).is_error) {
			ResultMemory error = {.is_error = true, .value = {.error = LoadData}};
			return error;
		}
		// Advance ptr
		ptr.offset += 1;
	}
	ResultMemory ok = {.is_error = false, .value = {.ptr = ptr}};
	return ok;
}

void print_memory(memory *mem) {
	printf("------------------MEMORY------------------\n");
	for (int i = 0; i < (int)mem->num_segments; i++) {
		relocatable ptr = {i, 0};
		while (true) {
			ResultMemory result = memory_get(mem, ptr);
			if (result.is_error) {
				break;
			}
			maybe_relocatable v = result.value.memory_value;
			if (v.is_felt) {
				limb_t *f = v.value.felt;
				printf("%i:%i : [%llu, %llu, %llu, %llu]\n", ptr.segment_index, ptr.offset,
				       (long long unsigned int)f[0], (long long unsigned int)f[1],
				       (long long unsigned int)f[2], (long long unsigned int)f[3]);
			} else {
				relocatable r = v.value.relocatable;
				printf("%i:%i : %i:%i\n", ptr.segment_index, ptr.offset, r.segment_index, r.offset);
			}
			ptr.offset += 1;
		}
	}
	printf("------------------------------------------\n");
}

void memory_free(memory *mem) {
	cc_hashtable_remove_all(mem->data);
	cc_hashtable_destroy(mem->data);
}
