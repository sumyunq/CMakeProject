// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-23 16:44:36 +08:00 wbliu
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QString>

#include <opencv2/core.hpp>
#include <opencv2/geometry.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
constexpr int kSide = 96;
constexpr double kTolerance = 1e-5;
/// \brief 数值契约失败时终止实验，禁止只凭图片判断成功。
void require(bool condition, const char* message)
{
    if (!condition) { throw std::runtime_error(message); }
}
/// \brief 检查编码与目录写入结果。
void save(const QString& directory, const char* name, const cv::Mat& image)
{
    require(!image.empty(), "empty output");
    require(cv::imwrite(QDir(directory).filePath(QString::fromLatin1(name)).toStdString(), image), "imwrite failed");
}
/// \brief 创建有明确面积的二值矩形。
cv::Mat rectangleImage()
{
    cv::Mat image = cv::Mat::zeros(kSide, kSide, CV_8U);
    image(cv::Rect(24, 24, 40, 30)).setTo(255);
    return image;
}
/// \brief 验证非连续 ROI 行遍历与 BGR 灰度转换。
void case03(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat image(4, 6, CV_8UC3, cv::Scalar(10, 20, 30));
    auto roi = image(cv::Rect(1, 1, 3, 2));
    require(!roi.isContinuous(), "ROI must retain parent stride");
    int sum = 0;
    for (int y = 0; y < roi.rows; ++y) {
        const auto* row = roi.ptr<cv::Vec3b>(y);
        for (int x = 0; x < roi.cols; ++x) { sum += row[x][2]; }
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    require(sum == 180 && gray.at<uchar>(0, 0) == 22, "pixel prediction");
    save(directory, "bgr.png", image); save(directory, "gray.png", gray);
    std::cout << "red sum=180 gray=22 ROI continuous=false\n";
}
/// \brief 展示饱和算术、浮点中间值和直方图计数。
void case04(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat input = cv::Mat_<uchar>({1, 4}, {0, 10, 250, 255});
    cv::Mat added; cv::add(input, cv::Scalar(20), added);
    require(added.at<uchar>(0, 2) == 255, "saturation");
    cv::Mat floating; input.convertTo(floating, CV_32F); floating += 20;
    require(floating.at<float>(0, 2) == 270, "float range");
    const int channel = 0; const int bins = 256; const float range[] = {0, 256};
    const float* ranges[] = {range}; cv::Mat hist;
    cv::calcHist(&input, 1, &channel, cv::Mat(), hist, 1, &bins, ranges);
    require(cv::sum(hist)[0] == 4 && hist.at<float>(255) == 1, "histogram mass");
    save(directory, "input.png", input); save(directory, "added.png", added);
    std::cout << "8U: 20 30 255 255; float:20 30 270 275; histogram sum=4\n";
}
/// \brief 检查严格大于阈值的语义与 Otsu 双峰分割。
void case05(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat input = cv::Mat_<uchar>({1, 6}, {0, 40, 40, 200, 200, 255});
    cv::Mat fixed; cv::threshold(input, fixed, 40, 255, cv::THRESH_BINARY);
    require(cv::countNonZero(fixed) == 3 && fixed.at<uchar>(0, 1) == 0, "threshold equality");
    cv::Mat otsu; const double threshold = cv::threshold(input, otsu, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    require(threshold == 40 && cv::countNonZero(otsu) == 3, "otsu bimodal");
    cv::Mat constant(7, 7, CV_8U, cv::Scalar(40)); cv::Mat adaptive;
    cv::adaptiveThreshold(constant, adaptive, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 3, 2);
    require(cv::countNonZero(adaptive) == 49, "local mean minus C");
    save(directory, "fixed.png", fixed); save(directory, "otsu.png", otsu); save(directory, "adaptive.png", adaptive);
    std::cout << "threshold=40 foreground=3 adaptive constant foreground=49\n";
}
/// \brief 对比边界填充与带符号滤波输出。
void case06(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat input(5, 5, CV_8U, cv::Scalar(90)); cv::Mat reflected, zero;
    cv::blur(input, reflected, cv::Size(3, 3), cv::Point(-1, -1), cv::BORDER_REFLECT_101);
    cv::blur(input, zero, cv::Size(3, 3), cv::Point(-1, -1), cv::BORDER_CONSTANT);
    require(reflected.at<uchar>(0, 0) == 90 && zero.at<uchar>(0, 0) == 40, "border prediction");
    cv::Mat impulse = cv::Mat::zeros(9, 9, CV_8U); impulse.at<uchar>(4, 4) = 255;
    cv::Mat gaussian, median; cv::GaussianBlur(impulse, gaussian, cv::Size(3, 3), 0); cv::medianBlur(impulse, median, 3);
    require(gaussian.at<uchar>(4, 4) == 64 && cv::countNonZero(median) == 0, "impulse response");
    save(directory, "reflect.png", reflected); save(directory, "zero.png", zero); save(directory, "gaussian.png", gaussian); save(directory, "median.png", median);
    std::cout << "corner reflect=90 zero=40; gaussian impulse center=64 median=0\n";
}
/// \brief 验证腐蚀膨胀面积与开运算去除孤点。
void case07(const QString& directory)
{
    CVLAB_TRACE();
    auto image = rectangleImage(); const auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat eroded, dilated, opened; cv::erode(image, eroded, kernel); cv::dilate(image, dilated, kernel);
    require(cv::countNonZero(eroded) == 38 * 28 && cv::countNonZero(dilated) == 42 * 32, "morphology area");
    image.at<uchar>(4, 4) = 255; cv::morphologyEx(image, opened, cv::MORPH_OPEN, kernel);
    require(cv::countNonZero(opened) == 1200, "opening isolated pixel");
    save(directory, "noisy.png", image); save(directory, "eroded.png", eroded); save(directory, "dilated.png", dilated); save(directory, "opened.png", opened);
    std::cout << "eroded=1064 dilated=1344 opened=1200\n";
}
/// \brief 验证 Sobel 增益、符号与 Canny 非空边缘。
void case08(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat ramp(9, 9, CV_8U);
    for (int y = 0; y < ramp.rows; ++y) { for (int x = 0; x < ramp.cols; ++x) { ramp.at<uchar>(y, x) = static_cast<uchar>(x * 10); } }
    cv::Mat gradient; cv::Sobel(ramp, gradient, CV_32F, 1, 0, 3);
    require(gradient.at<float>(4, 4) == 80 && gradient.at<float>(4, 0) == 0, "sobel ramp");
    cv::Mat edges; cv::Canny(rectangleImage(), edges, 50, 100);
    require(cv::countNonZero(edges) > 0, "rectangle edges");
    cv::Mat constant(9, 9, CV_8U, cv::Scalar(100)), emptyEdges; cv::Canny(constant, emptyEdges, 50, 100);
    require(cv::countNonZero(emptyEdges) == 0, "constant has no edge");
    cv::Mat view; cv::convertScaleAbs(gradient, view); save(directory, "sobel.png", view); save(directory, "canny.png", edges);
    std::cout << "Sobel interior=80 border=0; constant Canny=0\n";
}
/// \brief 区分连通像素面积与轮廓多边形面积，并检测合成直线。
void case09(const QString& directory)
{
    CVLAB_TRACE();
    auto image = rectangleImage(); cv::Mat labels, stats, centers;
    require(cv::connectedComponentsWithStats(image, labels, stats, centers) == 2, "component count");
    require(stats.at<int>(1, cv::CC_STAT_AREA) == 1200, "pixel area");
    std::vector<std::vector<cv::Point>> contours; cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    require(contours.size() == 1 && cv::contourArea(contours.front()) == 1131, "polygon area");
    cv::Mat line = cv::Mat::zeros(kSide, kSide, CV_8U); cv::line(line, cv::Point(10, 40), cv::Point(85, 40), cv::Scalar(255));
    std::vector<cv::Vec4i> segments; cv::HoughLinesP(line, segments, 1, CV_PI / 180, 30, 40, 2);
    require(!segments.empty(), "hough segment");
    cv::Mat blank = cv::Mat::zeros(3, 3, CV_8U); cv::Mat blankLabels;
    require(cv::connectedComponents(blank, blankLabels) == 1, "background only");
    save(directory, "component.png", image); save(directory, "line.png", line);
    std::cout << "components including background=2; pixel area=1200 polygon area=1131; lines=" << segments.size() << '\n';
}
/// \brief 通过距离图和人工可验证种子解释分水岭标签。
void case10(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat mask = cv::Mat::zeros(kSide, kSide, CV_8U);
    cv::circle(mask, cv::Point(35, 48), 23, cv::Scalar(255), cv::FILLED); cv::circle(mask, cv::Point(62, 48), 23, cv::Scalar(255), cv::FILLED);
    cv::Mat distance; cv::distanceTransform(mask, distance, cv::DIST_L2, cv::DIST_MASK_PRECISE);
    require(distance.at<float>(0, 0) == 0 && distance.at<float>(48, 35) > 20, "distance seeds");
    cv::Mat color; cv::cvtColor(mask, color, cv::COLOR_GRAY2BGR);
    cv::Mat markers = cv::Mat::zeros(mask.size(), CV_32S); markers.setTo(1, mask == 0);
    cv::circle(markers, cv::Point(35, 48), 3, cv::Scalar(2), cv::FILLED); cv::circle(markers, cv::Point(62, 48), 3, cv::Scalar(3), cv::FILLED);
    cv::watershed(color, markers);
    require(markers.at<int>(48, 35) == 2 && markers.at<int>(48, 62) == 3 && cv::countNonZero(markers == -1) > 0, "watershed labels");
    cv::Mat distanceView; cv::normalize(distance, distanceView, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::Mat labelView; markers.convertTo(labelView, CV_8U, 60, 60);
    save(directory, "touching.png", mask); save(directory, "distance.png", distanceView); save(directory, "watershed.png", labelView);
    std::cout << "outside distance=0 centers retain labels 2,3 boundaries=-1\n";
}
/// \brief 验证前向矩阵的平移与透视单位矩阵。
void case11(const QString& directory)
{
    CVLAB_TRACE();
    auto image = rectangleImage(); cv::Mat shifted;
    const cv::Mat matrix = cv::Mat_<double>({2, 3}, {1, 0, 7, 0, 1, 5});
    cv::warpAffine(image, shifted, matrix, image.size(), cv::INTER_NEAREST);
    require(shifted.at<uchar>(29, 31) == 255 && shifted.at<uchar>(24, 24) == 0 && cv::countNonZero(shifted) == 1200, "translation");
    cv::Mat perspective; cv::warpPerspective(image, perspective, cv::Mat::eye(3, 3, CV_64F), image.size(), cv::INTER_LINEAR);
    require(cv::norm(image, perspective, cv::NORM_INF) == 0, "identity perspective");
    cv::Mat clipped; const cv::Mat outside = cv::Mat_<double>({2, 3}, {1, 0, 200, 0, 1, 0});
    cv::warpAffine(image, clipped, outside, image.size()); require(cv::countNonZero(clipped) == 0, "canvas clipping");
    save(directory, "source.png", image); save(directory, "shifted.png", shifted); save(directory, "perspective-identity.png", perspective);
    std::cout << "(24,24)->(31,29), area=1200 identity error=0 clipped area=0\n";
}
/// \brief 用归一化互相关匹配局部描述块，明确不冒充 ORB/SIFT。
void case12(const QString& directory)
{
    CVLAB_TRACE();
    cv::Mat image(60, 80, CV_8U); cv::RNG random(42); random.fill(image, cv::RNG::UNIFORM, 0, 180);
    const auto patch = image(cv::Rect(23, 17, 9, 9)).clone(); cv::Mat scores;
    cv::matchTemplate(image, patch, scores, cv::TM_CCOEFF_NORMED);
    double best = 0; cv::Point location; cv::minMaxLoc(scores, nullptr, &best, nullptr, &location);
    require(location == cv::Point(23, 17) && best > 0.999, "NCC exact match");
    cv::Mat brighter; image.convertTo(brighter, CV_8U, 1, 20); cv::matchTemplate(brighter, patch, scores, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(scores, nullptr, &best, nullptr, &location); require(location == cv::Point(23, 17) && best > 0.999, "NCC offset invariance");
    cv::Mat flat(9, 9, CV_8U, cv::Scalar(100)); cv::Scalar mean, deviation; cv::meanStdDev(flat, mean, deviation);
    require(deviation[0] == 0, "reject flat descriptor before NCC");
    cv::Mat scoreView; cv::normalize(scores, scoreView, 0, 255, cv::NORM_MINMAX, CV_8U);
    save(directory, "scene.png", image); save(directory, "patch.png", patch); save(directory, "scores.png", scoreView);
    std::cout << "NCC peak=(23,17) score>0.999; flat patch rejected by variance=0\n";
}
/// \brief 完整枚举最小平移假设的 RANSAC 共识与拼接示范。
void case13(const QString& directory)
{
    CVLAB_TRACE();
    const std::vector<cv::Point2d> displacement = {{7, 5}, {7.1, 5}, {6.9, 5}, {7, 5.1}, {40, -20}, {-30, 60}};
    std::vector<cv::Point2d> consensus;
    for (const auto& hypothesis : displacement) {
        std::vector<cv::Point2d> candidate;
        for (const auto& value : displacement) { if (cv::norm(value - hypothesis) < 0.5) { candidate.push_back(value); } }
        if (candidate.size() > consensus.size()) { consensus = candidate; }
    }
    require(consensus.size() == 4, "robust consensus"); cv::Point2d translation;
    for (const auto& value : consensus) { translation += value; }
    translation *= 1.0 / static_cast<double>(consensus.size());
    require(std::abs(translation.x - 7) < kTolerance && std::abs(translation.y - 5.025) < kTolerance, "consensus refit");
    require(displacement.size() - consensus.size() == 2, "outliers excluded");
    const auto source = rectangleImage(); cv::Mat warped;
    const cv::Mat matrix = cv::Mat_<double>({2, 3}, {1, 0, translation.x, 0, 1, translation.y});
    cv::warpAffine(source, warped, matrix, cv::Size(112, 112));
    cv::Mat canvas = cv::Mat::zeros(warped.size(), CV_8U); source.copyTo(canvas(cv::Rect(0, 0, source.cols, source.rows)));
    cv::Mat stitched; cv::max(canvas, warped, stitched);
    require(cv::countNonZero(stitched) >= 1200, "stitch canvas");
    save(directory, "warped.png", warped); save(directory, "stitched.png", stitched);
    std::cout << "inliers=4 outliers=2 translation=(7,5.025); model=translation only\n";
}
} // namespace
/// \brief 初始化日志、校验参数并运行独立合成实验，错误返回非零。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int result = 0;
    try {
        CVLAB_TRACE();
        require(argc == 3, "usage: foundation_labs <03..13> <outputdir>");
        bool valid = false; const int id = QString::fromLocal8Bit(argv[1]).toInt(&valid);
        require(valid && id >= 3 && id <= 13, "invalid lesson id");
        const auto directory = QString::fromLocal8Bit(argv[2]);
        require(!directory.isEmpty() && QDir().mkpath(directory), "cannot create output directory");
        switch (id) {
        case 3: case03(directory); break; case 4: case04(directory); break;
        case 5: case05(directory); break; case 6: case06(directory); break;
        case 7: case07(directory); break; case 8: case08(directory); break;
        case 9: case09(directory); break; case 10: case10(directory); break;
        case 11: case11(directory); break; case 12: case12(directory); break;
        case 13: case13(directory); break; default: throw std::runtime_error("invalid dispatch");
        }
        require(static_cast<bool>(std::cout), "stdout failed");
    } catch (const std::exception& error) {
        std::cerr << "Experiment failed: " << error.what() << '\n'; result = app::reportFailure("foundation-lab-failed");
    } catch (...) { result = app::reportFailure("foundation-lab-unknown-failure"); }
    return app::finishLogging(result);
}
