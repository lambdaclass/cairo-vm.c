#include <assert.h>
#include <stdio.h>
#include "parser.c"

int main(int argc, char **argv)
{
	// Define the hexadecimal values as strings
	const char *expected[] = {
			"0x40780017fff7fff",
			"0x0",
			"0x1104800180018000",
			"0x4",
			"0x10780017fff7fff",
			"0x0",
			"0x480680017fff8000",
			"0x1",
			"0x480680017fff8000",
			"0x1",
			"0x480680017fff8000",
			"0xa",
			"0x1104800180018000",
			"0x5",
			"0x400680017fff7fff",
			"0x90",
			"0x208b7fff7fff7ffe",
			"0x20780017fff7ffd",
			"0x5",
			"0x480a7ffc7fff8000",
			"0x480a7ffc7fff8000",
			"0x208b7fff7fff7ffe",
			"0x482a7ffc7ffb8000",
			"0x480a7ffc7fff8000",
			"0x48127ffe7fff8000",
			"0x482680017ffd8000",
			"0x800000000000011000000000000000000000000000000000000000000000000",
			"0x1104800180018000",
			"0x800000000000010fffffffffffffffffffffffffffffffffffffffffffffff7",
			"0x208b7fff7fff7ffe",
			NULL // Add NULL as the last element to indicate the end of the array
	};
	Program *program = parse(argc, argv);
	assert(program->data == expected);
	printf("Tests passed!\n");

	return 0;
}
