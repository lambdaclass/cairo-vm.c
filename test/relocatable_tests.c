#include "relocatable_tests.h"

void sub_relocatable_ok_test(void) {
	printf("Test: Sub relocatables ok test\n");
	relocatable expected = {.segment_index = 10, .offset = 5};
	relocatable rel = {.segment_index = 10, .offset = 10};
	relocatable result = sub_relocatable(rel, 5);
	assert(relocatable_equal(expected, result));
	printf("OK!\n");
}

void add_relocatable_ok_test(void) {
	printf("Test: Add relocatables ok test\n");
	relocatable expected = {.segment_index = 10, .offset = 10};
	relocatable rel = {.segment_index = 10, .offset = 5};
	relocatable result = add_relocatable(rel, 5);
	assert(relocatable_equal(expected, result));
	printf("OK!\n");
}
void relocatable_tests(void) {
	printf("--------------------------------- \n");
	printf("Relocatable tests\n");
	printf("---------------------------------\n");
	sub_relocatable_ok_test();
	printf("---------------------------------\n");
	add_relocatable_ok_test();
}
