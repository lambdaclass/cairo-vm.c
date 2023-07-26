#pragma once
#include "../lambdaworks/lib/lambdaworks.h"
#include "relocatable.h"
#include "clist.h"
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
	CList *data;
	DebugInfo debug_info;
	StringArray hints;
	Identifiers *identifiers;
	StringArray main_scope;
	char *prime;
	int main;
} Program;

void program_free(Program *program);
uint64_t hex_string_to_uint64(const char *hex);
Program get_empty_program(void);
Program parse_json_filename(const char *filename);
#ifdef __cplusplus
} // extern "C"
#endif
