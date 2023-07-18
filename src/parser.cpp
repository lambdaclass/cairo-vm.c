#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/simdjson.h"
#include "parser.h"
#include <string_view>
#include <sys/stat.h>


using namespace std;

extern "C" {  

  Program* allocate_program(const char *filename) {
    // Allocate custom Program in memory
    struct stat st;
    stat(filename, &st);
    size_t size = st.st_size;
    return (Program *)malloc(size * sizeof(int));
  }

  // void parseArray(const simdjson::dom::array elemArray, char **structArray) {
  //   int elements = elemArray.size();
  //   structArray = (char **)malloc(elements * sizeof(char *));
  //   for(int i = 0; i < elements; ++i) {
  //     simdjson::dom::element elem = elemArray.at(i).value();
  //     structArray[i] = (char *)malloc(elem.get_string().value() * sizeof(char *));
  //     strcpy(structArray[i], elem.get_c_str());
  //   }
  // }

  // void parseString(const char *jsonString, char *structString) {
  //   int max_chars = 100;
  //   structString = (char *)malloc((max_chars + 1) * sizeof(char));
  //   strcpy(structString, jsonString);

  // }

  int parseJson(const char* filename, Program* program) {
    // Read the Json data
    simdjson::padded_string json = simdjson::padded_string::load(filename);
    // Parse the JSON data
    simdjson::dom::parser parser;
    simdjson::dom::element root = parser.parse(json);

    // Populate the Program struct
    simdjson::dom::array dataArray = root["data"].get_array();
    int elements = dataArray.size();

    program->data.num_elements = elements;
    program->data.data = (char**)malloc(elements * sizeof(char*));

    for (int i = 0; i < elements; ++i) {
        simdjson::dom::element elem = dataArray.at(i).value();
        size_t str_length = elem.get_string_length();
        program->data.data[i] = (char*)malloc((str_length + 1) * sizeof(char));
        memcpy(program->data.data[i], elem.get_c_str(), str_length);
        program->data.data[i][str_length] = '\0'; // Null-terminate the string
    }

    return 0;
}


  Program *parseFibJson() {
    
    Program *program = (Program *)malloc(sizeof(Program));
    if (program == NULL) {
      printf("Memory allocation failed. \n");
      exit(EXIT_FAILURE);
    }

    const char filename[30] = "cairo_programs/fibonacci.json";
    parseJson(filename, program);
    return program;
  }
}
