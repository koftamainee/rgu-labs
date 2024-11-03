cmake_minimum_required(VERSION 3.10)

project(rgu-labs)

set(CMAKE_C_STANDARD 99)

include_directories(libs)

# Adding all src files
file(GLOB_RECURSE SRC_FILES
    src/*.c
    libs/src/*.c
    labs/00-Programming/src/*.c
    labs/01-Bit_arithmetic/src/*.c
    labs/03-Programming/src/*.c
)

add_executable(rgu-labs ${SRC_FILES})

set_target_properties(rgu-labs PROPERTIES RUNTIME_OUTPUT_DIRECTORY x86_64)

# Bash script doesnt work on Windows
if(WIN32)
    message(STATUS "Compiling for Windows. Skipping Bash lab")
else()
    file(COPY labs/02-Bash_lab DESTINATION x86_64/scripts)
endif()

file(COPY files DESTINATION x86_64)

target_link_libraries(rgu-labs m)