#include "opencv2/opencv.hpp"
#include "CLI/CLI.hpp"
#include "../lib/detection/detection.h"
#include "../lib/visualization/visualization.h"

class Args {

public:
    std::string src;
    std::string dst;

    Args(int argc, char **argv) : argc{argc}, argv{argv} {
        parseArgs();
    }

private:
    int argc;
    char **argv;

    void parseArgs() {
        CLI::App app;
        app.add_option("--src", src, "Source image filename")
                ->required()->check(CLI::Validator(CLI::ExistingFile));
        app.add_option("--dst", dst, "Result filename");

        app.parse(argc, argv);
    }
};


int main(int argc, char **argv) {
    Args args(argc, argv);

    cv::Mat img = cv::imread(args.src);
    Detection detector(img);
    auto traffic_signs = detector.proceed();
    Visualization visualizer(img);
    for (auto& sign : traffic_signs) {
        visualizer.drawSign(sign);
    }
    if (args.dst.empty()) {
        cv::imshow("Traffic signs", img);
        cv::waitKey(0);
    } else {
        cv::imwrite(args.dst, img);
    }
}