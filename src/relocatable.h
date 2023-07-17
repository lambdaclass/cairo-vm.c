#ifndef RELOCATABLE_H
#define RELOCATABLE_H

typedef struct relocatable
{
	unsigned int segment_index;
	unsigned int offset;
} relocatable;

typedef union maybe_relocatable
{
	struct relocatable relocatable;
	unsigned int felt; // TODO use a felt type
} maybe_relocatable;

#endif
