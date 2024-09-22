#include <opencv2/opencv.hpp>
#include <random>
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
    cv::Mat histogramImage(256, 256, CV_8UC1, cv::Scalar(230));

    std::vector<int> histogram(256, 0);
    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            int intensity = img.at<uchar>(y, x);
            histogram[intensity]++;
        }
    }

    // Нормируем гистограмму, чтобы максимальное значение имело высоту 230
    int maxHistogramValue = *std::max_element(histogram.begin(), histogram.end());
    for (int& value : histogram) {
        value = (int)((double)(value) / maxHistogramValue * 230);
    }

    for (int i = 0; i < 256; ++i) {
        int binValue = histogram[i];
        cv::line(histogramImage, cv::Point(i, 256),
                 cv::Point(i, 256 - binValue), cv::Scalar(0), 1);
    }

    return histogramImage;
}

void addNoise(cv::Mat& img, double sigma) {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, sigma);

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            int new_value = img.at<uchar>(i, j) + (int)std::round(distribution(generator));
            new_value = std::min(255, std::max(0, new_value));
            img.at<uchar>(i, j) = new_value;
        }
    }
}


void generateAndDisplay() {
    std::vector<std::vector<int>> brightness_levels = { {0, 127, 255}, {20, 127, 235}, {55, 127, 200}, {90, 127, 165} };
    std::vector<double> sigmas = { 0.0, 3.0, 7.0, 15.0 };

    cv::Mat combined_image;

    for (double sigma : sigmas) {
        cv::Mat combined_noised_image;
        for (auto levels : brightness_levels) {
            cv::Mat test_img = generateTestImage(levels[0], levels[1], levels[2]);
            addNoise(test_img, sigma);
            cv::Mat hist_img = drawHistogram(test_img);

            if (combined_noised_image.empty()) {
                vconcat(test_img, hist_img, combined_noised_image);
            } else {
                cv::Mat temp;
                vconcat(test_img, hist_img, temp);
                hconcat(combined_noised_image, temp, combined_noised_image);
            }
        }

        if (combined_image.empty()) {
            combined_image = combined_noised_image;
        } else {
            vconcat(combined_image, combined_noised_image, combined_image);
        }
    }

    imshow("Lab02", combined_image);
    cv::waitKey(0);
}

int main(int argc, char** argv) {
    generateAndDisplay();
    return 0;
}
