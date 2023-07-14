// Wrapper around the lambdaworks library. A mock, for now.
package lambdaworks

/*
#cgo LDFLAGS: pkg/lambdaworks/lib/liblambdaworks.a -ldl
#include "lib/lambdaworks.h"
*/
import "C"

type Felt = [4]uint64
type C_felt = *C.uint64_t

func c_felt(felt *Felt) C_felt {
	return C_felt(&(*felt)[0])
}

func From(value uint64) Felt {
	var result Felt
	C.from(c_felt(&result), C.uint64_t(value))
	return result
}

func Zero() Felt {
	var result Felt
	C.zero(c_felt(&result))
	return result
}

func One() Felt {
	var result Felt
	C.one(c_felt(&result))
	return result
}

func Add(a Felt, b Felt) Felt {
	var result Felt
	C.add(c_felt(&a), c_felt(&b), c_felt(&result))
	return result
}

func Sub(a Felt, b Felt) Felt {
	var result Felt
	C.sub(c_felt(&a), c_felt(&b), c_felt(&result))
	return result
}

func Mul(a Felt, b Felt) Felt {
	var result Felt
	C.mul(c_felt(&a), c_felt(&b), c_felt(&result))
	return result
}

func Div(a Felt, b Felt) Felt {
	var result Felt
	C.div(c_felt(&a), c_felt(&b), c_felt(&result))
	return result
}

// TODO: check if field equality exists in lambdaworks
func IsEqual(a Felt, b Felt) bool {
	for i := 0; i < 4; i++ {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}
