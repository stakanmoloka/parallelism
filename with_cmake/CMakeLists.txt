option(USE_FLOAT OFF)

if(USE_FLOAT)
    add_definitions(-DUSE_FLOAT)
endif()

add_executable(sin_sum laba1.cpp)
