cmake_minimum_required(VERSION 3.7)
project(test_debugger)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1 cmf-algo-exacto.cpp)
set(SOURCE_FILES_2 cmf-heuristica-constructiva-golosa.cpp)
add_executable(exacto ${SOURCE_FILES_1})
add_executable(hconstructiva ${SOURCE_FILES_2})