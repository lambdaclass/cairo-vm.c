#include "program.h"

void program_free_aux(struct program *program) { program->data->free(program->data); }
