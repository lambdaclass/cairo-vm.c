package vm

import "github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"

type CairoRunner struct {
	program        Program
	program_base   memory.Relocatable
	execution_base memory.Relocatable
	initial_ap     memory.Relocatable
	initial_fp     memory.Relocatable
	initial_pc     memory.Relocatable
	final_pc       memory.Relocatable
}

// Creates a new instance of a CairoRunner.
func NewCairoRunner(program Program) *CairoRunner {
	runner := CairoRunner{program: program}
	return &runner
}
