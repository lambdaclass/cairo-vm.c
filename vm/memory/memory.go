package memory

type Memory[M MaybeRelocatable] struct {
	data map[Relocatable]M
}
