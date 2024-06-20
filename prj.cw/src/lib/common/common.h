#ifndef COMMON_H
#define COMMON_H

#include <opencv2/opencv.hpp>

struct ColorRange {
    cv::Scalar lower, upper;
};

struct Triangle {  // остроугольный треугольник
    cv::Point p1, p2, p3;
    enum {
        NORMAL, INVERTED
    } orientation;
};

#endif //COMMON_H
