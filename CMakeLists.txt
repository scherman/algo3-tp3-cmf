cmake_minimum_required(VERSION 3.7)
project(algo3_tp3_cmf)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES cmf-algo-exacto.cpp DisjointSet.h Eje.h Clique.h)
add_executable(algo3_tp3_cmf ${SOURCE_FILES})