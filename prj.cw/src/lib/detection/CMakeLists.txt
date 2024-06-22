set(SRCS detection.cpp)

add_library(detection STATIC ${SRCS})

target_link_libraries(detection PRIVATE common opencv_core opencv_imgproc opencv_highgui opencv_features2d)


if(TEST)
    enable_testing()
    add_subdirectory(tests)
endif()
