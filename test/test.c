#include "decoder_test.h"
#include "felt_test.h"
#include "fibonacci_test.h"
#include "memory_tests.h"
#include "parser_test.h"
#include "runner_tests.h"
#include <assert.h>
#include <stdio.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

	felt_tests();
	decoding_tests();
	memory_tests();
	parsing_tests();
	runner_tests();
	// fib_test();
	return 0;
}
