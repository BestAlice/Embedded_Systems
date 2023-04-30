cmake_minimum_required(VERSION 3.21)
project(1_lab C)

set(CMAKE_C_STANDARD 99)

include_directories(./code)
include_directories(./headers)

file(GLOB code_files
        "code/*.c"
        "headers/*.h"
        )

add_executable(PE_read ${code_files})


