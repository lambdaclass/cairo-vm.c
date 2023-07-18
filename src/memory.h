#ifndef MEMORY_H
#define MEMORY_H

#include "clist.h"
#include "relocatable.h"
#include <stdbool.h>
// Contains behaviour of memory + memory segment manager
typedef struct memory {
	unsigned int num_segments;
	CList *data;
} memory;

// Error handling

enum MemoryError {
	Get,
	Insert,
	LoadData,
};

union ResultMemoryValue {
	enum MemoryError error;
	maybe_relocatable memory_value;
	relocatable ptr;
	int none;
};

typedef struct ResultMemory {
	bool is_error;
	union ResultMemoryValue value;
} ResultMemory;

memory memory_new(void);

ResultMemory memory_get(memory *mem, relocatable ptr);

ResultMemory memory_insert(memory *mem, relocatable ptr, maybe_relocatable value);

relocatable memory_add_segment(memory *memory);

relocatable memory_load_data(memory *memory, relocatable ptr, CList *data);

#endif
