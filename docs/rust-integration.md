# Lambdaworks wrapper

For now, this is a mock containing only a "number" function that always return 42. It's a
proof of concept of a go package that integrates with a rust library.

## Package structure

These are the relevant files:

```
./pkg/lambdaworks: go package wrapper so that other packages can call this
        │          one without worrying about FFI, C or rust.
        ├──lambdaworks.go: go wrapper library code. Casts C types to go types.
        └── lib: directory with the rust and C code.
            ├── lambdaworks.h: C headers representing the functions that will
            │                  be exported to go.
            └── lambdaworks: Rust package.
                ├── Cargo.Toml: Rust package definition.
                └── src/lib.rs: rust library.
```

## Compiling

The rust package can be compiled by moving to the `lib/lambdaworks` directory and executing `cargo build --release`. This produces a `lib/lambdaworks/target/liblambdaworks.a` file that is, in turn, imported by `lambdaworks.go` as a static library, as we can see in the comment over the `Number` function.

In `Makefile` we can see that one of the steps is actually moving to the rust project, compiling, and then copying the archive file to the `lib` directory so it can be consumed by the go code.

## FFI

FFI is a way of having calls between C and rust. To allow this we have two key steps:
- Wrapping function calls (with `no_mangle`` modifier) and adding a header file to make them available to C.
- Making sure that all data structures can be represented in C.

`lambdaworks.h` is the file containing all of the function wrappers around the field element type. As FieldElement and all its variants are already defined in lambdaworks and do not conform
to the C memory layout we need to have our own representation with the `repr(C)` modifier.
Internally, field elements are represented by an array of four integers called "limbs", using
montgomery representation, and do not contain any extra data. For simplicity, we then choose to
represent them in C as that array, and `felt_t` is just an alias for an unsigned 64 bit integer array with four elements.

As we can't return arrays in C, any rust function that returns a felt (one, zero, from, mul, add, etc) has a `result` with a raw pointer that is written instead. Memory allocation can happen
statically in C, as they will always be 4-element arrays.

`lib.rs` mostly does, for every wrapper function:
- Convert argument felts (limb arrays) to rust felts.
- Call the wrapped operation
- Convert the result back to limbs representation in the result parameter.

## Limbs representation

We need to take into account that we take the `felt.representative()` limbs instead of `felt.value().limbs` version, as the latter is in montgomery representation. If we took that one 
instead, when building the `UnsignedInteger<4> struct` we'll be representing a different number.
When constructiong the `FieldElement` from that, the montgomery algorithm will be applied even if the data is already in that representation, effectively building a Felt that represents a different number.

