cmake_minimum_required(VERSION 3.10)
project(Any C)

option(ANY_ENABLE_MEMORY_TRACKING "Enable Any memory tracking system" OFF)

set(CMAKE_C_STANDARD 11)

add_library(Any STATIC src/any.c include/any.h)

target_include_directories(Any PUBLIC include)
if (ANY_ENABLE_MEMORY_TRACKING)
    target_compile_definitions(Any PRIVATE ANY_TRACK_ALLOCATION_COUNT=1)
endif ()

add_subdirectory(examples)
add_subdirectory(tests)
