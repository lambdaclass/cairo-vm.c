#include "../src/parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>



Program *allocate(const char *filename) {
    // Allocate custom Program in memory
    struct stat st;
    stat(filename, &st);
    size_t size = st.st_size;
    Program *program = malloc(size * sizeof(int));
    return program;
  }

Program *parse(int argc, char **argv)
{

	char *filename = "cairo_programs/fibonacci.json";
  Program *program = allocate(filename);
	parseJson(filename, program);

	// for (int i = 0; program->data[i] != NULL; ++i) {
  //       printf("Element %d: %s\n", i, program->data[i]);
	// 			// free(program->data[i]);
  // }

	// free(program->data);
	// free(program);
  return program;
}
