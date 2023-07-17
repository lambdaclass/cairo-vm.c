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
#include <sys/stat.h>


using namespace std;

extern "C" {  

  void allocate(const char *filename, Program *program) {
    // Allocate custom Program in memory
    struct stat st;
    stat(filename, &st);
    size_t size = st.st_size;
    program = (Program *)malloc(size * sizeof(int));
  }

  void parseData(const simdjson::dom::element root, Program *program) {
    simdjson::simdjson_result<simdjson::dom::array> data= root["data"].get_array();
    int elements = data.size();
    int max_chars = 100;
    program->data = (char **)malloc(elements * max_chars * sizeof(char));


    for(int i = 0; i < elements; ++i) {
      simdjson::dom::element elem = data.at(i).value();
      program->data[i] = (char *)malloc((max_chars + 1) * sizeof(char));
      strcpy(program->data[i], elem.get_c_str());
    }
    program->data[elements] = NULL;
  }

  int parseJson(const char *filename, Program *program) {
    // Read the Json data
    simdjson::padded_string json = simdjson::padded_string::load(filename);
    // Parse the JSON data
    simdjson::dom::parser parser;
    simdjson::dom::element root = parser.parse(json);

    // Populate the Program->Data struct
    parseData(root, program);
    return 0;
  }

  Program *parseFibJson()
  {
    Program *program;
    char *filename = "cairo_programs/fibonacci.json";
    allocate(filename, program);
    parseJson(filename, program);
    return program;
  }
}
