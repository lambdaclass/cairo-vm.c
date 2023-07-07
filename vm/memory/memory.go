package memory

import "github.com/lambdaclass/cairo-vm.go/vm/memory"

// Memory represents the Cairo VM's memory.
type Memory[M MaybeRelocatable] struct {
	data [][]M
}

func (m *Memory[M]) insert(key memory.Relocatable, val M) M {
	key_idx, key_offset := key.into_indexes()
}
