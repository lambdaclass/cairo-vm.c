#include "memory.h"

memory memory_new(void)
{
	struct CList *mem_data = CList_init(10 * sizeof(struct CList *));
	memory mem = {0, mem_data};
	return mem;
}

relocatable memory_add_segment(memory *memory)
{
	relocatable rel = {memory->num_segments, 0};
	struct CList *segment = CList_init(sizeof(CList *));
	memory->data->add(memory->data, segment);
	memory->num_segments += 1;
	return rel;
}

relocatable memory_load_data(memory *memory, relocatable ptr, CList *data)
{
	int size = data->count(data);
	CList *segment = memory->data->at(memory->data, ptr.segment_index);
	for (int i = 0; i < size; i++)
	{
		segment->insert(segment, data->at(data, i), ptr.offset + i);
	}
	ptr.offset += size;
	return ptr;
}
