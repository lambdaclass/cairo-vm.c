package memory_test

import (
	"reflect"
	"testing"

	"github.com/lambdaclass/cairo-vm.go/pkg/vm/memory"
)

func TestMemoryInsert(t *testing.T) {
	// Instantiate memory with 3 empty segments
	data := make([][]memory.MaybeRelocatable, 3)
	mem := memory.NewMemory(data)

	// Instantiate the address where we want to insert and the value.
	// We will insert the value Int(5) in segment 1, offset 0
	key := memory.NewRelocatable(1, 0)
	val := memory.NewMaybeRelocatableInt(5)

	// Make the insertion
	err := mem.Insert(key, val)
	if err != nil {
		t.Errorf("Insert error in test: %s", err)
	}

	// Get the value from the address back
	res_val, err := mem.Get(key)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}

	// Check that the original and the retrieved values are the same
	if !reflect.DeepEqual(res_val, val) {
		t.Errorf("Inserted value and original value are not the same")
	}
}

func TestMemoryInsertWithHoles(t *testing.T) {
	// Instantiate memory with 3 empty segments
	data := make([][]memory.MaybeRelocatable, 3)
	mem := memory.NewMemory(data)

	// Instantiate the address where we want to insert and the value.
	// We will insert the MaybeRelocatable Int(7) in segment 1, offset 2
	key := memory.NewRelocatable(1, 2)
	val := memory.NewMaybeRelocatableInt(7)

	// Make the insertion
	err := mem.Insert(key, val)
	if err != nil {
		t.Errorf("Insert error in test: %s", err)
	}

	// Get the value from the address back
	resVal, err := mem.Get(key)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}

	// Check that the original and the retrieved values are the same
	if !reflect.DeepEqual(resVal, val) {
		t.Errorf("Inserted value and original value are not the same")
	}

	// Since we inserted in segment 1, offset 2 in an empty memory, now
	// the values in segment 1, offset 0 and 1 should be `nil` (memory holes)
	hole1Addr := memory.NewRelocatable(1, 0)
	hole2Addr := memory.NewRelocatable(1, 1)

	hole1, err := mem.Get(hole1Addr)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}

	hole2, err := mem.Get(hole2Addr)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}

	// Check that we got the holes from memory
	expected_hole := memory.NewMaybeRelocatableNil()
	if !reflect.DeepEqual(hole1, expected_hole) || !reflect.DeepEqual(hole2, expected_hole) {
		t.Errorf("Expected nil value but got another")
	}
}

func TestAddSegment(t *testing.T) {
	// Instantiate memory with 3 empty segments
	data := make([][]memory.MaybeRelocatable, 3)
	mem := memory.NewMemory(data)

	// Instantiate MemorySegmentManager
	segmentManager := memory.MemorySegmentManager{}
	segmentManager.Memory = *mem

	// Check that number of segments is 3 before adding another one
	if segmentManager.NumSegments() != 3 {
		t.Errorf("The number of memory segments should be 3")
	}

	// Add a new segment to the memory, now we should have 4 segments
	newSegmentAddr := segmentManager.Add()

	// The address of the base of new segment should be (3, 0)
	expectedAddr := memory.NewRelocatable(3, 0)
	if !reflect.DeepEqual(newSegmentAddr, expectedAddr) {
		t.Errorf("The new segment address is not the expected")
	}

	if segmentManager.NumSegments() != 4 {
		t.Errorf("The number of memory segments should be 4")
	}
}

func TestLoadData(t *testing.T) {
	// Instantiate memory with 3 empty segments
	segment0 := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(1), *memory.NewMaybeRelocatableInt(3)}
	segment1 := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(7), *memory.NewMaybeRelocatableInt(5)}
	memData := [][]memory.MaybeRelocatable{segment0, segment1}
	mem := memory.NewMemory(memData)

	// Instantiate MemorySegmentManager
	segmentManager := memory.MemorySegmentManager{}
	segmentManager.Memory = *mem

	data := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(8), *memory.NewMaybeRelocatableInt(13)}

	startPtr := *memory.NewRelocatable(1, 2)

	endPtr, err := segmentManager.LoadData(startPtr, &data)
	if err != nil {
		t.Errorf("LoadData error: %s", err)
	}

	expectedEndPtr := memory.NewRelocatable(1, 4)
	if !reflect.DeepEqual(*endPtr, *expectedEndPtr) {
		t.Errorf("LoadData end pointer and expected pointer are not equal")
	}
}

func TestLoadDataWithHoles(t *testing.T) {
	// Instantiate memory with 3 empty segments
	segment0 := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(1), *memory.NewMaybeRelocatableInt(3)}
	segment1 := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(7), *memory.NewMaybeRelocatableInt(5)}
	memData := [][]memory.MaybeRelocatable{segment0, segment1}
	mem := memory.NewMemory(memData)

	// Instantiate MemorySegmentManager
	segmentManager := memory.MemorySegmentManager{}
	segmentManager.Memory = *mem

	data := []memory.MaybeRelocatable{*memory.NewMaybeRelocatableInt(8), *memory.NewMaybeRelocatableInt(13)}

	startPtr := *memory.NewRelocatable(1, 4)

	endPtr, err := segmentManager.LoadData(startPtr, &data)
	if err != nil {
		t.Errorf("LoadData error: %s", err)
	}

	expectedEndPtr := memory.NewRelocatable(1, 6)
	if !reflect.DeepEqual(*endPtr, *expectedEndPtr) {
		t.Errorf("LoadData end pointer and expected pointer are not equal")
	}

	// There should be memory holes in (1, 2) and (1, 3)
	hole1Addr := memory.NewRelocatable(1, 2)
	hole2Addr := memory.NewRelocatable(1, 3)
	hole1, err := mem.Get(hole1Addr)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}
	hole2, err := mem.Get(hole2Addr)
	if err != nil {
		t.Errorf("Get error in test: %s", err)
	}

	expected_hole := memory.NewMaybeRelocatableNil()
	if !reflect.DeepEqual(hole1, expected_hole) || !reflect.DeepEqual(hole2, expected_hole) {
		t.Errorf("Expected nil value but got another")
	}
}
