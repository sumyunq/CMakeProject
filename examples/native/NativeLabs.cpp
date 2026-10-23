// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-23 16:44:36 +08:00 wbliu
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <QtCore/QCoreApplication>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/features.hpp>
#include <opencv2/geometry.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>
#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
/// \brief 将输入或数值契约失败转为可定位异常。
void require(bool condition, const char* reason)
{
    CVLAB_TRACE();
    if (!condition) { throw std::runtime_error(reason); }
}
/// \brief 用固定随机纹理验证真正的 ORB 描述子与汉明距离匹配。
void features()
{
    CVLAB_TRACE();
    cv::Mat image(240, 320, CV_8UC1);
    cv::RNG random(420);
    random.fill(image, cv::RNG::UNIFORM, 0, 256);
    const auto detector = cv::ORB::create();
    std::vector<cv::KeyPoint> first, second;
    cv::Mat left, right;
    detector->detectAndCompute(image, cv::noArray(), first, left);
    detector->detectAndCompute(image.clone(), cv::noArray(), second, right);
    require(!left.empty() && left.type() == CV_8U, "orb-descriptor-contract");
    const auto matcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches;
    matcher->match(left, right, matches);
    require(matches.size() >= 50, "orb-too-few-matches");
    require(std::all_of(matches.begin(), matches.end(), [](const auto& m) { return m.distance == 0; }), "identity-match-distance");
    std::cout << "ORB identity matches=" << matches.size() << " distance=0\n";
}
/// \brief 投影已知三维目标并由 PnP 恢复位姿，以重投影误差验收。
void camera()
{
    CVLAB_TRACE();
    const cv::Matx33d intrinsic(800, 0, 320, 0, 800, 240, 0, 0, 1);
    const std::vector<cv::Point3f> object{{0,0,0},{1,0,0},{0,1,0},{1,1,0},{0,0,1},{1,0,1},{0,1,1},{1,1,1}};
    const cv::Vec3d rotation(0.1, -0.1, 0.05), translation(0.1, -0.2, 5.0);
    std::vector<cv::Point2f> image, recovered;
    cv::projectPoints(object, rotation, translation, intrinsic, cv::noArray(), image);
    cv::Mat r, t;
    require(cv::solvePnP(object, image, intrinsic, cv::noArray(), r, t), "pnp-failed");
    cv::projectPoints(object, r, t, intrinsic, cv::noArray(), recovered);
    const double error = cv::norm(image, recovered, cv::NORM_L2) / std::sqrt(static_cast<double>(object.size()));
    require(error < 0.001, "pnp-reprojection-error");
    std::cout << "PnP reprojection RMS=" << error << " pixels\n";
}
/// \brief 生成本地 MJPEG 视频并实际解码，验证帧数而不依赖下载或相机。
void video(const std::filesystem::path& output)
{
    CVLAB_TRACE();
    const auto file = (output / "generated.avi").string();
    constexpr int kFrames = 12;
    cv::VideoWriter writer(file, cv::CAP_OPENCV_MJPEG, cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(160,120));
    require(writer.isOpened(), "mjpeg-writer-unavailable");
    for (int index = 0; index < kFrames; ++index) {
        cv::Mat image(120,160,CV_8UC3,cv::Scalar(20,20,20));
        cv::rectangle(image, cv::Rect(10+index*3,40,20,20), cv::Scalar(220,220,220), cv::FILLED);
        writer.write(image);
    }
    writer.release();
    cv::VideoCapture capture(file, cv::CAP_OPENCV_MJPEG);
    require(capture.isOpened(), "mjpeg-reader-unavailable");
    int frames = 0;
    cv::Mat decoded;
    while (capture.read(decoded)) {
        require(decoded.size() == cv::Size(160,120) && decoded.type() == CV_8UC3, "decoded-frame-contract");
        ++frames;
    }
    require(frames == kFrames, "video-frame-count");
    cv::VideoCapture missing((output / "does-not-exist.avi").string(), cv::CAP_OPENCV_MJPEG);
    require(!missing.isOpened(), "missing-video-was-accepted");
    std::cout << "MJPEG decoded=" << frames << " frames FPS=10\n";
}
/// \brief 用固定平移验证稀疏金字塔光流的误差和有效状态。
void flow()
{
    CVLAB_TRACE();
    cv::Mat first(120,160,CV_8UC1,cv::Scalar(0));
    for (int y = 20; y < 100; y += 20) {
        for (int x = 20; x < 140; x += 20) { cv::rectangle(first,cv::Rect(x,y,7,7),cv::Scalar(255),cv::FILLED); }
    }
    const cv::Matx23d transform(1,0,3,0,1,2);
    cv::Mat second;
    cv::warpAffine(first,second,transform,first.size());
    std::vector<cv::Point2f> points, tracked;
    cv::goodFeaturesToTrack(first,points,40,0.01,8);
    std::vector<uchar> status;
    std::vector<float> errors;
    cv::calcOpticalFlowPyrLK(first,second,points,tracked,status,errors);
    int accepted = 0;
    for (std::size_t index = 0; index < points.size(); ++index) {
        if (!status[index]) { continue; }
        const auto residual = tracked[index] - points[index] - cv::Point2f(3,2);
        require(cv::norm(residual) < 0.1, "lk-translation-error");
        ++accepted;
    }
    require(accepted >= 10, "lk-too-few-points");
    std::cout << "LK accepted=" << accepted << " delta=(3,2) tolerance=0.1px\n";
}
/// \brief 实际读取本地 ONNX 并验证 CPU ReLU 推理，独立于张量模拟实验。
void dnn(const std::string& model)
{
    CVLAB_TRACE();
    require(std::filesystem::is_regular_file(model), "missing-onnx-model");
    auto net = cv::dnn::readNetFromONNX(model);
    require(!net.empty(), "onnx-empty-net");
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    const std::array<int,4> shape{1,3,2,2};
    cv::Mat input(4, shape.data(), CV_32F);
    for (std::size_t index = 0; index < input.total(); ++index) { input.ptr<float>()[index] = static_cast<float>(index)-5.0F; }
    net.setInput(input);
    const auto result = net.forward();
    require(result.total() == input.total() && result.type() == CV_32F && result.isContinuous(), "relu-output-contract");
    for (std::size_t index = 0; index < input.total(); ++index) {
        require(std::abs(result.ptr<float>()[index]-std::max(0.0F,input.ptr<float>()[index])) < 1e-6F, "relu-output-value");
    }
    bool rejected = false;
    try { static_cast<void>(cv::dnn::readNetFromONNX(model + ".missing")); }
    catch (const cv::Exception&) { rejected = true; }
    require(rejected, "missing-onnx-not-rejected");
    std::cout << "ONNX CPU ReLU 12/12 values correct; not a trained detector\n";
}
} // namespace
/// \brief 启动固定环境下的原生模块实验并统一报告失败。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int result = 0;
    try {
        CVLAB_TRACE();
        require(argc == 4, "usage: native_labs <features|camera|video|flow|dnn> <output> <onnx>");
        const std::string topic(argv[1]);
        const std::filesystem::path output(argv[2]);
        std::filesystem::create_directories(output);
        if (topic == "features") { features(); }
        else if (topic == "camera") { camera(); }
        else if (topic == "video") { video(output); }
        else if (topic == "flow") { flow(); }
        else if (topic == "dnn") { dnn(argv[3]); }
        else { throw std::invalid_argument("unknown native topic"); }
        require(static_cast<bool>(std::cout), "stdout-write-failed");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        result = app::reportFailure("native-lab-failed");
    }
    return app::finishLogging(result);
}
