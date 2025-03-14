#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include "detection.h"

Detection::Detection(cv::Mat& image) {
    cv::cvtColor(image, this->image, cv::COLOR_BGR2HSV);
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

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    return mask;
}

std::vector<std::vector<cv::Point>> Detection::getContours(const cv::Mat &mask) const {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Фильтрация контуров по площади
    std::vector<std::vector<cv::Point>> filtered_contours;
    double min_area = image.rows * image.cols * 0.001;  // 0.1% of overall area
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
    cv::HoughCircles(mask, circles, cv::HOUGH_GRADIENT, 1, mask.rows / 4.0, 250, 27, 0, 0);
    return circles;
}

std::vector<Triangle> Detection::getTriangles(const std::vector<std::vector<cv::Point>> &contours) const {
    std::vector<Triangle> triangles;
    for (const auto& cnt : contours) {
        std::vector<cv::Point> approx;
        double epsilon = 0.05 * cv::arcLength(cnt, true);
        cv::approxPolyDP(cnt, approx, epsilon, true);
        if (approx.size() == 3) {
            std::sort(approx.begin(), approx.end(), [](const cv::Point& a, const cv::Point& b){return a.y < b.y;});
            cv::Point p1 = approx[0], p2 = approx[1], p3 = approx[2];
            if ( (p2.x < p1.x && p1.x < p3.x) || (p3.x < p1.x && p1.x < p2.x) ) {
                triangles.push_back({p1, p2, p3, Triangle::NORMAL});
            } else if ( (p1.x < p3.x && p3.x < p2.x) || (p2.x < p3.x && p3.x < p1.x) ) {
                triangles.push_back({p1, p2, p3, Triangle::INVERTED});
            }
        }
    }
    return triangles;
}

std::vector<TrafficSign> Detection::proceed() const {
    std::vector<TrafficSign> result;

    auto red_mask = getMask(Detection::RED_RANGES);
    filterMask(red_mask);
    auto contours = getContours(red_mask);
    auto circles = getCircles(red_mask);

    for (auto &circle: circles) {
        cv::Point top_left((int) (circle[0] - circle[2]), (int) (circle[1] - circle[2]));
        cv::Point bottom_right((int) (circle[0] + circle[2]), (int) (circle[1] + circle[2]));
        result.push_back({top_left, bottom_right, TrafficSign::PROHIBITING});
    }

    auto triangles = getTriangles(contours);
    for (auto &triangle: triangles) {
        cv::Point top_left(
                std::min(triangle.p1.x, std::min(triangle.p2.x, triangle.p3.x)),
                std::min(triangle.p1.y, std::min(triangle.p2.y, triangle.p3.y))
        );
        cv::Point bottom_right(
                std::max(triangle.p1.x, std::max(triangle.p2.x, triangle.p3.x)),
                std::max(triangle.p1.y, std::max(triangle.p2.y, triangle.p3.y))
        );
        if (triangle.orientation == Triangle::NORMAL) {
            result.push_back({top_left, bottom_right, TrafficSign::WARNING});
        } else {
            result.push_back({top_left, bottom_right, TrafficSign::YIELD});
        }
    }

    return result;
}