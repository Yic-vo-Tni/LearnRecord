cmake_minimum_required(VERSION 3.3)
project(learnCplusplus)

set(CMAKE_CXX_STANDARD 17)

include_directories("${PROJECT_BINARY_DIR}")
include_directories(src/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/c++/src/include src)

file(GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
foreach (mainfile IN LISTS mains)
    get_filename_component(cppname src/${mainfile} NAME_WE)
    add_executable(${cppname} src/${mainfile} src)
endforeach ()

add_executable(learnCplusplus main.cpp)