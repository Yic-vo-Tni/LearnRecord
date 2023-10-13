cmake_minimum_required(VERSION 3.3)
project(LearnImgui)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/imgui include/imgui/backends
        include src src/shader
)
#怪怪的，算了

link_directories(../lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/Api_and_Library_Learn/OpenGL_Imgui/src SRC)

#file(GLOB_RECURSE GL_SRCS ../include/GL/*.h)
file(GLOB_RECURSE IMGUI_SRCS include/imgui/*.cpp)
file(GLOB_RECURSE GLFW_SRCS src/*.cpp)
#file(GLOB_RECURSE IMAGE_SRCS include/stb-master/*.cpp)

add_executable(LearnImgui main.cpp glad.c ${IMGUI_SRCS} ${GLFW_SRCS}  ${SRC})

#target_link_libraries(LearnImgui glew32s.lib glew32.dll glew32.lib)
target_link_libraries(LearnImgui glfw3)
