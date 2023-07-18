#include "memory.h"
#include "clist.h"
#include "relocatable.h"

memory memory_new(void) {
	struct CList *mem_data = CList_init(sizeof(struct CList));
	memory mem = {0, mem_data};
	return mem;
}

ResultMemory memory_get(memory *mem, relocatable ptr) {
	if (ptr.segment_index > mem->data->count(mem->data)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	CList *segment = mem->data->at(mem->data, ptr.segment_index);
	if (ptr.offset > segment->count(segment)) {
		ResultMemory error = {.is_error = true, .value = {.error = Get}};
		return error;
	}
	maybe_relocatable *value = segment->at(segment, ptr.offset);
	ResultMemory ok = {.is_error = false, .value = {.memory_value = *value}};
	return ok;
}

relocatable memory_add_segment(memory *memory) {
	relocatable rel = {memory->num_segments, 0};
	struct CList *segment = CList_init(sizeof(maybe_relocatable));
	memory->data->add(memory->data, segment);
	memory->num_segments += 1;
	return rel;
}

relocatable memory_load_data(memory *memory, relocatable ptr, CList *data) {
	int size = data->count(data);
	CList *segment = memory->data->at(memory->data, ptr.segment_index);
	for (int i = 0; i < size; i++) {
		segment->insert(segment, data->at(data, i), ptr.offset + i);
	}
	ptr.offset += size;
	return ptr;
}
