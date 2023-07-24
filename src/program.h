#ifndef PROGRAM_H
#define PROGRAM_H

#include <collectc/cc_array.h>
// Incomplete struct, should be expanded
struct program {
	unsigned int main;
	CC_Array *data;
};

// Frees resources used by program struct
void program_free(struct program *program);

#endif
