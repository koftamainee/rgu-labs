
# Build Guide

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Clone the Repository](#clone-the-repository)
3. [Edit Toolchains](#edit-toolchains)
   - [Linux Toolchain](#linux-toolchain)
   - [Windows Toolchain](#windows-toolchain)
4. [Build the Project](#build-the-project)
   - [Linux Compilation](#linux-compilation)
   - [Windows Compilation](#windows-compilation)
5. [Run the Project](#run-the-project)
6. [License](#license)

---

## 1. Prerequisites

Before you begin, ensure you have the following software installed:

- **CMake** (version 3.10 or higher)
- **Ninja or Visual Studio** build system
- **A C Compiler** (GCC or Clang for Linux/macOS, MinGW or Visual Studio for Windows)
- **Git**


## 2. Clone the Repository

To get started, clone the repository to your local machine:

```bash
git clone https://github.com/koftamainee/rgu-labs/
cd rgu-labs
```

## 3. Edit Toolchains

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

## 4. Build the Project

#### Create a Build Directory

```bash
mkdir build
cd build
```

### Linux Compilation

#### Run CMake with Ninja

```bash
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_linux.cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### Build the Project

```bash
ninja
```

### Windows Compilation

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

## 5. Run the Project

Once the build is complete, you can run the executable located in the `build/release` directory:

```bash
./release/rgu-labs   # For Linux and macOS
release/rgu-labs.exe # For Windows
```

## 6. License

This project is licensed under the [MIT License](LICENSE).
