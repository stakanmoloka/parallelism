cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME simpleiterations)
project(${PROJECT_NAME})

find_package(OpenMP REQUIRED)

set(EXECUTABLE_ONE var1)
set(EXECUTABLE_TWO var2)

add_executable(${EXECUTABLE_ONE} src/var1/lab2.3.cpp)
add_executable(${EXECUTABLE_TWO} src/var2/lab2.3.cpp)

target_compile_options(${EXECUTABLE_ONE} PRIVATE -fopenmp)
target_compile_options(${EXECUTABLE_TWO} PRIVATE -fopenmp)

target_link_libraries(${EXECUTABLE_ONE} PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(${EXECUTABLE_TWO} PRIVATE OpenMP::OpenMP_CXX)

