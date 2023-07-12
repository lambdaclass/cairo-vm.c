package vm

import (
	"errors"
	"fmt"

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
func NewCairoRunner(program Program) *CairoRunner {
	runner := CairoRunner{program: program}
	return &runner
}

func (cr *CairoRunner) Initialize(vm *VirtualMachine) (*memory.Relocatable, error) {
	// TODO: Initialize builtins
	// err = cr.initialize_builtins(vm)
	cr.InitializeSegments(vm, nil)
	endPc, err := cr.InitializeMainEntrypoint(vm)
	if err != nil {
		fmt.Errorf("could not initialize Cairo runner, got error: %s", err)
	}
	err = cr.InitializeVM(vm)
	if err != nil {
		fmt.Errorf("could not initialize Cairo runner, got error: %s", err)
	}

	return endPc, nil
}

func (cr *CairoRunner) InitializeSegments(vm *VirtualMachine, programBase *memory.Relocatable) {
	if *programBase == (memory.Relocatable{}) {
		cr.programBase = *vm.segments.Add()
	}

	cr.executionBase = *vm.segments.Add()
	// TODO: Initialize builtins segments.
}

func (cr *CairoRunner) InitializeState(vm *VirtualMachine, entrypoint uint, stack []memory.MaybeRelocatable) error {
	if cr.programBase == (memory.Relocatable{}) {
		return errors.New("state initialization error - Could not set program base")

	}

	if cr.executionBase == (memory.Relocatable{}) {
		return errors.New("state initialization error - Could not set execution base")

	}

	programBaseSegmentIndex := cr.programBase.SegmentIndex
	initialPcOffset := cr.programBase.Offset + entrypoint

	initialPc := memory.NewRelocatable(programBaseSegmentIndex, initialPcOffset)
	cr.initialPc = *initialPc

	data := memory.SliceUintToMaybeRelocatable(cr.program.Data)

	_, err := vm.segments.LoadData(cr.programBase, &data)
	if err != nil {
		return errors.New("state initialization error - Could not load program data")
	}

	// TODO: Mark program segment addresses as accessed.

	_, err = vm.segments.LoadData(cr.executionBase, &stack)
	if err != nil {
		return errors.New("state initialization error - Could not load initial stack")
	}

	return nil
}

func (cr *CairoRunner) InitializeFunctionEntrypoint(vm *VirtualMachine, entrypoint uint, stack []memory.MaybeRelocatable, returnFp memory.MaybeRelocatable) (*memory.Relocatable, error) {
	finalPc := *vm.segments.Add()
	stackExtension := []memory.MaybeRelocatable{returnFp, *memory.NewMaybeRelocatableAddr(finalPc)}

	stack = append(stack, stackExtension...)

	var relocatableNil memory.Relocatable
	if cr.executionBase == relocatableNil {
		return nil, errors.New("no execution base")
	}

	cr.initialFp = cr.executionBase.Offset + uint(len(stack))
	cr.finalPc = finalPc

	err := cr.InitializeState(vm, entrypoint, stack)
	if err != nil {
		return nil, fmt.Errorf("error initializing VM state: %s", err)
	}

	return &finalPc, nil
}

func (cr *CairoRunner) InitializeMainEntrypoint(vm *VirtualMachine) (*memory.Relocatable, error) {
	// Here we really want to check that `entrypoint` is not set to nil
	// Maybe we could ommit this check, because in principle 0 could be a valid value.
	if cr.entrypoint == 0 {
		return nil, errors.New("missing main() entrypoint")
	}
	stack := []memory.MaybeRelocatable{}
	returnFp := memory.NewMaybeRelocatableAddr(*vm.segments.Add())
	pc, err := cr.InitializeFunctionEntrypoint(vm, cr.entrypoint, stack, *returnFp)
	if err != nil {
		return nil, fmt.Errorf("could not initialize function entrypoint, got error: %s", err)
	}

	return pc, nil
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
	// See `initialize_vm` method in Rust cairo-vm.

	return nil
}
