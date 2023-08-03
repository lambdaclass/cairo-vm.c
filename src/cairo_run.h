#include <stdbool.h>
typedef struct ResultCairoRun {
	bool is_error;
	char *error_message;
} ResultCairoRun;

ResultCairoRun cairo_run(char *program_file_name);
