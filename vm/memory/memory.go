package memory

// Memory represents the Cairo VM's memory.
type Memory[M MaybeRelocatable] struct {
	data map[Relocatable]M
}
