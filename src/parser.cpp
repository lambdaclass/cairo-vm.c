#include "parser.h"
#include "../lib/simdjson.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>

uint64_t hex_string_to_uint64(const char *hex) {
	uint64_t num;
	sscanf(hex, "%" SCNx64, &num);
	return num;
}

void parse_attributes(simdjson::dom::array attributes_array, Program *program) {
	size_t elements = attributes_array.size();
	if (elements == 0) {
		program->attributes.length = 0;
		program->attributes.data = nullptr;
		return;
	}

	program->attributes.length = elements;
	program->attributes.data = (char **)malloc(elements * sizeof(char *));
	if (program->attributes.data == NULL) {
		printf("Memory allocation failed\n");
		program = nullptr;
	} else {
		for (size_t i = 0; i < elements; ++i) {
			simdjson::dom::element elem = attributes_array.at(i).value();
			program->attributes.data[i] = (char *)malloc(elem.get_string_length() + 1);
			if (program->attributes.data[i] == NULL) {
				printf("Memory allocation failed\n");
				program = nullptr;
			}
			strcpy(program->attributes.data[i], elem.get_c_str().value());
		}
	}
}

// Helper function to parse the data array within the Program struct
void parse_data(simdjson::dom::array data_array, Program *program) {

	size_t elements = data_array.size();
	program->data = (felt_t *)malloc(elements * sizeof(felt_t));

	// Loop through the elements in the data array
	for (size_t i = 0; i < elements; ++i) {
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

void parse_file_contents(simdjson::dom::element file_contents, Program *program) {
	const char *start = file_contents["<start>"].get_c_str().value();
	program->debug_info.fileContent.start = (char *)malloc(strlen(start) * sizeof(char *));
	strcpy(program->debug_info.fileContent.start, start);
}

void parse_instruction_location(simdjson::dom::element instruction_locations, Program *program) {
	for (auto [k, v] : instruction_locations.get_object()) {
		const char *key = k.data();
		printf("%s, %s\n", key, v.get_c_str().value());
	}

	program = program;
}

void parse_debug_info(simdjson::dom::element debug_info, Program *program) {
	try {
		simdjson::dom::element file_contents = debug_info["file_contents"];
		parse_file_contents(file_contents, program);
		simdjson::dom::element instruction_locations = debug_info["instruction_locations"];
		parse_instruction_location(instruction_locations, program);
	} catch (simdjson::simdjson_error *se) {
		printf("error %s", se->what());
	}
}

void free_program(Program *program) {
	if (program != nullptr) {
		if (program->attributes.length > 0) {
			for (size_t i = 0; i < program->attributes.length; ++i) {
				free(program->attributes.data[i]);
			}
			free(program->attributes.data);
		}
		free(program->compiler_version);
		free(program->data);
		free(program->debug_info.fileContent.start);
		free(program);
	}
}

// Function to parse the JSON file and populate the Program struct
Program *parse_json_filename(const char *filename) {
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

	// Parse attributes array
	dom::array attributes_array = root["attributes"].get_array();
	parse_attributes(attributes_array, program);

	// Parse compiler version
	const char *compiler_version = root["compiler_version"].get_c_str().value();
	program->compiler_version = (char *)malloc(strlen(compiler_version) * sizeof(char *));
	strcpy(program->compiler_version, compiler_version);

	// Parse data array
	dom::array data_array = root["data"].get_array();
	parse_data(data_array, program);

	// Parse debug info
	dom::element debug_info = root["debug_info"];
	parse_debug_info(debug_info, program);

	return program;
}
