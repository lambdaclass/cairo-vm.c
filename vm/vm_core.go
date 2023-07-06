package vm

import "github.com/lambdaclass/cairo-vm.go/vm/memory"

type VirtualMachine[M memory.MaybeRelocatable] struct {
	run_conext   RunContext
	current_step uint
	segments     memory.MemorySegmentManager[M]
}
