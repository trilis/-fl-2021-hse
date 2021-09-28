cmake_minimum_required(VERSION 3.19)
project(fl_2021_hse)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(fl_2021_hse
        regex_engine.cpp
        regex_engine.h)
