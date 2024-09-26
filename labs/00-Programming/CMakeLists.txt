# CMakeLists.txt for 00-Programming

# Get all .c files in the src directory
file(GLOB PROGRAMMING_SRC "src/*.c")

# Create a library for all source files
add_library(program00 ${PROGRAMMING_SRC})
