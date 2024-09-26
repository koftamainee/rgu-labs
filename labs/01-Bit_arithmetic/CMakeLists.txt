# CMakeLists.txt for 01-Bit_arithmetic

# Get all .c files in the src directory
file(GLOB BINARY_ARITHMETIC_SRC "src/*.c")

# Create a library for all source files
add_library(program01 ${BINARY_ARITHMETIC_SRC})
