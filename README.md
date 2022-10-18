# Sodium
The Sodium programming language is currently in the very early stages of development.

**The Windows operating system is not currently supported.**

### Contents:
- [nac](#nac)
- [Building Sodium](#building-sodium)
- [Run Unit Tests](#run-unit-tests)
- [Sodium Plan](#sodium-plan)

## nac
---
`nac` is the Sodium compiler. The Sodium compiler will be able to be used to compiler programs written in Sodium.

The compiler uses [LLVM](https://github.com/llvm/llvm-project) as a compiler back-end.

## Building Sodium
---
**Sodium currently requires LLVM version 15 in order to be built.**

> Currently only the Sodium compiler is in development.
> Therefore the following instructions show how to build the Sodium compiler: `nac`.

First clone the repository and change to it's directory:
```
git clone https://github.com/cg-2611/sodium.git
cd sodium
```
Then to build the `nac` executable:
```
cargo build --bin nac
```
The executable can then be run using:
```
./target/debug/nac path/to/sodium/file.na
```

### Run Unit Tests
---
To run the unit tests for the Sodium compiler, run the following command:
```
cargo test --bin nac
```

### Sodium Plan
---
Currently, Sodium is in version 0.0.0.
To see the current plan for future Sodium versions, see [PLAN.md](./PLAN.md).
