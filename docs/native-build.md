# Native Build

This is the initial host-side C/SDL scaffold for the port.

## Requirements

- CMake 3.20+
- SDL2 development package

## Configure

```sh
cmake -S . -B build
```

## Build

```sh
cmake --build build
```

## Run

```sh
./build/crusn
```

The current executable only initializes the machine model and presents a memory-backed SDL framebuffer with a simple test pattern.
