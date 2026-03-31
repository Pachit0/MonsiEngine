# MonsiEngine

**Monsi Engine** is a lightweight, modular, and high-performance engine built in C++ for game and graphics development.

## Features

- High-performance rendering using modern C++ and GLSL.
- Modular architecture for easy extension and customization.
- Sandbox environment for testing and prototyping.

## Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.16+
- OpenGL (so far)

### Build Instructions
```bash
git clone --recurse-submodules https://github.com/Pachit0/MonsiEngine.git
cd MonsiEngine
mkdir build && cd build
cmake ..
make OR cmake build . --config <debug/release>
```
