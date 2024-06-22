#ifndef COMMON_H
#define COMMON_H

#include <opencv2/opencv.hpp>

struct ColorRange {
    cv::Scalar lower, upper;
};

struct TrafficSign {
    cv::Point top_left, bottom_right;
    enum {
        PROHIBITING = 0,
        WARNING = 1,
        YIELD = 2,
    } type;

    static inline const cv::Scalar PROHIBITING_COLOR = {0, 0, 255},
            WARNING_COLOR = {0, 255, 255},
            YIELD_COLOR = {255, 127, 255};

    inline std::string repr() const {
        switch (type) {
            case PROHIBITING: return "Prohibiting";
            case WARNING: return "Warning";
            case YIELD: return "Yield";
        }
    };

    inline cv::Scalar color() const {
        switch (type) {
            case PROHIBITING: return PROHIBITING_COLOR;
            case WARNING: return WARNING_COLOR;
            case YIELD: return YIELD_COLOR;
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
