package memory

import "errors"

// Memory represents the Cairo VM's memory.
type Memory struct {
	data [][]MaybeRelocatable
}

func (m *Memory) insert(key Relocatable, val MaybeRelocatable) (MaybeRelocatable, error) {
	key_idx, key_offset := key.into_indexes()

	// FIXME: There should be a special handling if the key
	// segment index is negative. This is an edge
	// case, so for now let's raise an error.
	if key.segment_index < 0 {
		return MaybeRelocatable{key}, errors.New("Segment index of key is negative - unimplemented")
	}

	segment := m.data[key_idx]
}
