find_package(CLI11 CONFIG REQUIRED)
include_directories(${CLI11_INCLUDE_DIRS})

add_executable(main main.cpp)
target_link_libraries(main PRIVATE common detection visualization)
target_link_libraries(main PRIVATE opencv_core opencv_imgproc opencv_highgui)
target_link_libraries(main PRIVATE CLI11::CLI11)

add_subdirectory(metrics)