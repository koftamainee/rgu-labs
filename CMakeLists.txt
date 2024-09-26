cmake_minimum_required(VERSION 3.10)

project(rgu-labs)

set(CMAKE_C_STANDARD 99)

include_directories(libs)

file(GLOB_RECURSE SRC_FILES
    src/*.c
    libs/*.c
    00-Programming/src/*.c
    01-Bit_arithmetic/src/*.c
)

add_executable(rgu-labs ${SRC_FILES})

target_link_libraries(rgu-labs m)
