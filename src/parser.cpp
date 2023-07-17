#include "parser.h"
#include "../lib/simdjson.h"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <string_view>

#include <iostream>
using namespace std;



// Function to convert from simdjson::dom::array to char ** 
char ** simdjsonArrayToCharPtrArray(const simdjson::dom::array )


extern "C" {

  Program parseJson(const char* filename, Program *program) {

    // Read the JSON file
    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string::load(filename);
    simdjson::ondemand::document doc = parser.iterate(json);
    simdjson::ondemand::array data = doc.find_field("data");

    // Convert the simdjson::array to char** vector
    data.
    std::vector<char*> char_ptr_vector = simdjsonArrayToCharPtrVector(data);


    program->data = char_ptr_vector;
    

    return *program;
  }
}
