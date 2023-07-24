#include "program.h"

void program_free(struct program *program) { cc_array_remove_all_free(program->data); }
