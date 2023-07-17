#include "parser_test.h"
#include "../src/parser.h"

void parsing_tests() {

  printf("---------------------- \n");
  printf("Test: decoding_data_array \n");

  const char *expected_data[] = {
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
			NULL // Add NULL as the last element to indicate the end of the array
	}; 
  Program *program = parseFibJson();
  assert(program->data == expected_data);

  printf("OK! \n");


}
