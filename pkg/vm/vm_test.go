package vm_test

import (
	"testing"

	"github.com/lambdaclass/cairo-vm.go/pkg/parser"
	"github.com/lambdaclass/cairo-vm.go/pkg/vm"
)

// Things we are skipping for now:
// - Initializing hint_executor and passing it to `cairo_run`
// - cairo_run_config stuff
// - Asserting expected
// - Asserting memory_holes
func TestFibonacci(t *testing.T) {
	compiledProgram := parser.Parse("../../cairo_programs/fibonacci.json")

	err := vm.CairoRun(compiledProgram.DATA)
	if err != nil {
		t.Errorf("Program execution failed with error: %s", err)
	}
}
