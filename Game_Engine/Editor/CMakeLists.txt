cmake_minimum_required(VERSION 3.3)
project(Editor)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/yicvot_editor
        ../External/spdlog/include)


file(GLOB_RECURSE SRCS src/yicvot_editor/*.cpp)
file(GLOB_RECURSE H_SRCS src/yicvot_editor/*.h)
file(GLOB_RECURSE E_SRCS src/yicvot_editor/Event/*.h)


add_library(editor SHARED ${SRCS} ${H_SRCS})

add_compile_definitions(-DYIC_PLATFORM_WINDOWS YIC_BUILD_DLL)


target_include_directories(editor PUBLIC lib)