cmake_minimum_required(VERSION 3.10)

project(rgu-labs)

set(CMAKE_C_STANDARD 99)
set(CMAKE_GENERATOR "Ninja")

include_directories(libs)

# Adding all src files
file(GLOB_RECURSE SRC_FILES
    src/*.c
    libs/*.c
    labs/00-Programming/src/*.c
    labs/01-Bit_arithmetic/src/*.c
)

add_executable(rgu-labs ${SRC_FILES})

set_target_properties(rgu-labs PROPERTIES RUNTIME_OUTPUT_DIRECTORY release)

# Bash script doesnt work on Windows
if(WIN32)
    message(STATUS "Compiling for Windows. Skipping Bash lab")
else()
    file(COPY labs/02-Bash_lab DESTINATION release/scripts)
endif()

target_link_libraries(rgu-labs m)