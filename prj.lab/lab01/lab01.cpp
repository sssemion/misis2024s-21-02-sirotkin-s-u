#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "CLI/CLI.hpp"

class Args {

public:
    int width = 3;
    int height = 30;
    double gamma = 2.4;
    std::string filename;

    Args(int argc, char **argv) : argc{argc}, argv{argv} {
        parse_args();
    }

private:
    int argc;
    char **argv;

    void parse_args() {
        CLI::App app;
        app.set_help_flag("-H,--help", "Help"); // -h is reserved by default, change to -H
        app.add_option("-h", height, "Height");
        app.add_option("-s", width, "Width");
        app.add_option("--gamma", gamma, "Gamma");
        app.add_option("filename", filename, "Result filename");

        app.parse(argc, argv);
    }
};



cv::Mat1b generate_gradient(int width, int height) {
    cv::Mat1b image(height, width * 256, CV_8UC1);
    for (int color = 0; color < 256; ++color)
        for (int i = 0; i < height; ++i)
            for (int j = width * color; j < width * color + width; ++j)
                image.at<uchar>(i, j) = (uchar)color;
    return image;
}

cv::Mat1b apply_gamma_correction(cv::Mat1b &image, double gamma) {
    cv::Mat1b result(image.size(), image.type());
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            uchar result_color = 255 * pow((double)image.at<uchar>(i, j) / 255, 1.0 / gamma);
            result.at<uchar>(i, j) = result_color;
        }
    }

    return result;
}


int main(int argc, char **argv) {
    Args args(argc, argv);

    cv::Mat1b gradient = generate_gradient(args.width, args.height);
    cv::Mat1b gamma_corrected = apply_gamma_correction(gradient, args.gamma);
    cv::Mat1b result;
    cv::vconcat(gradient, gamma_corrected, result);

    if (args.filename.empty()) {
        cv::imshow("Gradient + gamma-corrected", result);
        cv::waitKey(0);
    } else {
        cv::imwrite(args.filename, result);
    }
}