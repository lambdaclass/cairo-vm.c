#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <stdlib.h>
#include "../lib/simdjson.h"
#include "parser.h"
#include <string_view>
#include <sys/stat.h>


using namespace std;

extern "C" {  

  Program* allocate(const char *filename) {
    // Allocate custom Program in memory
    struct stat st;
    stat(filename, &st);
    size_t size = st.st_size;
    return (Program *)malloc(size * sizeof(int));
  }

  void parseArray(const simdjson::dom::array elemArray, char **structArray) {
    int elements = elemArray.size();
    int max_chars = 100;
    structArray = (char **)malloc(elements * max_chars * sizeof(char));
    for(int i = 0; i < elements; ++i) {
      simdjson::dom::element elem = elemArray.at(i).value();
      structArray[i] = (char *)malloc((max_chars + 1) * sizeof(char));
      strcpy(structArray[i], elem.get_c_str());
    }
  }

  void parseString(const char *jsonString, char *structString) {
    int max_chars = 100;
    structString = (char *)malloc((max_chars + 1) * sizeof(char));
    strcpy(structString, jsonString);

  }

  int parseJson(const char *filename, Program *program) {
    // Read the Json data
    simdjson::padded_string json = simdjson::padded_string::load(filename);
    // Parse the JSON data
    simdjson::dom::parser parser;
    simdjson::dom::element root = parser.parse(json);

    // Populate the Program struct
    parseArray(root["attributes"].get_array(), program->attributes);
    parseArray(root["builtins"].get_array(), program->builtins);
    parseString(root["compiler_version"].get_c_str(), program->compiler_version);
    parseArray(root["data"].get_array(), program->data);


    return 0;
  }

  Program *parseFibJson(void) {
    Program *program;
    const char filename[30] = "cairo_programs/fibonacci.json";
    program = allocate(filename);
    parseJson(filename, program);
    return program;
  }
}
