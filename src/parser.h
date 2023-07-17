#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

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
    char **attributes;
    char **builtins;
    char *compiler_version;
    char **data;
    DebugInfo debug_info;
    char** hints;
    Identifiers* identifiers;
    char* main_scope;
    char* prime;
  } Program;

  Program parseJson(const char* filname, Program *program);

  

#ifdef __cplusplus
} // extern "C"
#endif