#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <opencv2/opencv.hpp>
#include "../common/common.h"
#include <opencv2/freetype.hpp>

class Visualization {
public:
    cv::Mat image;

    explicit Visualization(cv::Mat& image);

    void drawSign(TrafficSign sign);

private:
    cv::Ptr<cv::freetype::FreeType2> ft2;

    cv::Ptr<cv::freetype::FreeType2> get_ft2();
};

#endif //VISUALIZATION_H
