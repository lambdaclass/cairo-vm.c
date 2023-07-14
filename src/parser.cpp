#include "parser.h"
#include "../lib/simdjson.h"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stddef.h>
#include <string.h>
#include <string>
#include <string_view>

#include <iostream>
using namespace std;


extern "C" {
//   void parseDebugInfo(const simdjson::dom::element& debugInfoElement, DebugInfo* debugInfo) {
//     // Parse file_contents
//     const simdjson::dom::object fileContentsObj = debugInfoElement["file_contents"];
//     parseFileContents(fileContentsObj, debugInfo->file_contents_start)

//     size_t fileContentsCount = fileContentsObj.size();
//     debugInfo->file_contents_start = *new char*[fileContentsCount];
    
//     size_t i = 0;
//     for (auto [key, value] : fileContentsObj) {
//         std::string filename = std::string(key);
//         std::string fileContent = std::string(value);
//         debugInfo->file_contents_start[i] = *strdup(filename.c_str());
//         i++;
//     }

//     // Parse instruction_locations
//     const simdjson::dom::object instructionLocationsObj = debugInfoElement["instruction_locations"];
//     size_t instructionLocationsCount = instructionLocationsObj.size();
//     debugInfo->instruction_location = new InstructionLocation[instructionLocationsCount];

//     i = 0;
//     for (auto [key, value] : instructionLocationsObj) {
//         InstructionLocation* instructionLocation = &debugInfo->instruction_location[i];
//         std::string instructionKey = std::string(key);
//         const simdjson::dom::object instructionObj = value;
//         const simdjson::dom::array accessibleScopesArr = instructionObj["accessible_scopes"];
//         size_t accessibleScopesCount = accessibleScopesArr.size();
//         instructionLocation->accessible_scopes = new char*[accessibleScopesCount];
        
//         size_t j = 0;
//         for (simdjson::dom::element accessibleScopeElement : accessibleScopesArr) {
//             std::string accessibleScope = std::string(accessibleScopeElement);
//             instructionLocation->accessible_scopes[j] = strdup(accessibleScope.c_str());
//             j++;
//         }
        
//         const simdjson::dom::object flowTrackingDataObj = instructionObj["flow_tracking_data"];
//         instructionLocation->flow_tracking_data_group = flowTrackingDataObj["ap_tracking"]["group"].get_int64();
//         instructionLocation->flow_tracking_data_offset = flowTrackingDataObj["ap_tracking"]["offset"].get_int64();
        
//         const simdjson::dom::array hintsArr = instructionObj["hints"];
//         size_t hintsCount = hintsArr.size();
//         instructionLocation->hints = new char*[hintsCount];
        
//         j = 0;
//         for (simdjson::dom::element hintElement : hintsArr) {
//             std::string hint = std::string(hintElement);
//             instructionLocation->hints[j] = strdup(hint.c_str());
//             j++;
//         }
        
//         const simdjson::dom::object instObj = instructionObj["inst"];
//         instructionLocation->inst.end_col = instObj["end_col"].get_int64();
//         instructionLocation->inst.end_line = instObj["end_line"].get_int64();
//         const simdjson::dom::object inputFileObj = instObj["input_file"];
//         instructionLocation->inst.input_file.filename = strdup(inputFileObj["filename"].get_c_str());
//         instructionLocation->inst.start_col = instObj["start_col"].get_int64();
//         instructionLocation->inst.start_line = instObj["start_line"].get_int64();

//         i++;
//     }
// }

  Program parseJson(const char* dataBuffer) {
    Program *program = malloc(simdjson::SIMDJSON_PADDING+nBytes);

    // Read the JSON file
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(dataBuffer);
    auto dataArray = doc.find_field("data").get_array();
    cout << dataArray.count_elements() << endl;   

    return *program;


    // Parse compiler_version
    // program->compiler_version = strdup(doc["compiler_version"].get_c_str());

    // Parse data array 
    // simdjson::dom::array dataArr = doc["data"];
    // int dataSize = dataArr.size();
    // program->data = (char **)malloc(dataSize * sizeof(char *));
    // std::cout << "arriving" << std::endl;

    // for (int i = 0; i < dataSize; i++) {
    //   simdjson::dom::element element = dataArr.at(i);
    //   program -> data[i] = strdup(element.get_c_str());
    // }

    // // Parse attributes array 
    // dataArr = doc["attributes"];
    // dataSize = dataArr.size();
    // program->attributes = (char**)malloc((dataSize + 1) * sizeof(char*));
    // for (int i = 0; i < dataSize; i++) {
    //   simdjson::dom::element element = dataArr.at(i);
    //   program -> attributes[i] = strdup(element.get_c_str());
    // }

    // // Parse the builtins array
    // dataArr = doc["builtins"];
    // dataSize = dataArr.size();
    // program->attributes = (char**)malloc((dataSize + 1) * sizeof(char*));
    // for (int i = 0; i < dataSize; i++) {
    //   simdjson::dom::element element = dataArr.at(i);
    //   program -> builtins[i] = strdup(element.get_c_str());
    // }

    // // Parse debug_info
    // dataArr = doc["debug_info"];
    // dataSize = dataArr.size();
    // program->attributes = (char**)malloc((dataSize + 1) * sizeof(char*));
    // for (int i = 0; i < dataSize; i++) {
    //   simdjson::dom::element element = dataArr.at(i);
    //   program -> builtins[i] = strdup(element.get_c_str());
    // }

    // parseDebugInfo(doc["debug_info"], &program->debug_info);

  }
}
