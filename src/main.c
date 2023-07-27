#include "decoder.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Error: expected the name of a program json file\n");
		return -1;
	}
	char * program_file_name = argv[1];
	printf("Running program: %s\n", program_file_name);

	return 0;
}
