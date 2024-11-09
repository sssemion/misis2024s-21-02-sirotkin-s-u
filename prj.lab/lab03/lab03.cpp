#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "CLI/CLI.hpp"

std::string GRAYSCALE = "grayscale", JOINT = "joint", PER_CHANNEL = "per-channel";
std::vector<cv::Scalar> COLORS = {{192, 0, 0}, {0, 192, 0}, {0, 0, 192}};

class Args {

public:
    double black_q = 0.05;
    double white_q = 0.95;
    std::string filename;
    std::string mode = GRAYSCALE;

    Args(int argc, char **argv) : argc{argc}, argv{argv} {
        parse_args();
    }

private:
    int argc;
    char **argv;

    void parse_args() {
        CLI::App app;
        app.add_option("-b", black_q, "Black quantile");
        app.add_option("-w", white_q, "White quantile");
        app.add_option("filename", filename, "Image filename")
            ->required()->check(CLI::Validator(CLI::ExistingFile));
        app.add_option("--mode", mode, "grayscale|joint|per-channel")
            ->check(CLI::IsMember({GRAYSCALE, JOINT, PER_CHANNEL}));
        app.parse(argc, argv);
    }
};

std::vector<int> calc_histogram(const cv::Mat& img) {
    std::vector<int> histogram(256, 0);
    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            uchar intensity;
            if (img.type() == CV_8UC1)
                intensity = img.at<uchar>(y, x);
            else
                intensity = cv::sum(img.at<cv::Vec3b>(y, x))[0] / 3;
            ++histogram[intensity];
        }
    }
    return histogram;
}

cv::Mat draw_histogram(const std::vector<int>& histogram, const cv::Mat& img, const cv::Scalar color = cv::Scalar(127), bool gray = true) {

    cv::Mat histogram_image;
    if (gray)
        histogram_image = cv::Mat(256, 256, CV_8UC1, cv::Scalar(255));
    else
        histogram_image = cv::Mat(256, 256, CV_8UC3, cv::Scalar(255, 255, 255));

    int max_histogram_value = *std::max_element(histogram.begin(), histogram.end());
    for (int i = 0; i < 256; ++i) {
        int y = ((double)(histogram[i]) / max_histogram_value * 230);
        cv::line(histogram_image, cv::Point(i, 256), cv::Point(i, 256 - y), color, 1);
    }

    // накопительная
    int total_pixels = img.rows * img.cols;
    double cumm = histogram[0];
    for (int i = 1; i < histogram.size(); ++i) {
        int y1 = cumm / total_pixels * 255;
        cumm += histogram[i];
        int y2 = cumm / total_pixels * 255;
        cv::line(histogram_image, cv::Point(i - 1, 256 - y1), cv::Point(i, 256 - y2), cv::Scalar(0), 1);
    }
    return histogram_image;
}


cv::Mat autoContrast(const cv::Mat& img, const std::vector<int>& histogram, double black_quantile, double white_quantile) {
    int cumm = 0;
    int total_pixels = img.rows * img.cols;
    int min_value = 0, max_value = 0;
    black_quantile *= total_pixels;
    for (int i = 0; i < 256; ++i) {
        cumm += histogram[i];
        if (cumm >= black_quantile) {
            min_value = i;
            break;
        }
    }

    cumm = 0;
    white_quantile = total_pixels * (1 - white_quantile);
    for (int i = 255; i >= 0; --i) {
        cumm += histogram[i];
        if (cumm >= white_quantile) {
            max_value = i;
            break;
        }
    }

    cv::Mat new_img;
    img.copyTo(new_img);

    // Применяем линейное растяжение
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            uchar pixelValue = img.at<uchar>(i, j);
            if (pixelValue < min_value) {
                pixelValue = 0;
            } else if (pixelValue > max_value) {
                pixelValue = 255;
            } else {
                pixelValue = static_cast<uchar>(255.0 * (pixelValue - min_value) / (max_value - min_value));
            }
            new_img.at<uchar>(i, j) = pixelValue;
        }
    }
    return new_img;
}

void resize(cv::Mat& img, int h) {
    double k = ((double)h) / img.rows;
    cv::resize(img, img, cv::Size(img.cols * k, h));
}

int main(int argc, char **argv) {
    Args args(argc, argv);
    cv::Mat original_image = cv::imread(args.filename);
    resize(original_image, 512);
    cv::Mat result;

    cv::Mat image;
    std::vector<int> original_histogram = calc_histogram(original_image);
    cv::Mat combined_histograms;

    if (args.mode == GRAYSCALE) {
        cv::cvtColor(original_image, original_image, cv::COLOR_BGR2GRAY);
        image = autoContrast(original_image, original_histogram, args.black_q, args.white_q);
        cv::vconcat(
            draw_histogram(original_histogram, original_image),
            draw_histogram(calc_histogram(image), image),
            combined_histograms
        );
    } else {
        std::vector<cv::Mat> channels;
        cv::split(original_image, channels);
        for (int i = 0; i < channels.size(); ++i) {
            std::vector<int> original_ch_histogram = calc_histogram(channels[i]);
            if (args.mode == PER_CHANNEL)
                original_histogram = original_ch_histogram;
            channels[i] = autoContrast(channels[i], original_histogram, args.black_q, args.white_q);
            cv::Mat tmp;
            cv::vconcat(
                draw_histogram(original_ch_histogram, channels[i], COLORS[i], false),
                draw_histogram(calc_histogram(channels[i]), channels[i], COLORS[i], false),
                tmp
            );
            if (combined_histograms.empty())
                combined_histograms = tmp;
            else
                cv::hconcat(combined_histograms, tmp, combined_histograms);
        }
        cv::merge(channels, image);
    }

    cv::hconcat(original_image, image, result);
    cv::hconcat(result, combined_histograms, result);

    cv::imshow("lab03", result);
    cv::waitKey(0);
    return 0;
}
