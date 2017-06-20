project(hermes)
cmake_minimum_required(VERSION 3.5.0)

find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(../plog/include)

file(GLOB CXX_SOURCES src/*.cpp)
add_executable(hermes ${CXX_SOURCES})
target_compile_options(hermes PUBLIC -std=c++11 -Wall -O3)
target_link_libraries(hermes ${OpenCV_LIBS})