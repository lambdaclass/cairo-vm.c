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

// Inner value

union memory_value {
	maybe_relocatable value;
	unsigned int none;
};

typedef struct memory_cell {
	union memory_value memory_value;
	bool is_some;
} memory_cell;

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

void memory_free(memory *mem);

#endif
