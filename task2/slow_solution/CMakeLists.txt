cmake_minimum_required(VERSION 3.16)

project(regular-parser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_compile_options(-Wall -Wextra -Werror)
add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

include_directories(include)

add_executable(regular-parser-tests src/regular_parser_tests.cpp)