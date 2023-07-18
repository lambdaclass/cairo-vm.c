#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    Program *program = parseFibJson();
    for (int i = 0; i < program->data.num_elements; ++i) {
        printf("Data %i: %s\n", i + 1, program->data.data[i]);
        free(program->data.data[i]); // Free each individual data element
    }
    free(program->data.data); // Free the data array
    free(program); // Free the main Program struct

    return 0;
}
