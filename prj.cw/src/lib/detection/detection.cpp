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

cv::Mat Detection::getMask(const std::vector<ColorRange>& ranges) const {
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

std::vector<std::vector<cv::Point>> Detection::getContours(const cv::Mat &mask) const {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Фильтрация контуров по площади
    std::vector<std::vector<cv::Point>> filtered_contours;
    double min_area = image.rows * image.cols * 0.01;
    for (const auto& cnt : contours) {
        double area = cv::contourArea(cnt);
        if (area > min_area)
            filtered_contours.push_back(cnt);
    }

    return filtered_contours;
}

void Detection::filterMask(cv::Mat &mask) const {
    auto contours = getContours(mask);
    mask = cv::Scalar(0);
    cv::drawContours(mask, contours, -1, {255}, 1);
}

std::vector<cv::Vec3f> Detection::getCircles(const cv::Mat &mask) const {
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(mask, circles, cv::HOUGH_GRADIENT, 1, mask.rows / 4.0, 200, 22, 0, 0);
    return circles;
}

std::vector<std::vector<cv::Point>> Detection::getTriangles(const std::vector<std::vector<cv::Point>> &contours) const {
    std::vector<std::vector<cv::Point>> triangles;
    for (const auto& cnt : contours) {
        std::vector<cv::Point> approx;
        double epsilon = 0.02 * cv::arcLength(cnt, true);
        cv::approxPolyDP(cnt, approx, epsilon, true);
        if (approx.size() == 3)
            triangles.push_back(approx);
    }
    return triangles;
}
