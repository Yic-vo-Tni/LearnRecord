cmake_minimum_required(VERSION 3.4)
project(learn_cmake)

include_directories(${PROJECT_SOURCE_DIR}/cmake/src)

#set(src cmake/main.cpp
#        cmake/src/yic_math.h
#        cmake/src/yic_math.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/cmake/src src)
set(src cmake/main.cpp
        ${src})

#file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/cmake*.cpp)

#set(EXECUTABLE_OUTPUT_PATH ../accessibility_tools/cmake)
set(CMAKE_CXX_STANDARD 17)

add_executable(learn_cmake cmake/main.cpp ${src})