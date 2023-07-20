#ifndef PARSER_H
#define PARSER_H
#include "../lambdaworks/lib/lambdaworks.h"
#include "stddef.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t length;
	char **data;
} StringArray;

typedef struct {
	char *filename;
} InputFile;

typedef struct {
	int end_col;
	int end_line;
	InputFile input_file;
	int start_col;
	int start_line;
} Inst;

typedef struct {
	char **accessible_scopes;
	int flow_tracking_data_group;
	int flow_tracking_data_offset;
	char **hints;
	Inst inst;

} InstructionLocation;

typedef struct {
	char *cairo_type;
	int offset;
} Members;

typedef struct {
	int pc;
	char *type;
	Members *members;
	int size;
} Identifiers;

typedef struct {
	char *start;
} FileContent;

typedef struct {
	FileContent fileContent;
	InstructionLocation *instruction_location;
} DebugInfo;

typedef struct {
	StringArray attributes;
	StringArray builtins;
	char *compiler_version;
	felt_t *data;
	DebugInfo debug_info;
	StringArray hints;
	Identifiers *identifiers;
	StringArray main_scope;
	char *prime;
} Program;
void free_program(Program *program);
uint64_t hex_string_to_uint64(const char *hex);
Program *parse_json_filename(const char *filename);
Program *parse_json_data(const char *parse_json_data);
#ifdef __cplusplus
} // extern "C"
#endif
#endif