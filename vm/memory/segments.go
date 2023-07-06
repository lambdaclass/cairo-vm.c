package memory

type MemorySegmentManager[M MaybeRelocatable] struct {
	segment_sizes map[uint]uint
	memory        Memory[M]
}
