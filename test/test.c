#include "decoder_test.h"
#include "felt_test.h"
#include "memory.h"
#include "memory_tests.h"
#include "runner_tests.h"
#include <assert.h>
#include <stdio.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	felt_tests();
	decoding_tests();
	memory_tests();
	return 0;
}
