cmake_minimum_required(VERSION 3.0)

project(contest)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wshadow -fsanitize=undefined")
set(SRC "main.cpp"
        "regexp.cpp")
set (HEADERS "regexp.hpp")

add_executable(derivative ${SRC} ${HEADERS})