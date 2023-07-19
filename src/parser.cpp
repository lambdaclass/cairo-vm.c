#include "parser.h"
#include "../lib/simdjson.h"

uint64_t hex_string_to_uint64(const char *hex) {
	uint64_t num;
	sscanf(hex, "%" SCNx64, &num);
	return num;
}

// Helper function to parse the data array within the Program struct
void parse_data(simdjson::dom::array data_array, Program *program) {

	int elements = data_array.size();
	program->data = (felt_t *)malloc(elements * sizeof(felt_t));

	// Loop through the elements in the data array
	for (int i = 0; i < elements; ++i) {
		// Get the element at the current index
		simdjson::dom::element elem = data_array.at(i).value();

		// Get the hexadecimal string representation of the element
		const char *hex = elem.get_c_str().value();

		// Convert the hexadecimal string to a 64-bit unsigned integer
		uint64_t num = hex_string_to_uint64(hex);

		// Convert the unsigned integer to the felt_t array
		felt_t felt;
		from(felt, num);

		// Copy the elements from felt to the program's data array
		for (int j = 0; j < 4; ++j) {
			program->data[i][j] = felt[j];
		}
	}
}

// Function to parse the JSON file and populate the Program struct
Program *parse_json(const char *filename) {
	// Add using namespace inside the parse_json function
	using namespace simdjson;

	Program *program = (Program *)malloc(sizeof(Program));

	// Check if memory allocation was successful
	if (program == NULL) {
		printf("Memory allocation failed. \n");
		free(program);
		exit(EXIT_FAILURE);
	}

	// Read the Json data from the file
	padded_string json = padded_string::load(filename);

	// Parse the JSON data
	dom::parser parser;
	dom::element root = parser.parse(json);

	// Get the "data" array from the JSON
	dom::array data_array = root["data"].get_array();

	// Call the helper function to parse the data array
	parse_data(data_array, program);

	return program;
}
