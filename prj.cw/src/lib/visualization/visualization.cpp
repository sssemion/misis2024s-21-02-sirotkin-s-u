#include "../common/common.h"
#include "visualization.h"
#include "opencv2/opencv.hpp"
#include "opencv2/freetype.hpp"

Visualization::Visualization(cv::Mat& image) {
    this->image = image;
}

void Visualization::drawSign(TrafficSign sign) {
    cv::rectangle(image, sign.top_left, sign.bottom_right, sign.color(), 2);
    cv::Size textSize = cv::getTextSize(sign.repr(), cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
    cv::Point textOrg(sign.top_left.x + 5, sign.top_left.y + textSize.height + 5);
    cv::putText(image, sign.repr(), textOrg, cv::QT_FONT_NORMAL, 0.5, sign.color(), 1);
};

cv::Ptr<cv::freetype::FreeType2> Visualization::get_ft2() {
    if (ft2 == nullptr) {
        ft2 = cv::freetype::createFreeType2();
        ft2->loadFontData( "data/roboto.ttf", 0 );
        printf("sasdasd");
    }
    return ft2;
}
