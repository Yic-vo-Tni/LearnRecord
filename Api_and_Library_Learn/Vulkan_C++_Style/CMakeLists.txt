cmake_minimum_required(VERSION 3.3)
project(VulkanC++)

set(CMAKE_CXX_STANDARD 20)

include_directories(external/GLFW
        external/glm)

link_directories(lib)

find_package(Vulkan REQUIRED)
find_program(GLSLC glslc REQUIRED)

aux_source_directory(src SRCS)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shader)
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shader)
set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shader)

file(GLOB_RECURSE VERTEX_SHADERS "${SHADER_SOURCE_DIR}/*.vert")
file(GLOB_RECURSE FRAGMENT_SHADERS "${SHADER_SOURCE_DIR}/*.frag")

foreach (VERTEX_SHADER ${VERTEX_SHADERS})
    get_filename_component(SHADER_NAME ${VERTEX_SHADER} NAME_WE)
    execute_process(COMMAND ${GLSLC} ${VERTEX_SHADER} -o ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.vert.spv)
endforeach ()

foreach (FRAGMENT_SHADER ${FRAGMENT_SHADERS})
    get_filename_component(SHADER_NAME ${FRAGMENT_SHADER} NAME_WE)
    execute_process(COMMAND ${GLSLC} ${FRAGMENT_SHADER} -o ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.frag.spv)
endforeach ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SHADER_OUTPUT_DIR})

#execute_process(COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shader/*.vert -o ${CMAKE_SOURCE_DIR}/shader/*vert.spv)
#execute_process(COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shader/*.frag -o ${CMAKE_SOURCE_DIR}/shader/*frag.spv)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/shader)

add_executable(VulkanC++ main.cpp ${SRCS})


target_link_libraries(VulkanC++ glfw3)
target_link_libraries(VulkanC++ Vulkan::Vulkan)