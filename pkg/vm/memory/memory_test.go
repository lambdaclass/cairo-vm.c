package memory_test

import (
	"testing"

	"github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"
)

func TestMemoryInsert(t *testing.T) {
	data := make([][]memory.MaybeRelocatable, 3, 3)
	mem := memory.NewMemory(data)

	key := memory.NewRelocatable(0, 0)
	val := memory.NewMaybeRelocatableInt(5)

	err := mem.Insert(key, val)

	if err != nil {
		t.Errorf("Insert error in test: %s", err)
	}

	res_val, err := mem.Get(key)

	if res_val != val {
		t.Errorf("Inserted value and original value are not the same")
	}
}
