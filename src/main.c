#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	//printf("Hello World!\n");
	FILE *fptr = fopen("cairo_programs/fibonacci.json", "r");
	char buf[32*1024];
	size_t nBytes = fread(buf, 1, 32 * 1024, fptr);
	buf[nBytes] = 0;
	parseJson(buf);
	return 0;
}