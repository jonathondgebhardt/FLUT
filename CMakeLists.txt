cmake_minimum_required(VERSION 3.0)
project(flut CXX Fortran)

add_library(fileprinter SHARED fileprinter.f90)
# target_compile_options(fileprinter PUBLIC -static-libgfortran)

add_executable(${PROJECT_NAME} main.cpp)
