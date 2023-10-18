cmake_minimum_required(VERSION 3.3)
project(VulkanC++)

set(CMAKE_CXX_STANDARD 20)

include_directories(external/GLFW
        external/glm)

link_directories(lib)

find_package(Vulkan REQUIRED)
find_program(GLSLC glslc REQUIRED)

aux_source_directory(src SRCS)

execute_process(COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shader/*.vert -o ${CMAKE_SOURCE_DIR}/shader/*vert.spv)
execute_process(COMMAND ${GLSLC} ${CMAKE_SOURCE_DIR}/shader/*.frag -o ${CMAKE_SOURCE_DIR}/shader/*frag.spv)

add_executable(VulkanC++ main.cpp ${SRCS})


target_link_libraries(VulkanC++ glfw3)
target_link_libraries(VulkanC++ Vulkan::Vulkan)