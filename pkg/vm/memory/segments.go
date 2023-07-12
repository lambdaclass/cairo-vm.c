package memory

import "errors"

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

func (ms *MemorySegmentManager) LoadData(ptr Relocatable, data *[]MaybeRelocatable) (*Relocatable, error) {
	dataLen := len(*data)

	for i := dataLen; i >= 1; i-- {
		insertAddr := NewRelocatable(ptr.SegmentIndex, ptr.Offset+uint(i))
		err := ms.Memory.Insert(insertAddr, &(*data)[i-1])
		if err != nil {
			return nil, errors.New("insertion error")
		}
	}

	return NewRelocatable(ptr.SegmentIndex, ptr.Offset+uint(len(*data))), nil
}
