cmake_minimum_required(VERSION 3.3)
project(editor)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_MAKE_PROGRAM "make -j12")



include_directories(src/yicvot_editor
        ../External/spdlog/include
        ../External
        src
)

link_directories(../External/lib)

file(GLOB_RECURSE SRCS "src/yicvot_editor/*.cpp"
        "src/yicvot_editor/*.h"
)
file(GLOB_RECURSE imgui_srcs "../External/imgui/*.cpp")
aux_source_directory(src/platform/window "platform_window")

add_library(editor SHARED src/yic_pch.cpp ${SRCS} ${platform_window} ${imgui_srcs})
add_library(glad OBJECT src/yicvot_editor/glad.c)

target_precompile_headers(editor PRIVATE src/yic_pch.h)

add_compile_definitions(-DYIC_PLATFORM_WINDOWS YIC_BUILD_DLL)

target_link_libraries(editor PUBLIC "glfw3" "glad")