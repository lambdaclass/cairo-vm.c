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
	var program_base memory.Relocatable
	var execution_base memory.Relocatable
	var initial_ap memory.Relocatable
	var initial_fp memory.Relocatable
	var initial_pc memory.Relocatable
	var final_pc memory.Relocatable

	return &CairoRunner {
		program: program,
		program_base: program_base,
		execution_base: execution_base,
		initial_ap: initial_ap,
		initial_fp: initial_fp,
		initial_pc: initial_pc,
		final_pc: final_pc,
	}
}