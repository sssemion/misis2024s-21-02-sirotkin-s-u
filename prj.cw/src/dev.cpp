#include "detection/detection.h"

int main() {
    cv::Mat img = cv::imread("dev.jpeg");

    Detection detector(img);

    auto red_mask = detector.getMask(Detection::RED_RANGES);

    auto contours = detector.getContours(red_mask);

//    detector.filterMask(red_mask);

    auto circles = detector.getCircles(red_mask);

    for (int i = 0; i < circles.size(); ++i) {
        cv::Vec3f circle = circles[i];
        cv::Point center = cv::Point(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(img, center, radius, cv::Scalar(0, 255, 0), 2);
    }

    auto triangles = detector.getTriangles(contours);

    cv::drawContours(img, triangles, -1, (0, 255, 255), 2);

    cv::imshow("Mask", red_mask);
    cv::imshow("Image", img);

    cv::waitKey(0);
}