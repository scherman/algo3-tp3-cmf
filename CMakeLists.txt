cmake_minimum_required(VERSION 3.7)
project(test_debugger)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES_1 cmf-algo-exacto.cpp exacto.cpp cmf-algo-exacto.h)
set(SOURCE_FILES_2 cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h hconstructiva.cpp)
set(SOURCE_FILES_3 cmf-algo-exacto.cpp cmf-algo-exacto.h cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h experimentos.cpp)
set(SOURCE_FILES_4 cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h busqueda-local.h busqueda-local.cpp blocal.cpp)
add_executable(exacto ${SOURCE_FILES_1})
add_executable(hconstructiva ${SOURCE_FILES_2})
add_executable(experimentos ${SOURCE_FILES_3})
add_executable(blocal ${SOURCE_FILES_4})