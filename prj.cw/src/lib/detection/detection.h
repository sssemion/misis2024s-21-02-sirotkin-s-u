#ifndef DETECTION_H
#define DETECTION_H

#include <opencv2/opencv.hpp>
#include "../common/common.h"

class Detection {
public:
    cv::Mat image;

    static inline const std::vector<ColorRange> RED_RANGES = {
        { cv::Scalar(0, 10, 10), cv::Scalar(30, 255, 255) },
        { cv::Scalar(150, 10, 10), cv::Scalar(180, 255, 255) },
    };

    static inline const std::vector<ColorRange> BLUE_RANGES = {
        { cv::Scalar(100, 70, 50), cv::Scalar(140, 255, 255) },
    };

    explicit Detection(cv::Mat& image);

    void blur(int size);

    cv::Mat get_mask(const std::vector<ColorRange> &ranges) const;
};

#endif //DETECTION_H
