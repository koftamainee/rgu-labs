
# Build Guide

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Clone the Repository](#clone-the-repository)
3. [Build the Project](#build-the-project)
4. [Run the Project](#run-the-project)
5. [License](#license)

---

## 1. Prerequisites

Before you begin, ensure you have the following software installed:

- **CMake** (version 3.10 or higher)
- **Ninja** (optional, but recommended for faster builds)
- **A C Compiler** (GCC or Clang for Linux/macOS, MinGW or Visual Studio for Windows)
- **Git**


## 2. Clone the Repository

To get started, clone the repository to your local machine:

```bash
git clone https://github.com/koftamainee/rgu-labs/
cd rgu-labs
```

## 3. Build the Project

#### Create a Build Directory

```bash
mkdir build
cd build
```

#### Run CMake with Ninja

```bash
cmake -G Ninja ..
```

#### Build the Project

```bash
ninja
```

## 4. Run the Project

Once the build is complete, you can run the executable:

```bash
./rgu-labs   # For Linux and macOS
rgu-labs.exe # For Windows
```

## 5. License

This project is licensed under the [MIT License](LICENSE).
