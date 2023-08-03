#include "cairo_run.h"
#include "parser.h"
#include <stdlib.h>

ResultCairoRun cairo_run(char *program_file_name) {
	Program *program = parse_json_filename(program_file_name);
	free(program);
	ResultCairoRun ok = {.is_error = false, .error_message = ""};
	return ok;
}
