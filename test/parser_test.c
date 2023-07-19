#include "parser_test.h"
#include "../src/parser.h"
#include <stddef.h>
#include <stdlib.h>

void parsing_data_test(Program *program) {
	printf("Test: decoding_data_array \n");

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

	free(program->data);
	free(program);
	printf("OK! \n");
}

void parsing_tests() {

	const char *filename = "cairo_programs/fibonacci.json";
	Program *program = parse_json(filename);

	printf("------- START PARSING TESTS -------------- \n");
	parsing_data_test(program);
	printf("------- END PARSING TESTS ---------------- \n");
}
