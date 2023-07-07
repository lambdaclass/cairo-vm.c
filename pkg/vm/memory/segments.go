package memory

// MemorySegmentManager manages the list of memory segments.
// Also holds metadata useful for the relocation process of
// the memory at the end of the VM run.
type MemorySegmentManager[M MaybeRelocatable] struct {
	segment_sizes map[uint]uint
	memory        Memory[M]
}
