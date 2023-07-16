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
	uint16_t val = 34;
	uint8_t *res = u16_to_le_bytes(val);

	printf("[ ");
	for (int j = 0; j < 2; j++)
	{
		printf("%hhu, ", res[j]);
	}
	printf(" ]\n");

	uint16_t encoded = u16_from_le_bytes(res);
	printf("encoded value: %lld", encoded);

	return 0;
}
