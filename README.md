# Introduction

Welcome to the **RGU Labs Project**! This guide will help you set up, build, and run the RGU Labs software from source. The project is designed for developers and students interested in learning the fundamentals of C development, Docker-based builds, and cross-platform compilation. With clear instructions for both Linux and Windows environments, this guide provides flexibility in building the project locally or within a Docker container.

Let’s get started with the prerequisites and dive into building and running your project!

# Build Guide

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Clone the Repository](#clone-the-repository)
3. [Build the Project](#build-the-project)
   - [3.1 Build via Docker (recommended)](#31-build-via-docker-recommended)
   - [3.2 Install Locally](#32-install-locally)
     - [3.2.1 Linux Compilation](#321-linux-compilation)
       - Build with `make` (recommended)
       - Manual Build 
     - [3.2.2 Windows Compilation](#322-windows-compilation)
       - Using MinGW
       - Using Visual Studio
4. [Run the Project](#run-the-project)
5. [License](#license)

---

# 1. Prerequisites

Before you begin, ensure you have the following software installed:

- **CMake** (version 3.10 or higher)
- **Ninja or Visual Studio** build system
- **A C Compiler** (GCC or Clang for Linux/macOS, MinGW or Visual Studio for Windows)
- **Git**


# 2. Clone the Repository

To get started, clone the repository to your local machine:

```bash
git clone https://github.com/koftamainee/rgu-labs/
cd rgu-labs
```
# 3. Build the project

## 3.1. Build via Docker (recommended)

Build project with Docker

```bash
docker build -t rgu-labs .
```
Run the application in a container:
```bash
docker run -it --rm rgu-labs
```
## 3.2. Install locally

Before compiling, edit the toolchain files to choose your compilers for Linux or Windows.

### Linux Toolchain

Open `cmake/toolchain_linux.cmake` and choose your C compiler (clang, gcc, etc.):

```cmake
# cmake/toolchain_linux.cmake

set(CMAKE_SYSTEM_NAME Linux)

# Choose your CC

set(CMAKE_C_COMPILER clang)
```

### Windows Toolchain

Open `cmake/toolchain_windows.cmake` and choose your C/CXX compiler (mingw, msvc, etc.):

```cmake
#cmake/toolchain_windows.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Choose your C/CXX compilers

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
```


## 3.2.1 Linux Compilation

### Build with make (recommended)

```bash
make build_linux
cd build
```
### Build manually

#### Create a Build Directory

```bash
mkdir build
cd build
```

#### Run CMake with Ninja

```bash
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_linux.cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### Build the Project

```bash
ninja
```

### 3.2.2 Windows Compilation

For Windows, you can use either MinGW or Visual Studio. Choose the appropriate instructions below:

#### Using MinGW

1. Open the Command Prompt or PowerShell and navigate to the build directory.

2. Run CMake with Ninja:

    ```bash
    cmake -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_windows.cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

3. Build the project:

    ```bash
    ninja
    ```

#### Using Visual Studio

1. Open the "Developer Command Prompt for VS" and navigate to the build directory.

2. Run CMake with Visual Studio:

    ```bash
    cmake -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_windows.cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

   Replace "2022" with your version of Visual Studio if necessary.

3. Build the project:

    ```bash
    cmake --build . --config Release
    ```

# 4. Run the Project

Once the build is complete, you can run the executable located in the `build/x86_64` directory:

```bash
./x86_64/rgu-labs   # For Linux and macOS
x86_64/rgu-labs.exe # For Windows
```

# 5. License

This project is licensed under the [MIT License](LICENSE).
