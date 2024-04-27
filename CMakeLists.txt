cmake_minimum_required(VERSION 3.0)
project(flut CXX Fortran)

add_library(fileprinter_1 SHARED fileprinter.f90)
# target_compile_options(fileprinter_1 PUBLIC -static-libgfortran)
add_library(fileprinter_2 SHARED fileprinter.f90)
# target_compile_options(fileprinter_1 PUBLIC -static-libgfortran)

add_executable(${PROJECT_NAME} main.cpp)
