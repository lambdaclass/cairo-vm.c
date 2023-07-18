#include "decoder_test.h"
#include "felt_test.h"
#include "parser_test.h"
#include <assert.h>
#include <stdio.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

	felt_tests();
	decoding_tests();
	parsing_tests();
	return 0;
}
