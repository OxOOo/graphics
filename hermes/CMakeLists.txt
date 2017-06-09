project(hermes)
cmake_minimum_required(VERSION 3.5.0)

add_compile_options(-std=c++11 -Wall -g)

include_directories(include)
include_directories(../plog/include)
include_directories(../libpng)

FIND_PACKAGE(CUDA REQUIRED)

file(GLOB CU_SOURCES cuda/*.cu)
set(GENCODE -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_60,code=sm_60 -gencode arch=compute_60,code=compute_60)
cuda_add_library(exec ${CU_SOURCES})

file(GLOB CXX_SOURCES src/*.cpp)
add_executable(hermes ${CXX_SOURCES})
target_link_libraries(hermes png exec)