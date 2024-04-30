cmake_minimum_required(VERSION 3.8)

project(SearchTool VERSION 1.0 DESCRIPTION "Search Tool for files")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
include_directories(include)
add_compile_options(-Wall -Wextra)