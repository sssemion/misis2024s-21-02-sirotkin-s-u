set(SRCS common.cpp)

add_library(common STATIC ${SRCS})

target_link_libraries(common PRIVATE opencv_core opencv_imgproc opencv_highgui)
