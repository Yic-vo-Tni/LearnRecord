cmake_minimum_required(VERSION 3.3)
project(learnCplusplus)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(${PROJECT_SOURCE_DIR}/c++/stl/include src)

file(GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp")
foreach (mainfile IN LISTS mains)
    get_filename_component(cppname include/${mainfile} NAME_WE)
    add_executable(${cppname} include/${mainfile} ${src})
endforeach ()