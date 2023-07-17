#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	//printf("Hello World!\n");
	char *filename = "cairo_programs/fibonacci.json";
	struct stat st;
  stat(filename, &st);
  size_t size = st.st_size;
	Program *program = malloc(size);
	
	parseJson(filename, program);
    return 0;
}