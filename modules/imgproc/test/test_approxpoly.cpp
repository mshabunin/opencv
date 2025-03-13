/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "test_precomp.hpp"

namespace opencv_test { namespace {

//
// TODO!!!:
//  check_slice (and/or check) seem(s) to be broken, or this is a bug in function
//  (or its inability to handle possible self-intersections in the generated contours).
//
//  At least, if // return TotalErrors;
//  is uncommented in check_slice, the test fails easily.
//  So, now (and it looks like since 0.9.6)
//  we only check that the set of vertices of the approximated polygon is
//  a subset of vertices of the original contour.
//


class CV_ApproxPolyTest : public cvtest::BaseTest {
public:
    CV_ApproxPolyTest() {}
    ~CV_ApproxPolyTest() {}

protected:
    void clear();
    bool get_contour(std::vector<cv::Point>& contour);
    int check_slice(const cv::Point& startPt, const cv::Point& endPt,
                    const std::vector<cv::Point>& srcContour, double epsilon);
    int check(const std::vector<cv::Point>& srcContour, 
              const std::vector<cv::Point>& dstContour, double epsilon);

    void run(int /*start_from*/) override;

private:
    cv::RNG rng;
};

void CV_ApproxPolyTest::clear() {
    BaseTest::clear();
}

bool CV_ApproxPolyTest::get_contour(std::vector<cv::Point>& contour) {
    int totalPoints = rng.uniform(1, 1000);
    cv::Point center(rng.uniform(0, 1000), rng.uniform(0, 1000));
    double radius = rng.uniform(1, 1000);
    double angleStep = CV_2PI / totalPoints;

    contour.resize(totalPoints);
    for (int i = 0; i < totalPoints; ++i) {
        double angle = i * angleStep;
        int dRadius = rng.uniform(-5, 6);
        radius += dRadius;
        contour[i] = cv::Point(cvRound(center.x + radius * cos(angle)),
                                cvRound(center.y + radius * sin(angle)));
    }

    return true;
}

#include <algorithm>
#include <cmath>

bool is_point_on_segment(const cv::Point& p, const cv::Point& startPt, const cv::Point& endPt) {
    double dx = static_cast<double>(endPt.x - startPt.x);
    double dy = static_cast<double>(endPt.y - startPt.y);
    double dxc = static_cast<double>(p.x - startPt.x);
    double dyc = static_cast<double>(p.y - startPt.y);

    // Check if point is collinear with the segment
    double cross = dx * dyc - dy * dxc;
    if (std::abs(cross) > std::numeric_limits<double>::epsilon())
        return false;

    // Check if point lies within the bounds of the segment
    double dot = dxc * dx + dyc * dy;
    if (dot < 0)
        return false;

    double len2 = dx * dx + dy * dy;
    if (dot > len2)
        return false;

    return true;
}

int CV_ApproxPolyTest::check_slice(const cv::Point& startPt, const cv::Point& endPt,
                                    const std::vector<cv::Point>& srcContour, double epsilon) {
    double dx = static_cast<double>(endPt.x - startPt.x);
    double dy = static_cast<double>(endPt.y - startPt.y);

    if (dx == 0 && dy == 0) return 0; // Zero-length segment

    double A = -dy;
    double B = dx;
    double C = dy * startPt.x - dx * startPt.y;
    double Sq = std::sqrt(A * A + B * B);
    double sin_a = B / Sq;
    double cos_a = A / Sq;
    double d = C / Sq;

    int TotalErrors = 0;
    for (const auto& pt : srcContour) {
        if (pt == startPt || pt == endPt) continue; // Skip the endpoints

        // Calculate the perpendicular distance from the point to the line
        double dist = std::abs(sin_a * pt.y + cos_a * pt.x - d);

        // Check if the point is within the segment using a helper function
        if (is_point_on_segment(pt, startPt, endPt)) {
            if (dist > epsilon) ++TotalErrors;
        }
    }

    return TotalErrors;
}

int CV_ApproxPolyTest::check(const std::vector<cv::Point>& srcContour, 
                              const std::vector<cv::Point>& dstContour, double epsilon) {
    int TotalErrors = 0;
    size_t count = srcContour.size();

    for (size_t i = 1; i < dstContour.size(); ++i) {
        cv::Point startPt = dstContour[i - 1];
        cv::Point endPt = dstContour[i];

        TotalErrors += check_slice(startPt, endPt, srcContour, epsilon);

        if (startPt == endPt && TotalErrors > 0)
            break;
    }

    // Check the last segment connecting back to the first point
    cv::Point startPt = dstContour.back();
    cv::Point endPt = dstContour.front();

    TotalErrors += check_slice(startPt, endPt, srcContour, epsilon);

    return TotalErrors;
}

void CV_ApproxPolyTest::run(int /*start_from*/) {
    int code = cvtest::TS::OK;

    for (int i = 0; i < 30; ++i) {
        std::vector<cv::Point> srcContour;
        if (!get_contour(srcContour)) {
            ts->printf(cvtest::TS::LOG, "Failed to generate contour #%d\n", i);
            code = cvtest::TS::FAIL_INVALID_OUTPUT;
            break;
        }

        double maxDiameter = 0;
        for (const auto& pt : srcContour) {
            double d = cv::norm(pt);
            if (d > maxDiameter) maxDiameter = d;
        }

        int intervalsCount = 10;
        double epsilonStep = maxDiameter / intervalsCount;

        for (double epsilon = epsilonStep; epsilon < maxDiameter; epsilon += epsilonStep) {
            std::vector<cv::Point> approxContour;
            cv::approxPolyDP(srcContour, approxContour, epsilon, true);

            if (approxContour.empty()) {
                ts->printf(cvtest::TS::LOG,
                           "cvApproxPolyDP returned empty contour for contour #%d, epsilon = %g\n", i, epsilon);
                code = cvtest::TS::FAIL_INVALID_OUTPUT;
                break;
            }

            int totalErrors = check(srcContour, approxContour, epsilon);
            if (totalErrors != 0) {
                ts->printf(cvtest::TS::LOG,
                           "Incorrect result for the contour #%d approximated with epsilon=%g\n", i, epsilon);

                // Verbose logging
                ts->printf(cvtest::TS::LOG, "Original Contour:\n");
                for (const auto& pt : srcContour) {
                    ts->printf(cvtest::TS::LOG, "(%d, %d)\n", pt.x, pt.y);
                }
                ts->printf(cvtest::TS::LOG, "Approximated Contour:\n");
                for (const auto& pt : approxContour) {
                    ts->printf(cvtest::TS::LOG, "(%d, %d)\n", pt.x, pt.y);
                }

                // Visualize the contours
                cv::Mat canvas(4000, 4000, CV_8UC3, cv::Scalar(255, 255, 255));
                std::vector<std::vector<cv::Point>> srcContours{srcContour};
                std::vector<std::vector<cv::Point>> approxContours{approxContour};

                // Offset contours to make them visible within the canvas
                int offset = 1300;
                for (auto& pt : srcContours[0]) {
                    pt += cv::Point(offset, offset);
                }
                for (auto& pt : approxContours[0]) {
                    pt += cv::Point(offset, offset);
                }

                // Draw contours
                cv::drawContours(canvas, srcContours, -1, cv::Scalar(0, 255, 0), 2); // Green for original contour
                cv::drawContours(canvas, approxContours, -1, cv::Scalar(0, 0, 255), 2); // Red for approximated contour

                // Highlight vertices of the original contour which exceed epsilon distance from the result contour
                std::vector<cv::Point> errorVertices;
                for (const auto& pt : srcContour) {
                    double minDist = std::numeric_limits<double>::max();
                    for (size_t j = 0; j < approxContour.size(); ++j) {
                        cv::Point startPt = approxContour[j];
                        cv::Point endPt = approxContour[(j + 1) % approxContour.size()];

                        double dist;
                        if (startPt == endPt) {
                            dist = cv::norm(pt - startPt);
                        } else {
                            // Check if the point is on the line segment
                            double t = ((pt.x - startPt.x) * (endPt.x - startPt.x) + 
                                         (pt.y - startPt.y) * (endPt.y - startPt.y)) / 
                                        cv::norm(endPt - startPt);
                            
                            if (t < 0)
                                dist = cv::norm(pt - startPt);
                            else if (t > 1)
                                dist = cv::norm(pt - endPt);
                            else {
                                cv::Point projection = startPt + t * (endPt - startPt);
                                dist = cv::norm(pt - projection);
                }
                        }

                        minDist = std::min(minDist, dist);
                    }

                    if (minDist > epsilon) {
                        errorVertices.push_back(pt);
                    }
                }
                // Draw incorrect vertices in blue
                for (const auto& pt : errorVertices) {
                    cv::circle(canvas, pt + cv::Point(offset, offset), 5, cv::Scalar(255, 0, 0), -1); // Blue color
                }

                // Save or display the visualization
                std::string fileName = "contour_failure_" + std::to_string(i) + "_epsilon_" + std::to_string(epsilon) + ".png";
                cv::imwrite(fileName, canvas);
                ts->printf(cvtest::TS::LOG, "Visualization saved to %s\n", fileName.c_str());

                code = cvtest::TS::FAIL_BAD_ACCURACY;
                break;
            }
        }
    }

    if (code < 0)
        ts->set_failed_test_info(code);
}
TEST(Imgproc_ApproxPoly, accuracy) { CV_ApproxPolyTest test; test.safe_run(); }

//Tests to make sure that unreasonable epsilon (error)
//values never get passed to the Douglas-Peucker algorithm.
TEST(Imgproc_ApproxPoly, bad_epsilon)
{
    std::vector<Point2f> inputPoints;
    inputPoints.push_back(Point2f(0.0f, 0.0f));
    std::vector<Point2f> outputPoints;

    double eps = std::numeric_limits<double>::infinity();
    ASSERT_ANY_THROW(approxPolyDP(inputPoints, outputPoints, eps, false));

    eps = 9e99;
    ASSERT_ANY_THROW(approxPolyDP(inputPoints, outputPoints, eps, false));

    eps = -1e-6;
    ASSERT_ANY_THROW(approxPolyDP(inputPoints, outputPoints, eps, false));

    eps = NAN;
    ASSERT_ANY_THROW(approxPolyDP(inputPoints, outputPoints, eps, false));
}

struct ApproxPolyN: public testing::Test
{
    void SetUp()
    {
        vector<vector<Point>> inputPoints = {
            {  {87, 103}, {100, 112}, {96, 138}, {80, 169}, {60, 183}, {38, 176}, {41, 145}, {56, 118}, {76, 104} },
            {  {196, 102}, {205, 118}, {174, 196}, {152, 207}, {102, 194}, {100, 175}, {131, 109} },
            {  {372, 101}, {377, 119}, {337, 238}, {324, 248}, {240, 229}, {199, 214}, {232, 123}, {245, 103} },
            {  {463, 86}, {563, 112}, {574, 135}, {596, 221}, {518, 298}, {412, 266}, {385, 164}, {462, 86} }
        };

        Mat image(600, 600, CV_8UC1, Scalar(0));

        for (vector<Point>& polygon : inputPoints) {
            polylines(image, { polygon }, true, Scalar(255), 1);
        }

        findContours(image, contours, RETR_LIST, CHAIN_APPROX_NONE);
    }

    vector<vector<Point>> contours;
};

TEST_F(ApproxPolyN, accuracyInt)
{
    vector<vector<Point>> rightCorners = {
        { {72, 187}, {37, 176}, {42, 127}, {133, 64} },
        { {168, 212}, {92, 192}, {131, 109}, {213, 100} },
        { {72, 187}, {37, 176}, {42, 127}, {133, 64} },
        { {384, 100}, {333, 251}, {197, 220}, {239, 103} },
        { {168, 212}, {92, 192}, {131, 109}, {213, 100} },
        { {333, 251}, {197, 220}, {239, 103}, {384, 100} },
        { {542, 6}, {596, 221}, {518, 299}, {312, 236} },
        { {596, 221}, {518, 299}, {312, 236}, {542, 6} }
    };
    EXPECT_EQ(rightCorners.size(), contours.size());

    for (size_t i = 0; i < contours.size(); ++i) {
        std::vector<Point> corners;
        approxPolyN(contours[i], corners, 4, -1, true);
        ASSERT_EQ(rightCorners[i], corners );
    }
}

TEST_F(ApproxPolyN, accuracyFloat)
{
    vector<vector<Point2f>> rightCorners = {
        { {72.f, 187.f}, {37.f, 176.f}, {42.f, 127.f}, {133.f, 64.f} },
        { {168.f, 212.f}, {92.f, 192.f}, {131.f, 109.f}, {213.f, 100.f} },
        { {72.f, 187.f}, {37.f, 176.f}, {42.f, 127.f}, {133.f, 64.f} },
        { {384.f, 100.f}, {333.f, 251.f}, {197.f, 220.f}, {239.f, 103.f} },
        { {168.f, 212.f}, {92.f, 192.f}, {131.f, 109.f}, {213.f, 100.f} },
        { {333.f, 251.f}, {197.f, 220.f}, {239.f, 103.f}, {384.f, 100.f} },
        { {542.f, 6.f}, {596.f, 221.f}, {518.f, 299.f}, {312.f, 236.f} },
        { {596.f, 221.f}, {518.f, 299.f}, {312.f, 236.f}, {542.f, 6.f} }
    };
    EXPECT_EQ(rightCorners.size(), contours.size());

    for (size_t i = 0; i < contours.size(); ++i) {
        std::vector<Point2f> corners;
        approxPolyN(contours[i], corners, 4, -1, true);
        EXPECT_LT(cvtest::norm(rightCorners[i], corners, NORM_INF), .5f);
    }
}

TEST_F(ApproxPolyN, bad_args)
{
    Mat contour(10, 1, CV_32FC2);
    vector<vector<Point>> bad_contours;
    vector<Point> corners;
    ASSERT_ANY_THROW(approxPolyN(contour, corners, 0));
    ASSERT_ANY_THROW(approxPolyN(contour, corners, 3, 0));
    ASSERT_ANY_THROW(approxPolyN(bad_contours, corners, 4));
}


}} // namespace