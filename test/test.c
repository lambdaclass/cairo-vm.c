#include "decoder_test.h"
#include "felt_test.h"
#include "instruction_test.h"
#include "memory_tests.h"
#include "parser_test.h"
#include "relocatable_tests.h"
#include "run_context_tests.h"
#include "runner_tests.h"
#include "vm_test.h"
#include <assert.h>
#include <stdio.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	felt_tests();
	decoding_tests();
	instruction_tests();
	memory_tests();
	runner_tests();
	parsing_tests();
	relocatable_tests();
	run_context_test();
	vm_tests();
	return 0;
}
