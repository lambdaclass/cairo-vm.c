#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    int length;
    char *data;
  } StringArray;

  typedef struct {
    char* filename;
  } InputFile;

  typedef struct {
    int end_col;
    int end_line;
    InputFile input_file;
    int start_col;
    int start_line;
  } Inst;

  typedef struct {
    char** accessible_scopes;
    int flow_tracking_data_group;
    int flow_tracking_data_offset;
    char** hints;
    Inst inst;

  } InstructionLocation;

  typedef struct {
    char* cairo_type;
    int offset;
  } Members;

  typedef struct {
    int pc;
    char* type;
    Members* members;
    int size;
  } Identifiers;

  typedef struct {
    char* start;
  } FileContent;

  typedef struct {
    FileContent fileContent;
    InstructionLocation* instruction_location;
  } DebugInfo;

  typedef struct {
    int num_elements;
    char ** data;
  } Data;

  typedef struct {
    StringArray attributes;
    StringArray builtins;
    char *compiler_version;
    Data data;
    DebugInfo debug_info;
    StringArray hints;
    Identifiers* identifiers;
    StringArray main_scope;
    char* prime;
  } Program;

  Program *parseJson(const char *filename);
#ifdef __cplusplus
} // extern "C"
#endif