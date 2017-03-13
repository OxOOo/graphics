project(zeus)
cmake_minimum_required(VERSION 3.5.0)

add_compile_options(-std=c++11 -Wall -g)

file(GLOB SOURCES src/*.cpp)
include_directories(include)
include_directories(../plog/include)
include_directories(../libpng)
add_executable(zeus ${SOURCES})
target_link_libraries(zeus png)