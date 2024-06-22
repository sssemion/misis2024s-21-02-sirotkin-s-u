cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

project(sirotkin-s-u)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

find_package(OpenCV CONFIG REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(prj.cw)
add_subdirectory(prj.lab)
