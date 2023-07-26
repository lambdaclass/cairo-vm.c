#include "fibonacci_test.h"
#include "stdio.h"

// This test does the following things
// Parses fibonacci json into custom program struct
// Initializes a runner with data field in the struct
// Runs cairo run with initialized runner
void fib_test(void) {
	Program program = parse_json_filename("cairo_programs/fibonacci.json");
	cairo_runner runner = runner_new(program);
	relocatable relocatable = runner_initialize(&runner);
	printf("%u", relocatable.offset);
}
