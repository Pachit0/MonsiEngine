# MonsiEngine

**Monsi Engine** is a lightweight, modular, and high-performance engine built in C++ for game and graphics development.

## Features In Development

- Skeletal Animations
- Gizmos
- Play Button
- Jolt Physics
- Indirect instancing
- Bindless rendering

## Features

- High-performance rendering using modern C++ and GLSL.
- Modular architecture for easy extension and customization.
- Sandbox environment for testing and prototyping.

## Getting Started

### Prerequisites

- C++20 compatible compiler (GCC, Clang, MSVC)
- CMake 4.4
- OpenGL 4.6

### Build Instructions
```bash
git clone --recurse-submodules https://github.com/Pachit0/MonsiEngine.git
cd MonsiEngine
mkdir build && cd build
cmake ..
make OR cmake build . --config <debug/release>
If you are using visual studio go to: tools->options->CMake->CMake Executable directory: C:\Program Files\CMake\bin (make sure you have CMake 4.4)
```
