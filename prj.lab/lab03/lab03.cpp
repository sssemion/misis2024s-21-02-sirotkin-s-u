#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "CLI/CLI.hpp"

class Args {

public:
    double black_q = 0.02;
    double white_q = 0.02;
    std::string filename;

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
        app.parse(argc, argv);
    }
};

int main(int argc, char **argv) {
    Args args(argc, argv);
    
    return 0;
}