project(zeus)
cmake_minimum_required(VERSION 3.5.0)

file(GLOB SOURCES src/*.cpp)
include_directories(include)
include_directories(../plog/include)
add_executable(zeus ${SOURCES})
target_link_libraries(zeus png)