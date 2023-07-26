#include "memory.h"
#include "collectc/cc_array.h"
#include "collectc/cc_common.h"
#include "collectc/cc_hashtable.h"
#include "relocatable.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int key_compare(const void *key1, const void *key2) {
	relocatable r1 = *((relocatable *)key1);
	relocatable r2 = *((relocatable *)key2);
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
	if (cc_hashtable_get(mem->data, &ptr, (void *)&prev_value) == CC_OK) {
		if (!maybe_relocatable_equal(prev_value, &value)) {
			ResultMemory error = {.is_error = true, .value = {.error = Insert}};
			return error;
		} else {
			ResultMemory ok = {.is_error = false, .value = {.none = 0}};
			return ok;
		}
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


// TODO: Add a hashmap(int, felt) for relocated memory
void memory_relocate(memory * mem) {
	// Calculate the size of each segment
	int * segment_sizes;
	for (int i = 0; i < mem->num_segments; i++) {
		// We use -1 here as we will then be adding 1 to convert maximun offsets to segment sizes
		segment_sizes[i] = -1;
	}
	// Find the highest offset at each segment
	// Obtain all the addresses in memory
	CC_Array * mem_keys = NULL;
	cc_array_new(&mem_keys);
	CC_ArrayIter * mem_keys_iter = NULL;
	cc_array_iter_init(mem_keys_iter, mem_keys);
	relocatable * key = NULL;
	while (cc_array_iter_next(mem_keys_iter, (void *) &key) != CC_ITER_END) {
		if (key->offset > segment_sizes[key->segment_index]) {
			segment_sizes[key->segment_index] = key->offset;
		}
	}
	// Add 1 to each size 
	for (int i = 0; i < mem->num_segments; i++) {
		segment_sizes[i] += 1;
	}

	// Assign a base to each segment
	int * segment_bases;
	// First segment base is set to 1
	segment_bases[0] = 1;
	for (int i = 1; i < mem->num_segments; i++) {
		segment_bases[i] = segment_bases[i - 1] + segment_sizes[i - 1];
	}

	// Convert all values to felt
	CC_ArrayIter * mem_keys_iter_b = NULL;
	cc_array_iter_init(mem_keys_iter_b, mem_keys);
	relocatable * key_b = NULL;
	while (cc_array_iter_next(mem_keys_iter_b, (void *) &key) != CC_ITER_END) {
		// Relocate Key
		int relocated_key = segment_bases[key_b->segment_index] + key_b->offset;
		// Remove value from 
		maybe_relocatable * value = NULL;
		cc_hashtable_remove(mem->data, key_b, (void *) &value);
		// Relocate value (if needed)
		// TODO copy value so that we dont modify it
		if (!value->is_felt) {
			felt_t felt;
			from(felt, segment_bases[value->value.relocatable.segment_index] + value->value.relocatable.offset);
			*value = maybe_relocatable_from_felt_limbs(felt);
		}
		// Insert relocated key-value pair
		cc_hashtable_add(mem->relocated_data, &relocated_key, &value);


	}

	cc_array_destroy(mem_keys);

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
	CC_HashTableIter iter;
	cc_hashtable_iter_init(&iter, mem->data);

	TableEntry *entry;
	while (cc_hashtable_iter_next(&iter, &entry) != CC_ITER_END) {
		free(entry->value);
		free(entry->key);
	}
	cc_hashtable_remove_all(mem->data);
	cc_hashtable_destroy(mem->data);
}
