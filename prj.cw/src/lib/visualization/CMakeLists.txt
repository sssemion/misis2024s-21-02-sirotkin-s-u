set(SRCS visualization.cpp)

add_library(visualization STATIC ${SRCS})

target_link_libraries(visualization PRIVATE common opencv_core opencv_imgproc opencv_highgui opencv_freetype)
