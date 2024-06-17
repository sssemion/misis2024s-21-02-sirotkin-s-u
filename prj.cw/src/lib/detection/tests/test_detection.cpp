#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../detection.h"


TEST_SUITE("Test detection lib") {

    TEST_CASE("Test red signs") {

        struct TestParams {
            std::string filename;
            int expected_circles, expected_triangles;
        };

        TestParams params;

        SUBCASE("Yeld") { params = { "yeld.jpeg", 0, 1 }; }
        SUBCASE("Railway cross") { params = { "railway_cross.jpeg", 0, 1 }; }
        SUBCASE("Speed limit") { params = { "speed_limit_and_bump.jpeg", 1, 0 }; }
        SUBCASE("Speed limit (temporary)") { params = { "speed_limit_temporary.jpeg", 1, 0 }; }
        SUBCASE("Many cautions") { params = { "6_cautions.jpeg", 0, 6 }; }
        SUBCASE("Forbid") { params = { "1_forbid.jpg", 1, 0 }; }

        CAPTURE(params);

        std::string filename = "data/" + params.filename;
        cv::Mat img = cv::imread(filename);
        REQUIRE_FALSE_MESSAGE(img.empty(), "File does not exist");

        Detection detector(img);

        cv::Mat mask = detector.getMask(Detection::RED_RANGES);

        cv::imshow("asas", mask);
        detector.filterMask(mask);

        auto circles = detector.getCircles(mask);
        auto triangles = detector.getTriangles(detector.getContours(mask));

        CHECK(circles.size() == params.expected_circles);
        CHECK(triangles.size() == params.expected_triangles);

        for (int i = 0; i < circles.size(); ++i) {
            cv::Vec3f circle = circles[i];
            cv::Point center = cv::Point(cvRound(circle[0]), cvRound(circle[1]));
            int radius = cvRound(circle[2]);
            cv::circle(img, center, radius, cv::Scalar(0, 255, 0), 2);
        }
        cv::drawContours(img, triangles, -1, (0, 255, 255), 2);

//        cv::imshow("asas", img);
        cv::waitKey(0);
    }
}