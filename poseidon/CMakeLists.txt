project(poseidon)
cmake_minimum_required(VERSION 3.5.0)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES src/*.cpp)
include_directories(include)
add_executable(poseidon ${SOURCES})
target_compile_options(poseidon PUBLIC -std=c++11 -Wall -g -O2)
target_link_libraries(poseidon ${OpenCV_LIBS})