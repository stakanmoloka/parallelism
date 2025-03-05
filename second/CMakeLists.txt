cmake_minimum_required(VERSION 3.10)

set(project_name laba2.2)
project(${project_name})

find_package(OpenMP REQUIRED)

add_executable(${project_name} src/lab2.2.cpp)

target_link_libraries(${project_name} PRIVATE OpenMP::OpenMP_C)

target_compile_options(${project_name} PRIVATE -lm)
