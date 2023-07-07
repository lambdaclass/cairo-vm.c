package memory

// Relocatable in the Cairo VM represents an address
// in some memory segment. When the VM finishes running,
// these values are replaced by real memory addresses,
// represented by a field element.
type Relocatable struct {
	segment_index int
	offset        uint
}

func NewRelocatable(segment_idx int, offset uint) Relocatable {
	return Relocatable{segment_idx, offset}
}

func (r *Relocatable) into_indexes() (uint, uint) {
	if r.segment_index < 0 {
		corrected_segment_idx := uint(-(r.segment_index + 1))
		return corrected_segment_idx, r.offset
	}

	return uint(r.segment_index), r.offset
}

// Int in the Cairo VM represents a value in memory that
// is not an address.
type Int struct {
	// FIXME: Here we should use Lambdaworks felt, just mocking
	// this for now.
	felt uint
}

// MaybeRelocatable is the type of the memory cells in the Cairo
// VM. For now, `inner` will hold any type but it should be
// instantiated only with `Relocatable`, `Int` or `nil` types.
// We should analyze better alternatives to this.
type MaybeRelocatable struct {
	inner any
}

func NewMaybeRelocatableInt(felt uint) MaybeRelocatable {
	return MaybeRelocatable{felt}
}

func (m *MaybeRelocatable) is_nil() bool {
	return m.inner == nil
}
