#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat generateTestImage(int brightness1, int brightness2, int brightness3) {
    int img_size = 256;
    int square_size = 209;
    int circle_radius = 83;
    cv::Mat img = cv::Mat::zeros(img_size, img_size, CV_8UC1);

    // background
    img = cv::Scalar(brightness1);

    // inner square
    int square_offset = (img_size - square_size) / 2;
    cv::Rect inner_rect(square_offset, square_offset, square_size, square_size);
    img(inner_rect) = cv::Scalar(brightness2);

    // circle
    cv::Point center(img_size / 2, img_size / 2);
    circle(img, center, circle_radius, cv::Scalar(brightness3), -1);

    return img;
}

cv::Mat drawHistogram(const cv::Mat& img) {
    int histSize = 256;    // bin size
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    cv::Mat hist;

    calcHist(&img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    normalize(hist, hist, 0, 230, cv::NORM_MINMAX);

    int hist_w = 256, hist_h = 256;
    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(230));

    for (int i = 0; i < histSize; i++) {
        line(histImage, cv::Point(i, hist_h), cv::Point(i, hist_h - cvRound(hist.at<float>(i))), cv::Scalar(0), 1, 8, 0);
    }

    return histImage;
}

cv::Mat addNoise(const cv::Mat& img, double sigma) {
    cv::Mat noise = cv::Mat::zeros(img.size(), img.type());
    randn(noise, 0, sigma);
    cv::Mat noised_image;
    img.convertTo(noised_image, CV_32F);
    noised_image += noise;
    noised_image.convertTo(noised_image, CV_8UC1);
    return noised_image;
}

void generateAndDisplay() {
    std::vector<std::vector<int>> brightness_levels = { {0, 127, 255}, {20, 127, 235}, {55, 127, 200}, {90, 127, 165} };
    std::vector<double> sigmas = { 3.0, 7.0, 15.0 };

    cv::Mat combined_image;

    // Generate and combine test images
    for (auto levels : brightness_levels) {
        cv::Mat test_img = generateTestImage(levels[0], levels[1], levels[2]);
        if (combined_image.empty()) {
            combined_image = test_img;
        } else {
            hconcat(combined_image, test_img, combined_image);
        }
    }

    for (double sigma : sigmas) {
        cv::Mat combined_noised_image;
        for (auto levels : brightness_levels) {
            cv::Mat test_img = generateTestImage(levels[0], levels[1], levels[2]);
            cv::Mat noised_img = addNoise(test_img, sigma);
            cv::Mat hist_img = drawHistogram(noised_img);

            if (combined_noised_image.empty()) {
                vconcat(noised_img, hist_img, combined_noised_image);
            } else {
                cv::Mat temp;
                vconcat(noised_img, hist_img, temp);
                hconcat(combined_noised_image, temp, combined_noised_image);
            }
        }

        vconcat(combined_image, combined_noised_image, combined_image);
    }

    imshow("Combined Image", combined_image);
    cv::waitKey(0);
}

int main(int argc, char** argv) {
    generateAndDisplay();
    return 0;
}