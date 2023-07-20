#include "parser_test.h"
#include "../src/parser.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parsing_attributes_test(Program *program) {
	printf("Test: parsing__empty_attributes_array\n");

	char *expected[] = {"attr1", "attr2"};
	size_t num_values = sizeof(expected) / sizeof(expected[0]);
	assert(num_values == program->attributes.length);
	if (num_values == 0) {
		assert(program->attributes.data == NULL);
	}
	for (size_t i = 0; i < num_values; ++i) {
		assert(strcmp(expected[i], program->attributes.data[i]) == 0);
	}
	printf("OK! \n");
}

void parsing_empty_attributes_test(Program *program) {
	printf("Test: parsing__empty_attributes_array\n");
	size_t num_values = 0;
	assert(num_values == program->attributes.length);
	printf("OK! \n");
}

void parsing_compiler_version(Program *program) {
	printf("Test: parsing_compiler_version\n");
	char *expected = "0.10.3";
	assert(strcmp(expected, program->compiler_version) == 0);
	printf("OK! \n");
}

void parsing_data_test(Program *program) {
	printf("Test: parsing_data_array\n");

	char *hex_values[] = {
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
	};

	size_t num_values = sizeof(hex_values) / sizeof(hex_values[0]);

	for (size_t i = 0; i < num_values; ++i) {
		uint64_t num = hex_string_to_uint64(hex_values[i]);
		felt_t expected;
		from(expected, num);
		for (size_t j = 0; j < 4; ++j) {
			assert(expected[j] == program->data[i][j]);
		}
	}
	printf("OK! \n");
}

void parsing_tests() {

	const char *filename = "cairo_programs/fibonacci.json";
	Program *program = parse_json_filename(filename);
	printf("---------------------------------\n");
	parsing_empty_attributes_test(program);
	printf("---------------------------------\n");
	parsing_compiler_version(program);
	printf("---------------------------------\n");
	parsing_data_test(program);
	printf("---------------------------------\n");
	free_program(program);
}
