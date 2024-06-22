find_package(CLI11 CONFIG REQUIRED)
include_directories(${CLI11_INCLUDE_DIRS})

add_executable(metrics main.cpp)
target_link_libraries(metrics PRIVATE common detection visualization)
target_link_libraries(metrics PRIVATE opencv_core opencv_imgproc opencv_highgui)
target_link_libraries(metrics PRIVATE CLI11::CLI11)
