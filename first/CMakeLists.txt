cmake_minimum_required(VERSION 3.10)

set(project_name laba2.1)
project(${project_name})

find_package(OpenMP REQUIRED)

add_executable(${project_name} src/lab2.1.cpp)

target_link_libraries(${project_name} PRIVATE OpenMP::OpenMP_C)
