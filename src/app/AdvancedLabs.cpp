// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-23 16:44:36 +08:00 wbliu
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <QtCore/QCoreApplication>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry.hpp>

#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
constexpr double kTolerance = 1e-6;
/// \brief 将数值验收失败转为可见的非零退出。
void require(bool condition, const char* message)
{
    CVLAB_TRACE();
    if (!condition) { throw std::runtime_error(message); }
}
/// \brief 从显式数据建立拥有独立存储的矩阵，避免旧逗号初始化接口。
cv::Mat makeMatrix(int rows, int columns, const std::vector<double>& values)
{
    CVLAB_TRACE();
    require(values.size() == static_cast<std::size_t>(rows * columns), "matrix initializer shape");
    cv::Mat result(rows, columns, CV_64F);
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            result.at<double>(row, column) = values[static_cast<std::size_t>(row * columns + column)];
        }
    }
    return result;
}
/// \brief 对视差求深度，非有限值及零负视差必须拒绝。
std::optional<double> stereoDepth(double focal, double baseline, double disparity)
{
    CVLAB_TRACE();
    if (!std::isfinite(focal) || !std::isfinite(baseline) || !std::isfinite(disparity)
        || focal <= 0 || baseline <= 0 || disparity <= kTolerance) { return std::nullopt; }
    return focal * baseline / disparity;
}
/// \brief 验证平场校正的有效范围与饱和拒绝。
std::optional<double> correctFlat(double measured, double dark, double gain)
{
    CVLAB_TRACE();
    if (!std::isfinite(measured) || !std::isfinite(dark) || !std::isfinite(gain)
        || gain < 0.1 || measured >= 255.0 || measured < 0.0) { return std::nullopt; }
    return (measured - dark) / gain;
}

/// \brief 验证峰拟合的曲率与偏移范围。
std::optional<double> peakOffset(double left, double center, double right)
{
    CVLAB_TRACE();
    if (!std::isfinite(left) || !std::isfinite(center) || !std::isfinite(right)) {
        return std::nullopt;
    }
    const double denominator = left - 2 * center + right;
    if (denominator >= -1e-9) { return std::nullopt; }
    const double offset = 0.5 * (left - right) / denominator;
    if (std::abs(offset) > 0.5) { return std::nullopt; }
    return offset;
}

/// \brief 检查 PNG 写入及读回，拒绝将空输出视为成功。
void saveImage(const std::filesystem::path& path, const cv::Mat& image)
{
    CVLAB_TRACE();
    require(cv::imwrite(path.string(), image), "PNG write failed");
    const auto restored = cv::imread(path.string(), cv::IMREAD_UNCHANGED);
    require(!restored.empty() && restored.size() == image.size(), "PNG readback failed");
    require(cv::norm(restored, image, cv::NORM_INF) == 0.0, "PNG content mismatch");
}
/// \brief 用确定性合成数据验证十四至二十三课的核心数值关系。
void runExperiment(int lesson, const std::filesystem::path& output)
{
    CVLAB_TRACE();
    cv::Mat canvas(240, 400, CV_8UC3, cv::Scalar(24, 24, 24));
    std::ofstream report(output / "metrics.txt");
    require(report.is_open(), "report open failed");
    report << "baseline synthetic experiment; native camera/codec/DNN not tested\n";
    if (lesson == 14) {
        const cv::Mat x = makeMatrix(4, 2, {-0.2, 1, -0.1, 1, 0.1, 1, 0.2, 1});
        const cv::Mat u = makeMatrix(4, 1, {160, 240, 400, 480});
        cv::Mat parameters;
        require(cv::solve(x, u, parameters, cv::DECOMP_SVD), "calibration solve failed");
        require(std::abs(parameters.at<double>(0) - 800.0) < kTolerance, "focal length mismatch");
        require(std::abs(parameters.at<double>(1) - 320.0) < kTolerance, "principal point mismatch");
        const cv::Mat degenerate = cv::Mat::ones(4, 2, CV_64F);
        cv::SVD decomposition(degenerate);
        require(decomposition.w.at<double>(1) < kTolerance, "degeneracy undetected");
        report << "f=800 cx=320 residual=" << cv::norm(x * parameters, u) << " rank deficient case rejected\n";
        for (int index = 0; index < 4; ++index) {
            cv::circle(canvas, cv::Point(80 + 70 * index, 120), 5, cv::Scalar(0, 255, 0), -1);
        }
    } else if (lesson == 15) {
        constexpr double kFocal = 800.0;
        constexpr double kBaseline = 60.0;
        constexpr double kDisparity = 40.0;
        const auto computed = stereoDepth(kFocal, kBaseline, kDisparity);
        require(computed.has_value(), "positive disparity rejected");
        const double depth = *computed;
        const double perturbed = kFocal * kBaseline / (kDisparity + 0.5);
        require(std::abs(depth - 1200.0) < kTolerance && perturbed < depth, "triangulation mismatch");
        require(!stereoDepth(kFocal, kBaseline, 0.0) && !stereoDepth(kFocal, kBaseline, -1.0), "nonpositive disparity accepted");
        report << "Z_mm=" << depth << " Z_at_d40.5=" << perturbed << " d0 rejected\n";
        cv::line(canvas, {60, 200}, {200, 30}, {0, 255, 0}, 2);
        cv::line(canvas, {340, 200}, {200, 30}, {255, 128, 0}, 2);
    } else if (lesson == 16 || lesson == 17) {
        const std::vector<double> times{0.0, 0.04, 0.10, 0.14};
        double previousCenter = 0.0;
        double lastSpeed = 0.0;
        for (int index = 0; index < 4; ++index) {
            const auto position = static_cast<std::size_t>(index);
            const int center = 40 + static_cast<int>(500.0 * times[position]);
            cv::Mat frame = cv::Mat::zeros(120, 200, CV_8UC1);
            cv::circle(frame, {center, 60}, 8, cv::Scalar(255), -1);
            const auto moments = cv::moments(frame, true);
            require(moments.m00 > 0.0, "empty detection");
            const double observed = moments.m10 / moments.m00;
            if (index > 0) {
                const double dt = times[position] - times[position - 1];
                require(dt > 0.0, "timestamp not increasing");
                lastSpeed = (observed - previousCenter) / dt;
                require(std::abs(lastSpeed - 500.0) < kTolerance, "velocity mismatch");
            }
            report << index << " t=" << times[position] << " x=" << observed << " vx=" << lastSpeed << '\n';
            previousCenter = observed;
            saveImage(output / ("frame" + std::to_string(index) + ".png"), frame);
        }
        const cv::Mat missing = cv::Mat::zeros(30, 30, CV_8U);
        require(cv::moments(missing, true).m00 == 0.0, "missing observation guard failed");
        // 一维 Kalman 更新：先验方差4，测量方差1，增益0.8。
        constexpr double kGain = 4.0 / 5.0;
        const double corrected = 100.0 + kGain * (102.0 - 100.0);
        require(std::abs(corrected - 101.6) < kTolerance, "Kalman correction mismatch");
        report << "Kalman K=0.8 corrected=" << corrected << " missing measurement: predict only\n";
    } else if (lesson == 18) {
        cv::Mat field(120, 200, CV_64F);
        cv::Mat observed(120, 200, CV_64F);
        for (int row = 0; row < field.rows; ++row) {
            for (int column = 0; column < field.cols; ++column) {
                const double gain = 0.5 + static_cast<double>(column) / 400.0;
                field.at<double>(row, column) = gain;
                observed.at<double>(row, column) = 100.0 * gain + 10.0;
            }
        }
        cv::Mat corrected;
        cv::divide(observed - 10.0, field, corrected);
        require(cv::norm(corrected, cv::Mat(corrected.size(), CV_64F, cv::Scalar(100)), cv::NORM_INF) < kTolerance, "flat field failed");
        observed.convertTo(canvas, CV_8U);
        report << "dark=10 corrected min=max=100; saturation cannot be inverted\n";
        require(!correctFlat(255.0, 10.0, .5) && !correctFlat(60.0, 10.0, 0.0)
            && std::abs(correctFlat(60.0, 10.0, .5).value() - 100.0) < kTolerance, "saturation guard failed");
    } else if (lesson == 19) {
        constexpr double kLeft = 7.75;
        constexpr double kCenter = 9.75;
        constexpr double kRight = 9.75;
        const double denominator = kLeft - 2.0 * kCenter + kRight;
        require(std::abs(denominator) > kTolerance, "flat peak");
        const double offset = 0.5 * (kLeft - kRight) / denominator;
        require(std::abs(offset - 0.5) < kTolerance, "subpixel offset mismatch");
        report << "peak_offset=" << offset << " scale=.02mm/px sigma_px=.1 sigma_mm=.002\n";
        cv::line(canvas, {100, 180}, {200, 80}, {0, 255, 0}, 2);
        cv::line(canvas, {200, 80}, {300, 80}, {0, 255, 0}, 2);
        require(!peakOffset(1.0, 1.0, 1.0) && std::abs(peakOffset(kLeft, kCenter, kRight).value() - .5) < kTolerance, "flat denominator guard failed");
    } else if (lesson == 20) {
        cv::Mat reference(120, 200, CV_8U, cv::Scalar(100));
        auto sample = reference.clone();
        sample(cv::Rect(80, 40, 10, 10)).setTo(160);
        cv::Mat difference;
        cv::absdiff(sample, reference, difference);
        cv::threshold(difference, canvas, 20, 255, cv::THRESH_BINARY);
        require(cv::countNonZero(canvas) == 100, "defect area mismatch");
        constexpr double kPrecision = 8.0 / 10.0;
        constexpr double kRecall = 8.0 / 12.0;
        report << "defect_area=100 precision=" << kPrecision << " recall=" << kRecall << '\n';
        require(kPrecision > kRecall && kRecall > 0.66, "metrics mismatch");
        cv::Mat unchanged;
        cv::absdiff(reference, reference, unchanged);
        require(cv::countNonZero(unchanged) == 0, "negative defect mismatch");
    } else if (lesson == 21) {
        const cv::Mat features = makeMatrix(4, 2, {-2, 1, -1, 1, 1, 1, 2, 1});
        const cv::Mat labels = makeMatrix(4, 1, {-1, -1, 1, 1});
        cv::Mat weights;
        require(cv::solve(features, labels, weights, cv::DECOMP_SVD), "classifier fit failed");
        const cv::Mat predictions = features * weights;
        for (int row = 0; row < labels.rows; ++row) {
            require(predictions.at<double>(row) * labels.at<double>(row) > 0.0, "classifier sign mismatch");
        }
        require(std::abs(weights.at<double>(0) - 0.6) < kTolerance, "weight mismatch");
        const cv::Mat holdout = makeMatrix(3, 2, {-3, 1, 3, 1, 0, 1});
        const cv::Mat heldPredictions = holdout * weights;
        require(heldPredictions.at<double>(0) < 0 && heldPredictions.at<double>(1) > 0
            && std::abs(heldPredictions.at<double>(2)) < kTolerance, "holdout or boundary mismatch");
        report << "w=.6 bias=0 holdout x=-3 -> negative; x=3 -> positive; x=0 -> boundary reject\n";
        cv::line(canvas, {200, 20}, {200, 220}, {0, 255, 0}, 2);
    } else if (lesson == 22) {
        const cv::Vec3b bgr(10, 20, 30);
        const cv::Vec3f rgb(static_cast<float>(bgr[2]) / 255.0F, static_cast<float>(bgr[1]) / 255.0F, static_cast<float>(bgr[0]) / 255.0F);
        require(std::abs(static_cast<double>(rgb[0]) - 30.0 / 255.0) < kTolerance, "RGB conversion mismatch");
        constexpr int kShape[] = {1, 3, 2, 2};
        cv::Mat tensor(4, kShape, CV_32F, cv::Scalar(0));
        require(tensor.total() == 12 && tensor.dims == 4, "tensor shape mismatch");
        report << "NCHW=[1,3,2,2] elements=12 RGB=" << rgb << " scalar1x1conv=" << (2.0F * rgb[0] + 1.0F) << '\n';
        canvas.setTo(cv::Scalar(10, 20, 30));
        require(tensor.size[1] != 2, "NHWC confusion undetected");
    } else if (lesson == 23) {
        const cv::Rect first(40, 40, 80, 80);
        const cv::Rect second(50, 40, 80, 80);
        const double intersection = static_cast<double>((first & second).area());
        const double overlap = intersection / static_cast<double>(first.area() + second.area() - intersection);
        require(overlap > 0.5 && std::abs(overlap - 7.0 / 9.0) < kTolerance, "IoU mismatch");
        cv::rectangle(canvas, first, {0, 255, 0}, 2);
        cv::rectangle(canvas, second, {0, 0, 255}, 1);
        report << "IoU=" << overlap << " same-class .9/.8 NMS keeps .9; different-class keeps both\n";
        require((first & cv::Rect(300, 150, 10, 10)).area() == 0, "disjoint IoU failed");
        cv::Mat mask = cv::Mat::zeros(120, 200, CV_8U);
        mask(cv::Rect(40, 40, 20, 20)).setTo(255);
        require(cv::countNonZero(mask) == 400, "segmentation area failed");
        saveImage(output / "mask.png", mask);
    } else { throw std::runtime_error("lesson must be 14..23"); }
    saveImage(output / "result.png", canvas);
    report.flush();
    require(report.good(), "report write failed");
    std::cout << "lesson=" << lesson << " numeric assertions and PNG readback passed\n";
    require(std::cout.good(), "console write failed");
}
} // namespace
/// \brief 启动日志并为异常、非法参数、文件失败提供非零退出码。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    try {
        CVLAB_TRACE();
        require(argc == 3, "usage: advanced_labs <14..23> <output-directory>");
        std::size_t consumed = 0;
        const std::string argument(argv[1]);
        const int lesson = std::stoi(argument, &consumed);
        require(consumed == argument.size() && lesson >= 14 && lesson <= 23, "invalid lesson");
        const std::filesystem::path output(argv[2]);
        std::filesystem::create_directories(output);
        runExperiment(lesson, output);
    } catch (const std::exception& error) {
        std::cerr << "advanced-lab: " << error.what() << '\n';
        return app::finishLogging(app::reportFailure("advanced-lab-failed"));
    }
    return app::finishLogging(0);
}
