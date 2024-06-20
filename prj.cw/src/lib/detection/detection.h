#ifndef DETECTION_H
#define DETECTION_H

#include <opencv2/opencv.hpp>
#include "../common/common.h"

class Detection {
public:
    cv::Mat image;

    static inline const std::vector<ColorRange> RED_RANGES = {
        { cv::Scalar(0, 50, 90), cv::Scalar(9, 255, 255) },
        { cv::Scalar(171, 50, 90), cv::Scalar(180, 255, 255) },
    };

    static inline const std::vector<ColorRange> BLUE_RANGES = {
        { cv::Scalar(100, 70, 50), cv::Scalar(140, 255, 255) },
    };

    explicit Detection(cv::Mat& image);

    cv::Mat getMask(const std::vector<ColorRange> &ranges) const;

    std::vector<std::vector<cv::Point>> getContours(const cv::Mat& mask) const;

    void filterMask(cv::Mat& mask) const;

    std::vector<cv::Vec3f> getCircles(const cv::Mat& mask) const;

    std::vector<Triangle> getTriangles(const std::vector<std::vector<cv::Point>> &contours) const;
};

#endif //DETECTION_H
