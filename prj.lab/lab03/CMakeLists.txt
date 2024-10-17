find_package(CLI11 CONFIG REQUIRED)
include_directories(${CLI11_INCLUDE_DIRS})

add_executable(lab03 lab03.cpp)
target_link_libraries(lab03 PRIVATE opencv_core opencv_imgproc opencv_highgui)
target_link_libraries(lab03 PRIVATE CLI11::CLI11)
