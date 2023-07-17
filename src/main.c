#include "parser.h"
#include <stdio.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
		Program *program = parseFibJson();
    for (int i = 0; program->attributes[i] != NULL; ++i) {
      printf("Attribute %i: %s\n", i+1, program->attributes[i]);
    }	
    return 0;
}