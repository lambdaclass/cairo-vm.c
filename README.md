# Cairo VM in C

This is a work in progress implementation of the [Cairo VM](https://github.com/lambdaclass/cairo-vm) in `C`. The reasons for doing this include:

- Having a diversity of implementations helps find bugs and make the whole ecosystem more resilient.
- It's a good opportunity to extensively document the VM in general, as currently the documentation on its internals is very scarce and mostly lives on the minds of a few people.

## Requirements

- A working C compiler with `C11` support.
- `clang-format` installed in the system.
- [Rust](https://www.rust-lang.org/tools/install)
- [cmake](https://cmake.org/install/)
- pkg-config
- [cpputest](http://cpputest.github.io)

## Local development

To build the vm, run:

```
make
```

This will compile the code and store the `cairo_vm` binary under the `build` directory, so you can run it like so:

```
./build/cairo_vm
```

To format the code, do

```
make fmt
```

To remove all compilation objects:

```
make clean
```

Note: When building outside of macos, you may have to run this command after building the collections lib to make the system's runtime aware of the location of the new library:

```
sudo ldconfig
```

## Tests

Tests are located in the `test` directory. To run them:

```
make test
```

## Project Guidelines

Because `C` as a language leaves everything to the programmer, a lot of things can go wrong; discipline is required. This repo has strict rules to address this. They are:

- PRs addressing performance are forbidden. We are currently concerned with making it work without bugs and nothing more.
- All PRs must contain tests. Code coverage has to be above 98%.
- To check for security and other types of bugs, the code will be fuzzed extensively.
- To catch memory errors, all code is compiled by default with [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer). The CI pipeline runs [Valgrind](https://valgrind.org/docs/manual/quick-start.html) as well.
- PRs must be accompanied by its corresponding documentation. A book will be written documenting the entire inner workings of it, so anyone can dive in to a Cairo VM codebase and follow it along.

# Documentation

## High Level Overview

The Cairo virtual machine is meant to be used in the context of STARK validity proofs. What this means is that the point of Cairo is not just to execute some code and get a result, but to *prove* to someone else that said execution was done correctly, without them having to re-execute the entire thing. The rough flow for it looks like this:

- A user writes a Cairo program.
- The program is compiled into Cairo's VM bytecode.
- The VM executes said code and provides a *trace* of execution, i.e. a record of the state of the machine and its memory *at every step of the computation*.
- This trace is passed on to a STARK prover, which creates a cryptographic proof from it, attesting to the correct execution of the program.
- The proof is passed to a verifier, who checks that the proof is valid in a fraction of a second, without re-executing.

The main three components of this flow are:

- A Cairo compiler to turn a program written in the [Cairo programming language](https://www.cairo-lang.org/) into bytecode.
- A Cairo VM to then execute it and generate a trace.
- [A STARK prover and verifier](https://github.com/lambdaclass/starknet_stack_prover_lambdaworks) so one party can prove correct execution, while another can verify it.

While this repo is only concerned with the second component, it's important to keep in mind the other two; especially important are the prover and verifier that this VM feeds its trace to, as a lot of its design decisions come from them. This virtual machine is designed to make proving and verifying both feasible and fast, and that makes it quite different from most other VMs you are probably used to.

## Basic VM flow

Our virtual machine has a very simple flow:

- Take a compiled cairo program as input. You can check out an example program [here](https://github.com/lambdaclass/cairo-vm_in_C/blob/main/cairo_programs/fibonacci.cairo), and its corresponding compiled version [here](https://github.com/lambdaclass/cairo-vm_in_C/blob/main/cairo_programs/fibonacci.json).
- Run the bytecode from the compiled program, doing the usual `fetch->decode->execute` loop, running until program termination.
- On every step of the execution, record the values of each register.
- Take the register values and memory at every step and write them to a file, called the `execution trace`.

Barring some simplifications we made, this is all the Cairo VM does. The two main things that stand out as radically different are the memory model and the use of `Field Elements` to perform arithmetic. Below we go into more detail on each step, and in the process explain the ommisions we made.

## Architecture

The Cairo virtual machine uses a Von Neumann architecture with a Non-deterministic read-only memory. What this means, roughly, is that memory is immutable after you've written to it (i.e. you can only write to it once); this is to make the STARK proving easier, but we won't go into that here.

### Memory Segments and Relocation

The process of memory allocation in a contiguous write-once memory region can get pretty complicated. Imagine you want to have a regular call stack, with a stack pointer pointing to the top of it and allocation and deallocation of stack frames and local variables happening throughout execution. Because memory is immutable, this cannot be done the usual way; once you allocate a new stack frame that memory is set, it can't be reused for another one later on.

Because of this, memory in Cairo is divided into `segments`. This is just a way of organizing memory more conveniently for this write-once model. Each segment is nothing more than a contiguous memory region. Segments are identified by an `index`, an integer value that uniquely identifies them.

Memory `cells` (i.e. values in memory) are identified by the index of the segment they belong to and an `offset` into said segment. Thus, the memory cell `{2,0}` is the first cell of segment number `2`.

Even though this segment model is extremely convenient for the VM's execution, the STARK prover needs to have the memory as just one contiguous region. Because of this, once execution of a Cairo program finishes, all the memory segments are collapsed into one; this process is called `Relocation`. We will go into more detail on all of this below.

### Registers

There are only three registers in the Cairo VM:

- The program counter `pc`, which points to the next instruction to be executed.
- The allocation pointer `ap`, pointing to the next unused memory cell.
- The frame pointer `fp`, pointing to the base of the current stack frame. When a new function is called, `fp` is set to the current `ap`. When the function returns, `fp` goes back to its previous value. The VM creates new segments whenever dynamic allocation is needed, so for example the cairo analog to a Rust `Vec` will have its own segment. Relocation at the end meshes everything together.

### Instruction Decoding/Execution

TODO: explain the components of an instruction (`dst_reg`, `op0_reg`, etc), what each one is used for and how they're encoded/decoded.

### Felts

TODO: Short explanation of Felts and the Cairo/Stark field we use through Lambdaworks.

### More on memory

- Talk about different memory segments (execution, program, etc)
- What's a `relocatable`?
- Explain the memory API, adding segments, inserting values, etc. Talk about maybe_relocatables, memory cells, why can a memory cell be empty? memory holes/gaps, etc.
- Explain relocation in detail

### Program parsing

Go through the main parts of a compiled program `Json` file. `data` field with instructions, identifiers, program entrypoint, etc.

### Code walkthrough/Write your own Cairo VM

TODO

### Builtins

TODO

### Hints

TODO
