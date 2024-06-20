#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>

class Visualization {
public:
    cv::Mat image;

    explicit Visualization(cv::Mat& image);

    void drawSign(cv::Point top_left, cv::Point bottom_right, const std::string& label, const cv::Scalar& color);

    void drawSign(int x, int y, int width, int height, const std::string& label, const cv::Scalar& color);

private:
    cv::Ptr<cv::freetype::FreeType2> ft2;

    cv::Ptr<cv::freetype::FreeType2> get_ft2();
};

#endif //VISUALIZATION_H
