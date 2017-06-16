cmake_minimum_required(VERSION 3.7)
project(algo3_tp3_cmf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1 cmf-algo-exacto.cpp DisjointSet.h Eje.h Clique.h)
set(SOURCE_FILES_2 cmf-heuristica-constructiva-golosa.cpp DisjointSet.h Eje.h Clique.h)
add_executable(exacto ${SOURCE_FILES_1})
add_executable(hconstructiva ${SOURCE_FILES_2})