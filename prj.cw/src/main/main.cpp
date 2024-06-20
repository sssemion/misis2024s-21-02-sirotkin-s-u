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


void handleRedSigns(Detection &detector, Visualization &visualizer) {
    auto red_mask = detector.getMask(Detection::RED_RANGES);
    detector.filterMask(red_mask);
    auto contours = detector.getContours(red_mask);
    auto circles = detector.getCircles(red_mask);

    for (auto &circle: circles) {
        cv::Point top_left((int) (circle[0] - circle[2]), (int) (circle[1] - circle[2]));
        cv::Point bottom_right((int) (circle[0] + circle[2]), (int) (circle[1] + circle[2]));
        visualizer.drawSign(top_left, bottom_right, "Prohibiting", {0, 0, 255});
    }

    auto triangles = detector.getTriangles(contours);
    for (auto &triangle: triangles) {
        cv::Point top_left(
                std::min(triangle.p1.x, std::min(triangle.p2.x, triangle.p3.x)),
                std::min(triangle.p1.y, std::min(triangle.p2.y, triangle.p3.y))
        );
        cv::Point bottom_right(
                std::max(triangle.p1.x, std::max(triangle.p2.x, triangle.p3.x)),
                std::max(triangle.p1.y, std::max(triangle.p2.y, triangle.p3.y))
        );
        if (triangle.orientation == Triangle::NORMAL)
            visualizer.drawSign(top_left, bottom_right, "Warning", {0, 255, 255});
        else
            visualizer.drawSign(top_left, bottom_right, "Yeld", {255, 127, 255});
    }
}


int main(int argc, char **argv) {
    Args args(argc, argv);

    cv::Mat img = cv::imread(args.src);
    Detection detector(img);
    Visualization visualizer(img);
    handleRedSigns(detector, visualizer);
    if (args.dst.empty()) {
        cv::imshow("Traffic signs", img);
        cv::waitKey(0);
    } else {
        cv::imwrite(args.dst, img);
    }
}