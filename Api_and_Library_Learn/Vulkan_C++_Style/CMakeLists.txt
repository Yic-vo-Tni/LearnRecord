cmake_minimum_required(VERSION 3.3)
project(VulkanC++)

set(CMAKE_CXX_STANDARD 17)

include_directories(external/GLFW
        external/glm)

link_directories(lib)

find_package(Vulkan REQUIRED)

aux_source_directory(src SRCS)

add_executable(VulkanC++ main.cpp ${SRCS})

#add_custom_target(CustomTarget COMMAND cmd /c F:/Program/VulkanGaEN/compile.bat) add_dependencies(VulkanGaEN CustomTarget))


target_link_libraries(VulkanC++ glfw3)
target_link_libraries(VulkanC++ Vulkan::Vulkan)