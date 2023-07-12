# Cairo VM in C

This is a work in progress implementation of the [Cairo VM](https://github.com/lambdaclass/cairo-vm) in `C`. The reasons for doing this include:

- Having a diversity of implementations helps find bugs and make the whole ecosystem more resilient.
- It's a good opportunity to extensively document the VM in general, as currently the documentation on its internals is very scarce and mostly lives on the minds of a few people.

## Requirements

- A working C compiler with `C11` support.

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
