// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-11-02 11:19:51 +08:00 wbliu
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <QtCore/QCoreApplication>

#include <opencv2/core.hpp>
#include <opencv2/calib.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/features.hpp>
#include <opencv2/geometry.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>

#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
/// \brief 在实验契约不满足时抛出明确错误，交由入口统一处理。
void require(bool condition, const char* reason)
{
    CVLAB_TRACE();
    if (!condition) { throw std::runtime_error(reason); }
}

/// \brief 加载许可和哈希已登记的真实图片，拒绝缺失或错误类型。
cv::Mat loadImage(const std::filesystem::path& path, int flag)
{
    CVLAB_TRACE();
    require(std::filesystem::is_regular_file(path), "sample file missing");
    cv::Mat image = cv::imread(path.string(), flag);
    require(!image.empty() && image.dims == 2, "cannot decode sample image");
    return image;
}

/// \brief 检查真实彩色和灰度图像的读取、写回和错误输入。
void imageExperiment(const std::filesystem::path& fixtures, const std::filesystem::path& output)
{
    CVLAB_TRACE();
    const auto color = loadImage(fixtures / "astronaut.png", cv::IMREAD_COLOR);
    const auto gray = loadImage(fixtures / "cell.png", cv::IMREAD_GRAYSCALE);
    require(color.type() == CV_8UC3 && color.size() == cv::Size(512, 512), "color shape or type");
    require(gray.type() == CV_8UC1 && gray.size() == cv::Size(550, 660), "gray shape or type");
    const auto path = output / "astronaut-copy.png";
    require(cv::imwrite(path.string(), color), "cannot write image");
    const auto restored = loadImage(path, cv::IMREAD_COLOR);
    require(cv::norm(color, restored, cv::NORM_INF) == 0, "image round-trip mismatch");
    bool rejected = false;
    try { static_cast<void>(loadImage(fixtures / "missing-input.png", cv::IMREAD_COLOR)); }
    catch (const std::exception&) { rejected = true; }
    require(rejected, "missing image accepted");
    std::cout << "color=512x512x3 cell=550x660x1 PNG max_error=0 missing=rejected\n";
}

/// \brief 使用真实纹理与可知平移验证ORB匹配和单应估计的重投影误差。
void registrationExperiment(const std::filesystem::path& fixtures, const std::filesystem::path& output)
{
    CVLAB_TRACE();
    const auto color = loadImage(fixtures / "astronaut.png", cv::IMREAD_COLOR);
    cv::Mat gray;
    cv::cvtColor(color, gray, cv::COLOR_BGR2GRAY);
    const cv::Matx23d transform(1, 0, 7, 0, 1, 5);
    cv::Mat shifted;
    cv::warpAffine(gray, shifted, transform, gray.size());
    const auto detector = cv::ORB::create(1000);
    std::vector<cv::KeyPoint> originalPoints, shiftedPoints;
    cv::Mat originalDescriptors, shiftedDescriptors;
    detector->detectAndCompute(gray, cv::noArray(), originalPoints, originalDescriptors);
    detector->detectAndCompute(shifted, cv::noArray(), shiftedPoints, shiftedDescriptors);
    require(!originalDescriptors.empty() && !shiftedDescriptors.empty(), "no ORB descriptors");
    const auto matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> neighbors;
    matcher->knnMatch(originalDescriptors, shiftedDescriptors, neighbors, 2);
    std::vector<cv::Point2f> source, destination;
    for (const auto& pair : neighbors) {
        if (pair.size() == 2 && pair[0].distance < 0.75F * pair[1].distance) {
            source.push_back(originalPoints.at(static_cast<std::size_t>(pair[0].queryIdx)).pt);
            destination.push_back(shiftedPoints.at(static_cast<std::size_t>(pair[0].trainIdx)).pt);
        }
    }
    require(source.size() >= 20, "insufficient feature matches");
    cv::setRNGSeed(42);
    cv::Mat inliers;
    const cv::Mat homography = cv::findHomography(source, destination, cv::RANSAC, 2.5, inliers);
    require(!homography.empty() && cv::countNonZero(inliers) >= 20, "homography estimation failed");
    const std::vector<cv::Point2f> points{{100,100},{400,100},{100,400},{400,400}};
    std::vector<cv::Point2f> projected;
    cv::perspectiveTransform(points, projected, homography);
    double maximum = 0;
    for (std::size_t i = 0; i < points.size(); ++i) {
        maximum = std::max(maximum, cv::norm(projected[i] - points[i] - cv::Point2f(7,5)));
    }
    require(std::isfinite(maximum) && maximum < 2.0, "registration exceeds two pixels");
    require(cv::imwrite((output / "astronaut-shifted.png").string(), shifted), "cannot write shifted image");
    std::cout << "matches=" << source.size() << " inliers=" << cv::countNonZero(inliers)
              << " max_error_px=" << maximum << " expected_translation=(7,5)\n";
}

/// \brief 对真实照片衍生的视频逐帧解码，检查帧数和可知裁剪产生的光流。
void videoExperiment(const std::filesystem::path& fixtures)
{
    CVLAB_TRACE();
    cv::VideoCapture video((fixtures / "astronaut-motion.avi").string(), cv::CAP_OPENCV_MJPEG);
    require(video.isOpened(), "cannot open fixture video");
    require(std::abs(video.get(cv::CAP_PROP_FPS) - 10.0) < 0.01, "unexpected fixture FPS");
    cv::Mat first, next, frame;
    int count = 0;
    while (video.read(frame)) {
        require(frame.size() == cv::Size(256, 192), "unexpected frame dimensions");
        if (count == 0) { first = frame.clone(); }
        if (count == 1) { next = frame.clone(); }
        ++count;
    }
    require(count == 12 && !first.empty() && !next.empty(), "unexpected frame count");
    cv::Mat previousGray, nextGray;
    cv::cvtColor(first, previousGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(next, nextGray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Point2f> points, tracked;
    cv::goodFeaturesToTrack(previousGray, points, 150, 0.01, 8);
    std::vector<uchar> status;
    std::vector<float> errors;
    cv::calcOpticalFlowPyrLK(previousGray, nextGray, points, tracked, status, errors);
    int accepted = 0;
    double dx = 0, dy = 0;
    for (std::size_t i = 0; i < points.size(); ++i) {
        if (status[i] && errors[i] < 20.0F) { dx += tracked[i].x - points[i].x; dy += tracked[i].y - points[i].y; ++accepted; }
    }
    require(accepted >= 20, "not enough reliable flow points");
    dx /= accepted; dy /= accepted;
    require(std::abs(dx + 2.0) < 0.6 && std::abs(dy) < 0.6, "unexpected frame motion");
    cv::VideoCapture missing((fixtures / "missing-video.avi").string(), cv::CAP_OPENCV_MJPEG);
    require(!missing.isOpened(), "missing video accepted");
    std::cout << "frames=" << count << " fps=10 accepted=" << accepted
              << " mean_dx=" << dx << " mean_dy=" << dy << " expected=(-2,0)\n";
}

/// \brief 在真实纹理上注入已知缺陷并测量检出率和误报，不声称真实工件精度。
void inspectionExperiment(const std::filesystem::path& fixtures, const std::filesystem::path& output)
{
    CVLAB_TRACE();
    const auto reference = loadImage(fixtures / "cell.png", cv::IMREAD_GRAYSCALE);
    cv::Mat truth(reference.size(), CV_8UC1, cv::Scalar(0));
    cv::circle(truth, cv::Point(275, 330), 10, cv::Scalar(255), cv::FILLED);
    auto sample = reference.clone();
    for (int y = 0; y < sample.rows; ++y) {
        auto* destination = sample.ptr<uchar>(y);
        const auto* selected = truth.ptr<uchar>(y);
        for (int x = 0; x < sample.cols; ++x) {
            if (selected[x] != 0) { destination[x] = static_cast<uchar>(destination[x] <= 175 ? destination[x] + 80 : destination[x] - 80); }
        }
    }
    cv::Mat difference, detected, intersection;
    cv::absdiff(reference, sample, difference);
    cv::threshold(difference, detected, 40, 255, cv::THRESH_BINARY);
    cv::bitwise_and(detected, truth, intersection);
    const int tp = cv::countNonZero(intersection);
    const int predicted = cv::countNonZero(detected);
    const int actual = cv::countNonZero(truth);
    require(actual == 317 && tp == actual && predicted == actual, "synthetic defect metrics mismatch");
    cv::Mat unchanged;
    cv::absdiff(reference, reference, unchanged);
    require(cv::countNonZero(unchanged) == 0, "unchanged image not clean");
    require(cv::imwrite((output / "inspection-sample.png").string(), sample)
        && cv::imwrite((output / "inspection-mask.png").string(), detected)
        && cv::imwrite((output / "inspection-truth.png").string(), truth), "cannot write inspection output");
    std::cout << "synthetic_defect_on_real_texture TP=" << tp << " FP=" << predicted-tp
              << " FN=" << actual-tp << " pixel_precision=1 pixel_recall=1\n";
}

/// \brief 用真实模型逐帧分类并估计真实纹理的合成运动，不假装输出检测框。
void videoAiExperiment(const std::filesystem::path& fixtures, const std::filesystem::path& output,
                       const std::filesystem::path& model)
{
    CVLAB_TRACE();
    require(std::filesystem::is_regular_file(model), "model file missing");
    cv::VideoCapture video((fixtures / "astronaut-motion.avi").string(), cv::CAP_OPENCV_MJPEG);
    require(video.isOpened(), "cannot open video");
    auto net = cv::dnn::readNetFromONNX(model.string());
    require(!net.empty(), "cannot load model");
    std::ofstream report(output / "video-ai.csv");
    require(report.is_open(), "cannot open report");
    report << "frame,top_class,score,mean_dx,mean_dy,tracked_points\n";
    constexpr float kMean[] = {0.485F, 0.456F, 0.406F};
    constexpr float kStd[] = {0.229F, 0.224F, 0.225F};
    cv::Mat frame, previousGray;
    int frames = 0;
    while (video.read(frame)) {
        require(frame.size() == cv::Size(256, 192), "unexpected frame dimensions");
        cv::Mat rgb, resized, gray;
        cv::resize(frame, resized, cv::Size(224, 224));
        cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);
        rgb.convertTo(rgb, CV_32FC3, 1.0 / 255.0);
        std::vector<cv::Mat> channels;
        cv::split(rgb, channels);
        for (std::size_t i = 0; i < channels.size(); ++i) { channels[i] = (channels[i] - kMean[i]) / kStd[i]; }
        cv::merge(channels, rgb);
        const cv::Mat tensor = cv::dnn::blobFromImage(rgb);
        net.setInput(tensor);
        const cv::Mat logits = net.forward();
        require(logits.type() == CV_32F && logits.dims == 2 && logits.rows == 1
            && logits.cols == 1000 && cv::checkRange(logits), "invalid model output");
        const cv::Mat scores = logits.reshape(1, 1);
        double maxValue = 0;
        cv::Point maxPosition;
        cv::minMaxLoc(scores, nullptr, &maxValue, nullptr, &maxPosition);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        double dx = 0, dy = 0;
        int accepted = 0;
        if (!previousGray.empty()) {
            std::vector<cv::Point2f> points, tracked;
            cv::goodFeaturesToTrack(previousGray, points, 150, 0.01, 8);
            std::vector<uchar> status;
            std::vector<float> errors;
            cv::calcOpticalFlowPyrLK(previousGray, gray, points, tracked, status, errors);
            for (std::size_t i = 0; i < points.size(); ++i) {
                if (status[i] && errors[i] < 20.0F) { dx += tracked[i].x - points[i].x; dy += tracked[i].y - points[i].y; ++accepted; }
            }
            require(accepted >= 20, "insufficient tracked features");
            dx /= accepted; dy /= accepted;
            require(std::abs(dx + 2.0) < 0.8 && std::abs(dy) < 0.8, "video motion mismatch");
        }
        report << frames << ',' << maxPosition.x << ',' << maxValue << ',' << dx << ',' << dy << ',' << accepted << '\n';
        previousGray = gray;
        ++frames;
    }
    require(frames == 12, "unexpected frame count");
    report.flush();
    require(static_cast<bool>(report), "cannot write video report");
    std::cout << "classified_frames=" << frames << " motion_pairs=" << frames-1
              << " backend=CPU model=SqueezeNet task=classification (not detection)\n";
}
/// \brief 用十二个已知姿态的平面点集验证完整标定接口；不代表实机标定。
void calibrationExperiment()
{
    CVLAB_TRACE();
    const cv::Mat intrinsic = cv::Mat(cv::Matx33d(800, 0, 320, 0, 820, 240, 0, 0, 1));
    std::vector<cv::Point3f> board;
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 9; ++x) { board.emplace_back(20.0F * x, 20.0F * y, 0); }
    }
    std::vector<std::vector<cv::Point3f>> objects;
    std::vector<std::vector<cv::Point2f>> pixels;
    for (int view = 0; view < 12; ++view) {
        const cv::Vec3d rotation(-0.25 + 0.05 * view, 0.15 - 0.035 * view, 0.02 * view);
        const cv::Vec3d translation(-80 + 4 * view, -50 + 2 * view, 500 + 18 * view);
        std::vector<cv::Point2f> projected;
        cv::projectPoints(board, rotation, translation, intrinsic, cv::Mat(), projected);
        objects.push_back(board);
        pixels.push_back(projected);
    }
    cv::Mat estimated, distortion;
    std::vector<cv::Mat> rotations, translations;
    const int flags = cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_FIX_K1 | cv::CALIB_FIX_K2 | cv::CALIB_FIX_K3;
    const double rms = cv::calibrateCamera(objects, pixels, cv::Size(640, 480), estimated,
        distortion, rotations, translations, flags);
    require(std::isfinite(rms) && rms < 0.001, "calibration reprojection error");
    require(cv::norm(estimated, intrinsic, cv::NORM_INF) < 0.1, "calibration intrinsic mismatch");
    // 空观测应被接口拒绝；不将某个退化数据恰好返回结果解释为成功。
    bool rejected = false;
    try {
        const std::vector<std::vector<cv::Point3f>> emptyObjects;
        const std::vector<std::vector<cv::Point2f>> emptyPixels;
        cv::calibrateCamera(emptyObjects, emptyPixels, cv::Size(640, 480), estimated,
            distortion, rotations, translations, flags);
    } catch (const cv::Exception&) { rejected = true; }
    require(rejected, "empty calibration accepted");
    std::cout << "synthetic_views=12 points_per_view=54 rms_px=" << rms
              << " intrinsic_max_error<0.1 empty=rejected (not hardware calibration)\n";
}
} // namespace

/// \brief 建立日志上下文并分派真实输入实验，不静默忽略失败。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int result = 0;
    try {
        CVLAB_TRACE();
        require(argc == 4 || argc == 5, "usage: real_image_labs <mode> <fixtures> <output> [model]");
        const std::string experiment(argv[1]);
        require(experiment == "videoai" ? argc == 5 : argc == 4, "incorrect number of arguments");
        const std::filesystem::path fixtures(argv[2]), output(argv[3]);
        std::filesystem::create_directories(output);
        if (experiment == "image") { imageExperiment(fixtures, output); }
        else if (experiment == "registration") { registrationExperiment(fixtures, output); }
        else if (experiment == "video") { videoExperiment(fixtures); }
        else if (experiment == "inspection") { inspectionExperiment(fixtures, output); }
        else if (experiment == "calibration") { calibrationExperiment(); }
        else if (experiment == "videoai") { videoAiExperiment(fixtures, output, argv[4]); }
        else { throw std::invalid_argument("unknown real image experiment"); }
        require(static_cast<bool>(std::cout), "cannot write console output");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        result = app::reportFailure("real-image-lab-failed");
    }
    return app::finishLogging(result);
}
