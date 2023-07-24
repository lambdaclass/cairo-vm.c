#ifndef MEMORY_H
#define MEMORY_H

#include "Collections-C/src/include/cc_array.h"
#include "relocatable.h"
#include <stdbool.h>

// Contains behaviour of memory + memory segment manager
// Memory is inmutable
typedef struct memory {
	unsigned int num_segments;
	CC_Array *data;
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

// Functions

// Creates a new empty memory
memory memory_new(void);

// Fetches a value from memory indicated by ptr, returns a maybe_relocatable
ResultMemory memory_get(memory *mem, relocatable ptr);

// Inserts value into memory at address ptr, returns none
ResultMemory memory_insert(memory *mem, relocatable ptr, maybe_relocatable value);

// Adds a memory segment
relocatable memory_add_segment(memory *memory);

// Inserts the elements in data sequentially from address ptr and returns the next address after the data
// returns ptr
ResultMemory memory_load_data(memory *memory, relocatable ptr, CC_Array *data);

// Frees resources used by the memory struct
void memory_free(memory *mem);

#endif
