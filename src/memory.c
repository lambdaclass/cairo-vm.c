#include "memory.h"
#include "relocatable.h"
#include <stddef.h>
#include <stdlib.h>

memory memory_new(void) {
	CC_Array * data;
	cc_array_new(&data);
	memory mem = {0, data};
	return mem;
}

ResultMemory memory_get(memory *mem, relocatable ptr) {
	if (ptr.segment_index >= cc_array_size(mem->data)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	CC_Array *segment = NULL;
	cc_array_get_at(mem->data, ptr.segment_index, (void *) &segment);
	if (ptr.offset >= cc_array_size(segment)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	memory_cell *cell = NULL;
	cc_array_get_at(segment, ptr.offset, (void *) &cell);
	if (cell->is_some) {
		ResultMemory ok = {.is_error = false, .value = {.memory_value = cell->memory_value.value}};
		return ok;
	}
	ResultMemory error = {.is_error = true, .value = {.error = Get}};
	return error;
}

ResultMemory memory_insert(memory *mem, relocatable ptr, maybe_relocatable value) {
	if (ptr.segment_index >= cc_array_size(mem->data)) {
		ResultMemory error = {.is_error = true, .value = {.error = Insert}};
		return error;
	}
	CC_Array *segment = NULL;
	cc_array_get_at(mem->data, ptr.segment_index, (void *) &segment);
	// Handle gaps
	while (cc_array_size(segment) < ptr.offset) {
		memory_cell none = {.is_some = false, .memory_value = {.none = 0}};
		cc_array_add(segment, &none);
	}
	ResultMemory get_result = memory_get(mem, ptr);
	// Check for possible ovewrites
	if (!get_result.is_error) {
		if (maybe_relocatable_equal(get_result.value.memory_value, value)) {
			ResultMemory ok = {.is_error = false, .value = {.none = 0}};
			return ok;
		} else {
			ResultMemory error = {.is_error = true, .value = {.error = Insert}};
			return error;
		}
	}
	memory_cell new_cell = {.is_some = true, .memory_value = {.value = value}};
	cc_array_add_at(segment, &new_cell, ptr.offset);
	ResultMemory ok = {.is_error = false, .value = {.none = 0}};
	return ok;
}

relocatable memory_add_segment(memory *memory) {
	relocatable rel = {memory->num_segments, 0};
	CC_Array *segment = NULL;
	cc_array_new(&segment);
	cc_array_add(memory->data, segment);
	memory->num_segments += 1;
	return rel;
}

ResultMemory memory_load_data(memory *memory, relocatable ptr, CC_Array *data) {
	if (ptr.segment_index >= memory->num_segments) {
		ResultMemory error = {.is_error = true, .value = {.error = LoadData}};
		return error;
	}
	int size = cc_array_size(data);
	CC_Array *segment = NULL;
	cc_array_get_at(memory->data, ptr.segment_index, (void *) &segment);
	for (int i = 0; i < size; i++) {
		maybe_relocatable *value = NULL;
		cc_array_get_at(data, i, (void *) &value);
		memory_cell new_cell = {.is_some = true, .memory_value = {.value = *value}};
		cc_array_add_at(segment, &new_cell, ptr.offset + i);
	}
	ptr.offset += size;
	ResultMemory ok = {.is_error = false, .value = {.ptr = ptr}};
	return ok;
}

void memory_free(memory *mem) {
	cc_array_remove_all_free(mem->data);
}
