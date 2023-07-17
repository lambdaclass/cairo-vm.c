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


using namespace std;

extern "C" {

  

  int parseJson(const char *filename, Program *program) {
    
    // Read the Json data
    simdjson::padded_string json = simdjson::padded_string::load(filename);
    // Parse the JSON data
    simdjson::dom::parser parser;
    simdjson::dom::element root = parser.parse(json);

    // Populate the Program->Data struct
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
    return 0;


  }
}
