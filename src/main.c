#include "decoder.h"
#include <limits.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Hello World!\n");
	ResultInstruction res = decode_instruction(1);
	if (res.is_error)
	{
		printf("ERROR\n");
	}

	return 0;
}
