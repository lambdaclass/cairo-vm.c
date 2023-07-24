#ifndef PROGRAM_H
#define PROGRAM_H

#include "clist.h"
// Incomplete struct, should be expanded
struct program {
	unsigned int main;
	CList *data;
};

// Frees resources used by program struct
void program_free(struct program *program);

#endif
