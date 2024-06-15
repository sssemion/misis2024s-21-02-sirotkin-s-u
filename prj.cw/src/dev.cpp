#include "detection/detection.h"

int main() {
    cv::Mat img = cv::imread("dev.jpeg");

    Detection detector(img);
    detector.blur(5);

    auto red_mask = detector.get_mask(Detection::RED_RANGES);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(red_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Фильтрация контуров по площади
    std::vector<std::vector<cv::Point>> filtered_contours;
    double min_area = 1000;
    for (const auto& cnt : contours) {
        double area = cv::contourArea(cnt);
        if (area > min_area) {
            filtered_contours.push_back(cnt);
        }
    }

    cv::drawContours(img, filtered_contours, -1, {0, 255, 0}, 3);

    cv::imshow("Blurred", img);

    cv::waitKey(0);
}