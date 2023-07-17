#include "felt_test.h"

void felt_sub(void) {
	felt_t f_one;
	one(f_one);

	felt_t expected;
	felt_t result;
	zero(expected);

	sub(f_one, f_one, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_add(void) {
	felt_t f_one;
	one(f_one);

	felt_t f_zero;
	zero(f_zero);

	felt_t expected;
	one(expected);

	felt_t result;
	add(f_one, f_zero, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_mul1(void) {
	felt_t f_one;
	one(f_one);

	felt_t expected;
	one(expected);

	felt_t result;
	mul(f_one, f_one, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_mul0(void) {
	felt_t f_one;
	one(f_one);

	felt_t f_zero;
	zero(f_zero);

	felt_t expected;
	zero(expected);

	felt_t result;
	mul(f_one, f_zero, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_mul9(void) {
	felt_t f_three;
	from(f_three, 3);

	felt_t expected;
	from(expected, 9);

	felt_t result;
	mul(f_three, f_three, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_div3(void) {
	felt_t f_three;
	from(f_three, 3);

	felt_t expected;
	from(expected, 1);

	felt_t result;
	div(f_three, f_three, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_div4(void) {
	felt_t f_four;
	from(f_four, 4);

	felt_t f_two;
	from(f_two, 2);

	felt_t expected;
	from(expected, 2);

	felt_t result;
	div(f_four, f_two, result);
	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] == result[3]);
}

void felt_div4_err(void) {
	felt_t f_four;
	from(f_four, 4);

	felt_t f_one;
	from(f_one, 1);

	felt_t expected;
	from(expected, 45);

	felt_t result;
	div(f_four, f_one, result);

	assert(expected[0] == result[0]);
	assert(expected[1] == result[1]);
	assert(expected[2] == result[2]);
	assert(expected[3] != result[3]);
}

void felt_tests(void) {
	printf("----------------------\n");
	printf("Test: Substraction\n");
	felt_sub();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Addition\n");
	felt_add();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Multiplication\n");
	felt_mul1();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Multiplication0\n");
	felt_mul0();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Multiplication9\n");
	felt_mul9();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Divition3\n");
	felt_div3();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Divition4\n");
	felt_div4();
	printf("OK!\n");

	printf("----------------------\n");
	printf("Test: Divition error\n");
	felt_div4_err();
	printf("OK!\n");
}
