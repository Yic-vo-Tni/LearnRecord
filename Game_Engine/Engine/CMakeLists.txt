cmake_minimum_required(VERSION 3.3)
project(Engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(../Editor/src
        ../External/spdlog/include)

set(SOURCES src/yicApp.cpp)

add_executable(yicvot ${SOURCES})

add_compile_definitions(-DYIC_PLATFORM_WINDOWS)

add_custom_command(TARGET yicvot POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/../Editor/libeditor.dll
        ${CMAKE_CURRENT_BINARY_DIR}/libeditor.dll)


target_include_directories(yicvot PRIVATE include)
target_link_libraries(yicvot PRIVATE editor)