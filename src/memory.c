#include "memory.h"
#include "clist.h"
#include "relocatable.h"

memory memory_new(void) {
	struct CList *mem_data = CList_init(sizeof(struct CList));
	memory mem = {0, mem_data};
	return mem;
}

ResultMemory memory_get(memory *mem, relocatable ptr) {
	int index = ptr.segment_index;
	int offset = ptr.offset;
	if (index >= mem->data->count(mem->data)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	CList *segment = mem->data->at(mem->data, ptr.segment_index);
	if (offset >= segment->count(segment)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	memory_cell *cell = segment->at(segment, ptr.offset);
	if (cell->is_some) {
		ResultMemory ok = {.is_error = false, .value = {.memory_value = cell->memory_value.value}};
		return ok;
	}
	ResultMemory error = {.is_error = true, .value = {.error = Get}};
	return error;
}

ResultMemory memory_insert(memory *mem, relocatable ptr, maybe_relocatable value) {
	int index = ptr.segment_index;
	int offset = ptr.segment_index;
	if (index >= mem->data->count(mem->data)) {
		ResultMemory error = {.is_error = true, .value = {.error = Insert}};
		return error;
	}
	CList *segment = mem->data->at(mem->data, ptr.segment_index);
	// Handle gaps
	while (segment->count(segment) <= offset) {
		memory_cell none = {.is_some = false, .memory_value = {.none = 0}};
		segment->add(segment, &none);
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
	segment->insert(segment, &new_cell, ptr.offset);
	ResultMemory ok = {.is_error = false, .value = {.none = 0}};
	return ok;
}

relocatable memory_add_segment(memory *memory) {
	relocatable rel = {memory->num_segments, 0};
	struct CList *segment = CList_init(sizeof(memory_cell));
	memory->data->add(memory->data, segment);
	memory->num_segments += 1;
	return rel;
}

relocatable memory_load_data(memory *memory, relocatable ptr, CList *data) {
	int size = data->count(data);
	CList *segment = memory->data->at(memory->data, ptr.segment_index);
	for (int i = 0; i < size; i++) {
		maybe_relocatable *value = data->at(data, i);
		memory_cell new_cell = {.is_some = true, .memory_value = {.value = *value}};
		segment->insert(segment, &new_cell, ptr.offset + i);
	}
	ptr.offset += size;
	return ptr;
}

void memory_free(memory *mem) {
	int num_segments = mem->num_segments;
	for (int i = 0; i < num_segments; i++) {
		CList *segment = mem->data->at(mem->data, i);
		segment->clear(segment);
	}
	mem->data->clear(mem->data);
}
