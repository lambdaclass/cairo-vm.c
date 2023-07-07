package vm

import "github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"

// VirtualMachine represents the Cairo VM.
// Runs Cairo assembly and produces an execution trace.
type VirtualMachine[M memory.MaybeRelocatable] struct {
	run_conext   RunContext
	current_step uint
	segments     memory.MemorySegmentManager[M]
}
