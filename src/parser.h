#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

  typedef struct { uint64_t values[4]; } Felt;

  typedef Felt* data[]; 

  typedef struct {
    Felt *data;
    size_t len;
  } Instructions;


  typedef struct {
    Instructions instructions;
  } Program;

  Program parse(const char *jsonDataStr);

#ifdef __cplusplus
} // extern "C"
#endif