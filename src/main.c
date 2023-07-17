#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	// Allocate custom Program in memory
	char *filename = "cairo_programs/fibonacci.json";
	struct stat st;
  stat(filename, &st);
  size_t size = st.st_size;
	Program *program = malloc(size * sizeof(int));

	// Parse json file into struct 
	parseJson(filename, program);

	for (int i = 0; program->data[i] != NULL; ++i) {
        printf("Element %d: %s\n", i, program->data[i]);
				free(program->data[i]);
  }
	free(program->data);
	free(program);
  return 0;
}