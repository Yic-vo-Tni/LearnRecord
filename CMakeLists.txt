cmake_minimum_required(VERSION 3.3)

project(learn_record)

set(ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
string(REPLACE "/cmake-build-debug" "" CLEAN_PATH "${ABSOLUTE_PATH}")
message("Absolute Path:${ABSOLUTE_PATH}")
message("Clean Path:${CLEAN_PATH}")

# add_subdirectory(accessibility_tools)
 add_subdirectory(c++)
# add_subdirectory(library/OpenGL_Imgui)
# add_subdirectory(Game_Engine)