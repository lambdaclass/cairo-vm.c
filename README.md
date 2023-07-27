# Cairo VM in C

This is a work in progress implementation of the [Cairo VM](https://github.com/lambdaclass/cairo-vm) in `C`. The reasons for doing this include:

- Having a diversity of implementations helps find bugs and make the whole ecosystem more resilient.
- It's a good opportunity to extensively document the VM in general, as currently the documentation on its internals is very scarce and mostly lives on the minds of a few people.

## Requirements

- A working C compiler with `C11` support.
- `clang-format` installed in the system.
- [Rust](https://www.rust-lang.org/tools/install)

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

The cairo memory is made up of contiguous segments of variable length identified by their index. The first segment (index 0) is the program segment, which stores the instructions of a cairo program. The following segment (index 1) is the execution segment, which holds the values that are created along the execution of the vm, for example, when we call a function, a pointer to the next instruction after the call instruction will be stored in the execution segment which will then be used to find the next instruction after the function returns. The following group of segments are the builtin segments, one for each builtin used by the program, and which hold values used by the builtin runners. The last group of segments are the user segments, which represent data structures created by the user, for example, when creating an array on a cairo program, that array will be represented in memory as its own segment.

An address (or pointer) in cairo is represented as a `relocatable` value, which is made up of a `segment_index` and an `offset`, the `segment_index` tells us which segment the value is stored in and the `offset` tells us how many values exist between the start of the segment and the value.

As the cairo memory can hold both felts and pointers, the basic memory unit is a `maybe_relocatable`, a variable that can be either a `relocatable` or a `felt`

While memory is continous, some gaps may be present. These gaps can be created on purpose by the user, for example by running:

```
[ap + 1] = 2;
```

Where a gap is created at ap. But they may also be created indireclty by diverging branches, as for example one branch may declare a variable that the other branch doesn't, as memory needs to be allocated for both cases if the second case is ran then a gap is left where the variable should have been written.

#### Memory API

The memory can perform the following basic operations:

- `memory_add_segment`: Creates a new, empty segment in memory and returns a pointer to its start. Values cannot be inserted into a memory segment that hasn't been previously created.

- `memory_insert`: Inserts a `maybe_relocatable` value at an address indicated by a `relocatable` pointer. For this operation to succeed, the pointer's segment_index must be an existing segment (created using `memory_add_segment`), and there mustn't be a value stored at that address, as the memory is immutable after its been written once. If there is a value already stored at that address but it is equal to the value to be inserted then the operation will be successful.

- `memory_get`: Fetches a `maybe_relocatable` value from a memory address indicated by a `relocatable` pointer.

Other operations:

- `memory_load_data`: This is a convenience method, which takes an array of `maybe_relocatable` and inserts them contiguosuly in memory by calling `memory_insert` and advancing the pointer by one after each insertion. Returns a pointer to the next free memory slot after the inserted data.

#### Memory Relocation

During execution, the memory consists of segments of varying length, and they can be accessed by indicating their segment index, and the offset within that segment. When the run is finished, a relocation process takes place, which transforms this segmented memory into a contiguous list of values. The relocation process works as follows:

1- The size of each segment is calculated (The size is equal to the highest offset within the segment + 1, and not the amount of `maybe_relocatable` values, as there can be gaps)
2- A base is assigned to each segment by accumulating the size of the previous segment. The first segment's base is set to 1.
3- All `relocatable` values are converted into a single integer by adding their `offset` value to their segment's base calculated in the previous step

For example, if we have this memory represented by address, value pairs:

    0:0 -> 1
    0:1 -> 4
    0:2 -> 7
    1:0 -> 8
    1:1 -> 0:2
    1:4 -> 0:1
    2:0 -> 1

Step 1: Calculate segment sizes:

    0 -> 3
    1 -> 5
    2 -> 1

Step 2: Assign a base to each segment:

    0 -> 1
    1 -> 4 (1 + 3)
    2 -> 9 (4 + 5)

Step 3: Convert relocatables to integers

    1 (base[0] + 0) -> 1
    2 (base[0] + 1) -> 4
    3 (base[0] + 2) -> 7
    4 (base[1] + 0) -> 8
    5 (base[1] + 1) -> 3 (base[0] + 2)
    .... (memory gaps)
    8 (base[1] + 4) -> 2 (base[0] + 1)
    9 (base[2] + 0) -> 1


### Program parsing

Go through the main parts of a compiled program `Json` file. `data` field with instructions, identifiers, program entrypoint, etc.

### Code walkthrough/Write your own Cairo VM

TODO

### Builtins

TODO

### Hints

TODO
