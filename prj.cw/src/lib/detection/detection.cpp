#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include "detection.h"

Detection::Detection(cv::Mat& image) {
    cv::cvtColor(image, this->image, cv::COLOR_BGR2HSV);
}

void Detection::blur(int size) {
    cv::GaussianBlur(image, image, cv::Size(size, size), 0);
}

cv::Mat Detection::get_mask(const std::vector<ColorRange>& ranges) const {
    cv::Mat mask;
    for (auto &range : ranges) {
        cv::Mat tmp;
        cv::inRange(image, range.lower, range.upper, tmp);
        if (mask.empty())
            mask = tmp;
        else
            mask |= tmp;
    }

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    return mask;
}