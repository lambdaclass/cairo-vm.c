#include "parser.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	//printf("Hello World!\n");
	FILE *fptr;
	fptr = fopen("cairo_programs/fibonacci.json", "r");
	char jsonBuffer[32*1024];
	size_t nBytes = fread(jsonBuffer, 1, 32 * 1024, fptr);
	jsonBuffer[nBytes] = 0;
	parse(jsonBuffer);
	return 0;
}