package parser

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
)

type FlowTrackingData struct {
	AP_TRACKING   map[string]int `json:"ap_tracking"`
	REFERENCE_IDS map[string]int `json:"reference_ids"`
}

type Instructions struct {
	END_COL    int               `json:"end_col"`
	END_LINE   int               `json:"end_line"`
	INPUT_FILE map[string]string `json:"input_file"`
	START_COL  int               `json:"start_col"`
	START_LINE int               `json:"start_line"`
}

type InstructionLocation struct {
	ACCESSIBLE_SCOPES  []string         `json:"accessible_scopes"`
	FLOW_TRACKING_DATA FlowTrackingData `json:"flow_tracking_data"`
	HINTS              []string         `json:"hints"`
	INST               Instructions     `json:"inst"`
}

type DebugInfo struct {
	FILE_CONTENTS        map[string]string              `json:"file_contents"`
	INSTRUCTION_LOCATION map[string]InstructionLocation `json:"instruction_locations"`
}

type Identifier struct {
	FULL_NAME  string         `json:"full_name"`
	MEMBERS    map[string]any `json:"members"`
	SIZE       int            `json:"size"`
	DECORATORS []string       `json:"decorators"`
	PC         int            `json:"pc"`
	TYPE       string         `json:"type"`
	CAIRO_TYPE string         `json:"cairo_type"`
	VALUE      int            `json:"value"`
}

type ApTrackingData struct {
	GROUP  int `json:"group"`
	OFFSET int `json:"offset"`
}

type Reference struct {
	AP_TRACKING_DATA ApTrackingData `json:"ap_tracking_data"`
	PC               int            `json:"pc"`
	VALUE            string         `json:"value"`
}

type ReferenceManager struct {
	REFERENCES []Reference `json:"references"`
}

type CompiledJson struct {
	ATTRIBUTES        []string              `json:"attributes"`
	BUILTINS          []string              `json:"builtins"`
	COMPILER_VERSION  string                `json:"compiler_version"`
	DATA              []string              `json:"data"`
	DEBUG_INFO        DebugInfo             `json:"debug_info"`
	HINTS             map[string]string     `json:"hints"`
	IDENTIFIERS       map[string]Identifier `json:"identifiers"`
	MAIN_SCOPE        string                `json:"main_scope"`
	PRIME             string                `json:"prime"`
	REFERENCE_MANAGER ReferenceManager      `json:"reference_manager"`
}

func Parse(jsonPath string) CompiledJson {
	jsonFile, err := os.Open(jsonPath)

	if err != nil {
		fmt.Println(err)
	}
	defer jsonFile.Close()

	var cJson CompiledJson

	byteValue, _ := ioutil.ReadAll(jsonFile)
	err = json.Unmarshal(byteValue, &cJson)

	if err != nil {
		fmt.Println(err)
	}

	return cJson

}
