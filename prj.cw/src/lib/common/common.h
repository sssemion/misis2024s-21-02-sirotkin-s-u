#ifndef COMMON_H
#define COMMON_H

#include <opencv2/opencv.hpp>

struct ColorRange {
    cv::Scalar lower, upper;
};

struct TrafficSign {
    cv::Point top_left, bottom_right;
    enum {
        PROHIBITING,
        WARNING,
        YELD,
    } type;

    static inline const cv::Scalar PROHIBITING_COLOR = {0, 0, 255},
            WARNING_COLOR = {0, 255, 255},
            YELD_COLOR = {255, 127, 255};

    inline std::string repr() const {
        switch (type) {
            case PROHIBITING: return "Prohibiting";
            case WARNING: return "Warning";
            case YELD: return "Yeld";
        }
    };

    inline cv::Scalar color() const {
        switch (type) {
            case PROHIBITING: return PROHIBITING_COLOR;
            case WARNING: return WARNING_COLOR;
            case YELD: return YELD_COLOR;
        }
    }
};

struct Triangle {  // остроугольный треугольник
    cv::Point p1, p2, p3;
    enum {
        NORMAL, INVERTED
    } orientation;
};

#endif //COMMON_H
