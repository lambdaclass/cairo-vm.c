#include "cairo_run.h"
#include <stdio.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Error: expected the name of a program json file\n");
		return -1;
	}
	char * program_file_name = argv[1];
	printf("Running program: %s\n", program_file_name);
	ResultCairoRun result = cairo_run(program_file_name);
	if (result.is_error) {
		printf("Error: %s\n", result.error_message);
	}
	printf("Run succesful\n");
	return 0;
}
