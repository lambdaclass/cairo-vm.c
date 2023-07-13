package vm

import (
	"errors"

	"github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"
)

type CairoRunner struct {
	program       Program
	programBase   memory.Relocatable
	executionBase memory.Relocatable
	initialAp     uint
	initialFp     uint
	initialPc     memory.Relocatable
	finalPc       memory.Relocatable
	entrypoint    uint
}

// Creates a new instance of a CairoRunner.
func NewCairoRunner(program Program) (*CairoRunner, error) {
	runner := CairoRunner{program: program}
	return &runner, nil
}

func (cr *CairoRunner) InitializeFunctionEntrypoint(vm *VirtualMachine, entrypoint uint, stack []memory.MaybeRelocatable, returnFp memory.MaybeRelocatable) (*memory.Relocatable, error) {
	finalPc := *vm.segments.Add()
	stackExtension := []memory.MaybeRelocatable{returnFp, *memory.NewMaybeRelocatableAddr(finalPc)}

	stack = append(stack, stackExtension...)

	var relocatableNil memory.Relocatable
	if cr.executionBase == relocatableNil {
		return nil, errors.New("No execution base")
	}

	cr.initialFp = cr.executionBase.Offset + uint(len(stack))
	cr.finalPc = finalPc

	// err := cr.InitializeState(vm, entrypoint, stack)
	// if err != nil {
	// 	return nil, errors.New("Error initializing VM state: %s", err)
	// }

	return &finalPc, nil
}

func (cr *CairoRunner) InitializeMainEntrypoint(vm *VirtualMachine) (*memory.Relocatable, error) {
	if &cr.entrypoint == nil {
		return nil, errors.New("Missing main() entrypoint")
	}
	// stack := []memory.MaybeRelocatable{}

	return nil, nil
}

func (cr *CairoRunner) InitializeVM(vm *VirtualMachine) error {
	// FIXME: This `nil` error handling does not sound too good, but leaving
	// it like this for now
	var relocatableNil memory.Relocatable
	var uintNil uint
	if cr.initialPc == relocatableNil || cr.initialAp == uintNil || cr.initialFp == uintNil {
		return errors.New("VM initialization error - Could not set initial state")
	}
	vm.runContext.pc = cr.initialPc
	vm.runContext.ap = cr.initialAp
	vm.runContext.fp = cr.initialFp

	// TODO: Add builtins validation rules and validate memory
	// See `initialize_vm` method in cairo-vm.

	return nil
}
