# CANtina C++ Interface Compilation Guide

This document outlines the steps to compile the `cantina_interface` C++ shared library (`.dll`) for the CANtina project. This library handles the low-level communication with the PCAN hardware by dynamically loading the `PCANBasic.dll`.

## Prerequisites

*   **CMake**: Version 3.15 or higher.
*   **C++ Compiler**: A C++ compiler toolchain compatible with CMake, such as MinGW (used here with "MinGW Makefiles").
*   **PCANBasic SDK**:
    *   `PCANBasic.dll`: Must be present in the `cpp/lib/` directory. This DLL is loaded at runtime.
    *   `PCANBasic.h`: Must be present in the `cpp/lib/` directory. This header file is required at compile time for function declarations and type definitions.

## Directory Structure (Relevant Parts)

```
CANtina/
├── cpp/
│   ├── include/
│   │   └── cantina/
│   │       └── pcan_interface.h  // Header for the C++ interface
│   │   ├── lib/
│   │   │   ├── PCANBasic.dll         // PCAN Basic API DLL (runtime)
│   │   │   └── PCANBasic.h           // PCAN Basic API Header (compile time)
│   │   ├── src/
│   │   │   └── pcan_interface.cpp    // Implementation of the C++ interface
│   │   └── CMakeLists.txt            // CMake build script for the C++ library
│   └── ...
```

## Compilation Steps

1.  **Navigate to the C++ source directory**:
    Open a terminal or command prompt and change to the `cpp` directory within your project.
    ```bash
    cd path/to/your/CANtina/project/cpp
    ```

2.  **Create and navigate to the build directory**:
    It's good practice to perform out-of-source builds.
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure the project using CMake**:
    This step generates the build files for your chosen build system (e.g., MinGW Makefiles).
    ```bash
    cmake .. -G "MinGW Makefiles"
    ```
    *   `..` points CMake to the parent directory where the main `CMakeLists.txt` for the C++ library is located.
    *   `-G "MinGW Makefiles"` specifies the generator. Adjust if you use a different compiler/build system (e.g., "Visual Studio 17 2022").

4.  **Build the project**:
    This step compiles the source code and creates the shared library.
    ```bash
    cmake --build .
    ```

## Output

After a successful build, the compiled shared library will be located in the `cpp/build/` directory:

*   `cantina_interface.dll`

This DLL can then be used by other applications or, as intended in this project, by a Python wrapper. 