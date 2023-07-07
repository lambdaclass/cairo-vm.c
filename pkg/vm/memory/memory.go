package memory

// Memory represents the Cairo VM's memory.
type Memory struct {
	data map[Relocatable]MaybeRelocatable
}
