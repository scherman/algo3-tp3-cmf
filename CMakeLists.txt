cmake_minimum_required(VERSION 3.7)
project(test_debugger)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES_1 cmf-algo-exacto.cpp exacto.cpp cmf-algo-exacto.h)
set(SOURCE_FILES_2 cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h cmf-algo-exacto.cpp cmf-algo-exacto.h hconstructiva.cpp)
set(SOURCE_FILES_3 cmf-algo-exacto.cpp cmf-algo-exacto.h cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h busqueda-local.cpp busqueda-local.h graspv2.cpp graspv2.h experimentos.cpp)
set(SOURCE_FILES_4 cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h busqueda-local.h busqueda-local.cpp blocal.cpp cmf-algo-exacto.cpp cmf-algo-exacto.h cmf-heuristica-blocal.h cmf-heuristica-blocal.cpp)
set(SOURCE_FILES_5 cmf-heuristica-constructiva-golosa.cpp cmf-heuristica-constructiva-golosa.h busqueda-local.h busqueda-local.cpp meta-heuristica.h meta-heuristica.cpp mheuristica.cpp)
set(SOURCE_FILES_5 grasp.h grasp.cpp mheuristica.cpp)
set(SOURCE_FILES_6 cmf-algo-exacto.h cmf-algo-exacto.cpp cmf-heuristica-constructiva-golosa.h cmf-heuristica-constructiva-golosa.cpp busqueda-local.h busqueda-local.cpp graspv2.h graspv2.cpp mheuristica2.cpp mheuristica2.cpp)

add_executable(exacto ${SOURCE_FILES_1})
add_executable(hconstructiva ${SOURCE_FILES_2})
add_executable(experimentos ${SOURCE_FILES_3})
add_executable(blocal ${SOURCE_FILES_4})
add_executable(mheuristica ${SOURCE_FILES_5})
add_executable(mheuristica2 ${SOURCE_FILES_6})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/GRASP/bin)

