#include "decoder.h"
#include "utils.h"
#include <limits.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	// printf("Hello World!\n");
	//  ResultInstruction res = decode_instruction(1);
	//   if (res.is_error)
	//   {
	//   	printf("ERROR\n");
	//   }
	uint64_t val = 1;
	uint8_t buffer_bytes[8];
	uint8_t *res = u64_to_be_bytes(val, buffer_bytes);
	uint64_t encoded = u64_from_be_bytes(res);

	return 0;
}
