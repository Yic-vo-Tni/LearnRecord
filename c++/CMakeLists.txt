cmake_minimum_required(VERSION 3.3)
project(learnCplusplus)

set(CMAKE_CXX_STANDARD 17)

include_directories("${PROJECT_BINARY_DIR}")
include_directories(src/include)

set(SOURCE_FILE "vector.cpp")
set(ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")

string(REPLACE "/cmake-build-debug" "" CLEAN_PATH "${ABSOLUTE_PATH}")
message("Absolute Path:${ABSOLUTE_PATH}")
message("Clean Path:${CLEAN_PATH}")

aux_source_directory(${PROJECT_SOURCE_DIR}/c++/src/include src)

if (1)
file(GLOB_RECURSE mains RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
foreach (mainfile IN LISTS mains)
    get_filename_component(cppname src/${mainfile} NAME_WE)
    add_executable(${cppname} src/${mainfile} src)
endforeach ()
endif ()

add_executable(learnCplusplus main.cpp)
#add_subdirectory(stl)
#add_subdirectory(Modern_c++)
# add_subdirectory(Algorithm)