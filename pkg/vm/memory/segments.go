package memory

// MemorySegmentManager manages the list of memory segments.
// Also holds metadata useful for the relocation process of
// the memory at the end of the VM run.
type MemorySegmentManager struct {
	segmentSizes map[uint]uint
	Memory       Memory
}

// Returns the number of segments present in memory
func (ms *MemorySegmentManager) NumSegments() int {
	return len(ms.Memory.data)
}

// Adds a new segment to the memory and returns the base address
// of that segment
func (ms *MemorySegmentManager) Add() *Relocatable {
	extendedData := append((*ms).Memory.data, []MaybeRelocatable{})
	ms.Memory.data = extendedData

	addedSegmentIdx := len(extendedData) - 1

	return NewRelocatable(addedSegmentIdx, 0)
}
