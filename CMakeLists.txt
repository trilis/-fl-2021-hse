cmake_minimum_required(VERSION 3.20)
project(fl_2021_hse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPTIMIZED")
add_executable(fl_2021_hse main.cpp)
