#include "memory_tests.h"
#include "runner_tests.h"
#include "memory.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	//runner_tests();
	memory_tests();
	printf("Tests passed!\n");

	return 0;
}
