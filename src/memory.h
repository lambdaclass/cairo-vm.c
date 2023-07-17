#ifndef MEMORY_H
#define MEMORY_H

#include "clist.h"
#include "relocatable.h"
// Contains behaviour of memory + memory segment manager
typedef struct memory
{
	unsigned int num_segments;
	CList *data;
} memory;

relocatable memory_add_segment(memory *memory);

relocatable memory_load_data(memory *memory, relocatable ptr, CList *data);

#endif
