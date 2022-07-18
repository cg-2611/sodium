# Sodium
The Sodium programming language is currently in the very early stages of development.

**The Windows operating system is not currently supported.**

### Contents:
- [nac](#nac)
- [Documentation](#documentation)
- [Building Sodium](#building-sodium)
- [Run Unit Tests](#run-unit-tests)
- [Sodium Plan](#sodium-plan)

## nac
---
`nac` is the Sodium compiler. The name is derived from the chemical symbol for the element sodium: **Na c**-ompiler. The Sodium compiler will be able to be used to compiler programs written in Sodium.

The compiler will use [LLVM](https://github.com/llvm/llvm-project) as a compiler back-end.

## Documentation
---
The public API found in the include directory contains [doxygen](https://doxygen.nl/) comments to document the classes and methods.
Currently, documentation is not generated during the build process, however there are plans to change this in the future.
Feel free to generate the doxygen documentation yourself if you require it.

## Building Sodium
---
**Sodium currently requires LLVM version 14 in order to be built.**

Sodium uses [`cmake`](https://cmake.org/) to generate it's build system.

> Currently only the Sodium compiler is in development. Therefore the following instructions show how to build the Sodium compiler: `nac`.

First clone the repository and change to it's directory:
```
git clone --recurse-submodules https://github.com/cg-2611/sodium.git
cd sodium
```
Then to build the `nac` executable:
```
cmake -B build
cmake --build build
```
The executable can then be run using:
```
./build/nac path/to/sodium/file.na
```

### Run Unit Tests
---
To run the unit tests for the Sodium compiler, first, build the project using:
```
cmake -B build -DBUILD_TESTS=ON
```
Then from the root directory of Sodium, run the following command:
```
ctest --test-dir build --output-on-fail
```

### Sodium Plan
---
Currently, Sodium is in version 0.0.0.

To see the current plan for future Sodium versions, see [PLAN.md](./PLAN.md).
