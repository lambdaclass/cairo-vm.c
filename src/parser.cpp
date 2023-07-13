#include "parser.h"
#include "../lib/simdjson.h"
#include <iostream>
#include <ostream>
#include <stddef.h>
#include <string.h>


extern "C" Program parse(const char *jsonDataStr) {
  for (int i = 0; jsonDataStr[i] != 0; i++) {
    std::cout << jsonDataStr[i];
  }
  std::cout << std::endl;

  simdjson::padded_string myPaddedJson((std::string(jsonDataStr)));
  

  return Program();
}
