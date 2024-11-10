#include <opencv2/opencv.hpp>
#include <vector>
#include <random>

double iouThreshold = 0.2; // Intersection over Union Threshold - коэффициент перекрытия областей
int gaussian_ksize = 10, sigma = 25, threshold_value = 128, adaptive_block_size = 11, adaptive_C = 22;

namespace Windows {
    std::string NOISE_AND_BLUR = "Original image with noise and blur";
    std::string THRESHOLD_BINARIZATION = "Binarization: binary threshold";
    std::string ADAPTIVE_THRESHOLD = "Binarization: adaptive threshold";
    std::string OTSU_THRESHOLD = "Binarization: Otsu threshold";
}

cv::Mat original_img, noised_image, threshold_img, adaptive_img, otsu_img;
std::vector<cv::Vec3f> circles;

cv::Mat generateTestImage(std::vector<cv::Vec3f>& circles, int num_rows, int num_cols) {
    int min_radius = 4, max_radius = 48;
    int radius_step = (double)(max_radius - min_radius) / (num_cols - 1);
    int min_brightness = 30, max_brightness = 255;
    int brightness_step = (double)(max_brightness - min_brightness) / (num_rows - 1);

    int image_rows = 4 + (max_radius * 2 + 4) * num_rows + 16;
    int image_cols = 8 + (min_radius + max_radius + 8) * num_cols;

    cv::Mat img(image_rows, image_cols, CV_8UC1, cv::Scalar(0));

    for (int row = 0; row < num_rows; ++row) {
        int prev_x = 0;
        for (int col = 0; col < num_cols; ++col) {
            int radius = min_radius + col * radius_step;
            int brightness = min_brightness + row * brightness_step;

            double center_x = prev_x + 10 + radius;
            prev_x = center_x + radius;
            double center_y = 4 + (max_radius * 2 + 4) * row + max_radius;

            cv::circle(img, cv::Point(center_x, center_y), radius, cv::Scalar(brightness), -1);
            circles.push_back(cv::Vec3f(center_x, center_y, radius));
        }
    }
    return img;
}

void detectAndEvaluateCircles(cv::Mat& binImg) {
    int TP = 0, FP = 0, FN = circles.size();

    std::vector<cv::Vec3f> detected_circles;
    cv::HoughCircles(binImg, detected_circles, cv::HOUGH_GRADIENT, 1, binImg.rows / 8, 100, 10, 4, 50);

    cv::cvtColor(binImg, binImg, cv::COLOR_GRAY2RGB);

    for (auto& circle : detected_circles) {
        cv::circle(binImg, cv::Point(circle[0], circle[1]), circle[2], cv::Scalar(255, 0, 255), 2);
    }

    for (const auto& detected : detected_circles) {
        bool matched = false;
        for (const auto& real : circles) {
            double distance = cv::norm(cv::Point2f(detected[0], detected[1]) - cv::Point2f(real[0], real[1]));
            if (distance < (detected[2] + real[2]) * iouThreshold) {
                TP++;
                FN--;
                matched = true;
                break;
            }
        }
        if (!matched)
            FP++;
    }

    std::stringstream text;
    text << "TP: " << TP << "/" << circles.size();
    text << "; FP: " << FP << "/" << detected_circles.size();
    text << "; FN: " << FN << "/" << circles.size();
    cv::rectangle(binImg, cv::Point(0, binImg.rows - 16), cv::Point(binImg.cols, binImg.rows), cv::Scalar(0), cv::FILLED);
    cv::putText(binImg, text.str(), cv::Point(4, binImg.rows - 6), cv::FONT_HERSHEY_DUPLEX, 0.4, cv::Scalar(255, 0, 255));
}

void applyBinarization(int, void*) {
    cv::threshold(noised_image, threshold_img, threshold_value, 255, cv::THRESH_BINARY);
    detectAndEvaluateCircles(threshold_img);
    cv::imshow(Windows::THRESHOLD_BINARIZATION, threshold_img);
}

void applyAdaptiveBinarization(int, void*) {
    adaptive_block_size += (1 - adaptive_block_size % 2);
    adaptive_block_size = std::max(3, adaptive_block_size);

    cv::adaptiveThreshold(noised_image, adaptive_img, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, adaptive_block_size, adaptive_C - 20);
    detectAndEvaluateCircles(adaptive_img);
    cv::imshow(Windows::ADAPTIVE_THRESHOLD, adaptive_img);
}

void applyOtsuBinarization(int, void*) {
    cv::threshold(noised_image, otsu_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    detectAndEvaluateCircles(otsu_img);
    cv::imshow(Windows::OTSU_THRESHOLD, otsu_img);
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
void applyBlurAndNoise(int, void*) {
    gaussian_ksize += (1 - gaussian_ksize % 2);
    cv::GaussianBlur(original_img, noised_image, cv::Size(gaussian_ksize, gaussian_ksize), 0);
    addNoise(noised_image, sigma);
    cv::imshow(Windows::NOISE_AND_BLUR, noised_image);
    applyBinarization(0, 0);
    applyAdaptiveBinarization(0, 0);
    applyOtsuBinarization(0, 0);
}

int main() {
    original_img = generateTestImage(circles, 5, 6);

    cv::namedWindow(Windows::NOISE_AND_BLUR, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Gaussian blur kernel", Windows::NOISE_AND_BLUR, &gaussian_ksize, 50, applyBlurAndNoise);
    cv::createTrackbar("Noise stddev (sigma)", Windows::NOISE_AND_BLUR, &sigma, 50, applyBlurAndNoise);

    cv::namedWindow(Windows::THRESHOLD_BINARIZATION, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Threshold", Windows::THRESHOLD_BINARIZATION, &threshold_value, 255, applyBinarization);

    cv::namedWindow(Windows::ADAPTIVE_THRESHOLD, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Block Size", Windows::ADAPTIVE_THRESHOLD, &adaptive_block_size, 50, applyAdaptiveBinarization);
    cv::createTrackbar("C (x-20), x = ", Windows::ADAPTIVE_THRESHOLD, &adaptive_C, 40, applyAdaptiveBinarization);

    cv::namedWindow("Binarization - Otsu's Threshold", cv::WINDOW_AUTOSIZE);

    applyBlurAndNoise(0, 0);

    cv::waitKey(0);

    return 0;
}
