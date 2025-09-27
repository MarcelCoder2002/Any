cmake_minimum_required(VERSION 4.0)
project(Any C)

set(CMAKE_C_STANDARD 11)

add_library(Any STATIC src/any.c include/any.h)

target_include_directories(Any PRIVATE include)

add_subdirectory(examples)
add_subdirectory(tests)
