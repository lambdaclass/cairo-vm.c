#include "memory_tests.h"
#include "runner_tests.h"
#include "memory.h"
#include <assert.h>
#include <stdio.h>
#include "decoder_test.h"

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	decoding_tests();
	memory_tests();
	return 0;
}
