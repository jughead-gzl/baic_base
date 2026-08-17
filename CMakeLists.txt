cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")

project(BAIC_BASE LANGUAGES CXX)

add_subdirectory(algorithm)
add_subdirectory(structure)
add_subdirectory(test)

add_executable(baic_base main.cpp)

