#include "memory.h"
#include <stdio.h>

relocatable memory_add_segment(memory *memory)
{
	relocatable rel = {memory->num_segments, 0};
	struct CList *segment = CList_init(sizeof(CList *));
	printf("%i, %i\n", memory->data, segment);
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
