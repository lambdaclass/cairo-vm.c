#include "program.h"

void program_free(struct program *program) { program->data->free(program->data); }
