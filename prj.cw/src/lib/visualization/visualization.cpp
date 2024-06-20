#include "visualization.h"
#include "opencv2/opencv.hpp"
#include "opencv2/freetype.hpp"

Visualization::Visualization(cv::Mat& image) {
    this->image = image;
}

void Visualization::drawSign(cv::Point top_left, cv::Point bottom_right, const std::string& label, const cv::Scalar& color) {
    cv::rectangle(image, top_left, bottom_right, color, 2);
    cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
    cv::Point textOrg(top_left.x + 5, top_left.y + textSize.height + 5);
    cv::putText(image, label, textOrg, cv::QT_FONT_NORMAL, 0.5, color, 1);
};


void Visualization::drawSign(int x, int y, int width, int height, const std::string& label, const cv::Scalar& color) {
    cv::Point top_left(x, y);
    cv::Point bottom_right(x + width, y + height);
    drawSign(top_left, bottom_right, label, color);
};

cv::Ptr<cv::freetype::FreeType2> Visualization::get_ft2() {
    if (ft2 == nullptr) {
        ft2 = cv::freetype::createFreeType2();
        ft2->loadFontData( "data/roboto.ttf", 0 );
        printf("sasdasd");
    }
    return ft2;
}
