package vm

type Program struct {
	// FIXME: Should be a list of felts or MaybeRelocatables.
	data []uint
}
