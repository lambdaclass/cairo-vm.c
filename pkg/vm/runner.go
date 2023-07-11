package vm

import (
	"errors"
	"github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"
)

type CairoRunner struct {
	program        Program
	program_base   memory.Relocatable
	execution_base memory.Relocatable
	initial_ap     uint
	initial_fp     uint
	initial_pc     memory.Relocatable
	final_pc       memory.Relocatable
	entrypoint     uint
}

// Creates a new instance of a CairoRunner.
func NewCairoRunner(program Program) *CairoRunner {
	runner := CairoRunner{program: program}
	return &runner
}

func (cr *CairoRunner) InitializeMainEntrypoint(vm *VirtualMachine) (*memory.Relocatable, error) {

	if &cr.entrypoint == nil {
		return nil, errors.New("Missing main() entrypoint")
	}

	return nil, nil
}

func (cr *CairoRunner) InitializeVM(vm *VirtualMachine) error {
	// FIXME: This `nil` error handling does not sound too good, but leaving
	// it like this for now
	var relocatableNil memory.Relocatable
	var uintNil uint
	if cr.initial_pc == relocatableNil || cr.initial_ap == uintNil || cr.initial_fp == uintNil {
		return errors.New("VM initialization error - Could not set initial state")
	}
	vm.run_context.pc = cr.initial_pc
	vm.run_context.ap = cr.initial_ap
	vm.run_context.fp = cr.initial_fp

	// TODO: Add builtins validation rules and validate memory
	// See `initialize_vm` method in cairo-vm.

	return nil
}
