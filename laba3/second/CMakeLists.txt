cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME server-client)
project(${PROJECT_NAME})


set(EXECUTABLE_ONE server)
set(EXECUTABLE_TWO test)

add_executable(${EXECUTABLE_ONE} src/main.cpp)
add_executable(${EXECUTABLE_TWO} src/test.cpp)

