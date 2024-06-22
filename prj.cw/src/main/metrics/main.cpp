#include "CLI/CLI.hpp"
#include "opencv2/opencv.hpp"
#include "../../lib/detection/detection.h"

class Args {

public:
    std::string data_dir;

    Args(int argc, char **argv) : argc{argc}, argv{argv} {
        parseArgs();
    }

private:
    int argc;
    char **argv;

    void parseArgs() {
        CLI::App app;
        app.add_option("--data-dir", data_dir, "Directory with test files")
                ->required()->check(CLI::Validator(CLI::ExistingDirectory));

        app.parse(argc, argv);
    }
};

struct Test {
    std::string filename;
    enum {
        PROHIBITING = 0,
        WARNING = 1,
        YIELD = 2,
    } expected_sign;
};

const std::vector<Test> files = {
        {"autosave01_02_2012_09_18_38.jpg", Test::PROHIBITING},
        {"autosave01_02_2012_09_19_16.jpg", Test::YIELD},
        {"autosave01_02_2012_10_00_52.jpg", Test::YIELD},
        {"autosave01_02_2012_10_13_37.jpg", Test::WARNING},
        {"autosave01_02_2012_10_15_27.jpg", Test::WARNING},
        {"autosave01_02_2012_10_23_50.jpg", Test::YIELD},
        {"autosave01_02_2012_10_24_56.jpg", Test::WARNING},
        {"autosave09_10_2012_09_42_23_0.jpg", Test::WARNING},
        {"autosave10_10_2012_10_01_46_2.jpg", Test::PROHIBITING},
        {"autosave16_04_2013_15_32_22_3.jpg", Test::WARNING},
        {"autosave16_04_2013_15_36_52_2.jpg", Test::PROHIBITING},
        {"autosave16_10_2012_11_48_48_1.jpg", Test::YIELD},
        {"autosave21_01_2013_12_18_41_1.jpg", Test::WARNING},
        {"yield.jpeg", Test::YIELD},
        {"railway_cross.jpeg", Test::WARNING},
        {"speed_limit_temporary.jpeg", Test::PROHIBITING},
        {"1654711661_16-celes-club-p-dorozhnie-znaki-oboi-krasivie-31.jpg", Test::PROHIBITING},
};

int main(int argc, char **argv) {
    Args args(argc, argv);

    int false_positive = 0;
    int correct_detected = 0;
    int correct_classified = 0;

    for (auto test: files) {
        cv::Mat img = cv::imread(args.data_dir + "/" + test.filename);
        Detection detector(img);
        auto traffic_signs = detector.proceed();
        if (traffic_signs.size() == 1) {
            ++correct_detected;
            if (traffic_signs[0].type == test.expected_sign)
                ++correct_classified;
        } else {
            bool found = false;
            for (auto sign : traffic_signs) {
                if (sign.type == test.expected_sign) {
                    ++correct_detected;
                    ++correct_classified;
                    found = true;
                    break;
                }
            }
            if (found) {
                false_positive += traffic_signs.size() - 1;
            } else {
                false_positive += traffic_signs.size();
            }
        }
    }

    printf("Detection accuracy: %.2f %%\n", correct_detected / (double) files.size() * 100);
    printf("Classification accuracy: %.2f %%\n", correct_classified / (double) correct_detected * 100);
    printf("False positive per image: %.2f\n", false_positive / (double) files.size());
}
