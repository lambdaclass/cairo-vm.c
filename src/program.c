#include "program.h"
#include "collectc/cc_array.h"

void program_free(struct program *program) {
	cc_array_remove_all_free(program->data);
	cc_array_destroy(program->data);
}
