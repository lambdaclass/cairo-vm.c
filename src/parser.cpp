#include "parser.h"
#include "../lib/simdjson.h"
using namespace std;

extern "C" {

void parseData(simdjson::dom::array dataArray, Program *program) {
	int elements = dataArray.size();
	program->data.num_elements = elements;
	program->data.data = (char **)malloc(elements * sizeof(char *));
	for (int i = 0; i < elements; ++i) {
		simdjson::dom::element elem = dataArray.at(i).value();
		size_t str_length = elem.get_string_length();
		program->data.data[i] = (char *)malloc((str_length + 1) * sizeof(char));
		memcpy(program->data.data[i], elem.get_c_str(), str_length);
		program->data.data[i][str_length] = '\0'; // Null-terminate the string
	}
}

Program *parseJson(const char *filename) {
	Program *program = (Program *)malloc(sizeof(Program));
	if (program == NULL) {
		printf("Memory allocation failed. \n");
		free(program);
		exit(EXIT_FAILURE);
	}
	// Read the Json data
	simdjson::padded_string json = simdjson::padded_string::load(filename);
	// Parse the JSON data
	simdjson::dom::parser parser;
	simdjson::dom::element root = parser.parse(json);

	// Populate the Program struct

	simdjson::dom::array dataArray = root["data"].get_array();
	parseData(dataArray, program);
	return program;
}
}
