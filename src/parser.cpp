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
    for(int i = 0; i < data.size(); ++i) {
      simdjson::dom::element elem = data.at(i).value();
      program->data[i] = (char *)malloc((max_chars + 1) * sizeof(char));
      strcpy(program->data[i], elem.get_c_str());
      printf("%s\n", program->data[i]);
      free(program->data[i]);
    }
    free(program->data);






    // program->builtins = root["builtins"].get<std::vector<std::string>>();
    // program->compiler_version = root["compiler_version"];
    // program->data = root["data"].get<std::vector<std::string>>();
    // program->hints = root["hints"].get<std::vector<std::string>>();
    // program->main_scope = root["main_scope"];
    // program->prime = root["prime"];

    // // // Output the parsed data (for verification)
    // if (!program || !program->attributes) {
    //     std::cout << "Attributes: (null)" << std::endl;
    // }

    // std::cout << "Attributes: ";
    // for (size_t i = 0; program->attributes[i] != nullptr; ++i) {
    //     std::cout << program->attributes[i] << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Builtins: ";
    // for (const auto& builtin : program.builtins) {
    //     std::cout << builtin << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Compiler Version: " << program.compiler_version << std::endl;


    return 0;
  }
}
