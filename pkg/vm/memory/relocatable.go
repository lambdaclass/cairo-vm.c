package memory

// Relocatable in the Cairo VM represents an address
// in some memory segment. When the VM finishes running,
// these values are replaced by real memory addresses,
// represented by a field element.
type Relocatable struct {
	segment_index int
	offset        uint
}

// Int in the Cairo VM represents a value in memory that
// is not an address.
type Int struct {
	// FIXME: Here we should use Lambdaworks felt, just mocking
	// this for now.
	felt uint
}

// MaybeRelocatable is the type of memory cells in the Cairo
// VM. For now, inner will hold any type but it should be
// instantiated only with Relocatable or Int types.
// We should analyze better alternatives to this.
type MaybeRelocatable struct {
	inner any
}