package vm

import "github.com/lambdaclass/cairo-vm.go/vm/memory"

type RunContext struct {
	pc memory.Relocatable
	ap uint
	fp uint
}
