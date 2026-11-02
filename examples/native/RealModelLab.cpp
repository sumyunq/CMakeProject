// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-11-02 11:19:51 +08:00 wbliu
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <QtCore/QCoreApplication>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
/// \brief 读取图片，执行 ImageNet 标准化和 SqueezeNet CPU 推理。
void run(const char* imagePath, const char* modelPath, bool verifyFixture, const std::string& labelPath)
{
    CVLAB_TRACE();
    const cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty() || !std::filesystem::is_regular_file(modelPath)) { throw std::runtime_error("missing image or model"); }
    cv::Mat resized, rgb;
    cv::resize(image, resized, cv::Size(224, 224));
    cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);
    rgb.convertTo(rgb, CV_32FC3, 1.0 / 255.0);
    std::vector<cv::Mat> channels;
    cv::split(rgb, channels);
    constexpr std::array<float, 3> kMean{0.485F, 0.456F, 0.406F};
    constexpr std::array<float, 3> kStd{0.229F, 0.224F, 0.225F};
    for (std::size_t i = 0; i < channels.size(); ++i) { channels[i] = (channels[i] - kMean[i]) / kStd[i]; }
    cv::merge(channels, rgb);
    const cv::Mat input = cv::dnn::blobFromImage(rgb);
    auto net = cv::dnn::readNetFromONNX(modelPath);
    if (net.empty()) { throw std::runtime_error("empty model"); }
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    net.setInput(input);
    const cv::Mat output = net.forward();
    if (output.type() != CV_32F || !output.isContinuous() || output.total() != 1000 || !cv::checkRange(output)) { throw std::runtime_error("invalid output"); }
    std::vector<int> indices(output.total());
    std::iota(indices.begin(), indices.end(), 0);
    const float* values = output.ptr<float>();
    std::partial_sort(indices.begin(), indices.begin() + 5, indices.end(), [values](int a, int b) { return values[a] > values[b]; });
    std::vector<std::string> labels;
    if (!labelPath.empty()) {
        std::ifstream source(labelPath);
        if (!source.is_open()) { throw std::runtime_error("missing label file"); }
        std::string line;
        while (std::getline(source, line)) {
            if (!line.empty() && line.back() == '\r') { line.pop_back(); }
            labels.push_back(line);
        }
        if (source.bad() || labels.size() != output.total()) { throw std::runtime_error("invalid label file"); }
    }
    constexpr std::array<int, 5> kExpectedIndices{823, 543, 422, 503, 773};
    constexpr std::array<float, 5> kExpectedScores{19.1695F, 19.0708F, 18.8869F, 18.2567F, 17.7489F};
    constexpr float kTolerance = 0.01F;
    if (output.dims != 2 || output.size[0] != 1 || output.size[1] != 1000) { throw std::runtime_error("output shape mismatch"); }
    if (verifyFixture) {
        for (std::size_t i = 0; i < kExpectedIndices.size(); ++i) {
            if (indices[i] != kExpectedIndices[i] || std::abs(values[indices[i]] - kExpectedScores[i]) > kTolerance) { throw std::runtime_error("fixed image inference mismatch"); }
        }
    }
    std::cout << "input [1,3,224,224] output";
    for (int i = 0; i < output.dims; ++i) { std::cout << ' ' << output.size[i]; }
    std::cout << " top5";
    for (int i = 0; i < 5; ++i) {
        std::cout << ' ' << indices[i] << ':' << values[indices[i]];
        if (!labels.empty()) { std::cout << " (" << labels.at(static_cast<std::size_t>(indices[i])) << ')'; }
    }
    std::cout << '\n';
}
}
/// \brief 初始化日志并报告推理失败。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int result = 0;
    try {
        CVLAB_TRACE();
        if (argc < 3) { throw std::runtime_error("usage: real_model_lab <image> <model> [--verify-fixture] [--labels <file>]"); }
        bool verifyFixture = false;
        std::string labelPath;
        for (int i = 3; i < argc; ++i) {
            const std::string_view option(argv[i]);
            if (option == "--verify-fixture" && !verifyFixture) { verifyFixture = true; }
            else if (option == "--labels" && labelPath.empty() && i + 1 < argc) { labelPath = argv[++i]; }
            else { throw std::runtime_error("invalid option"); }
        }
        run(argv[1], argv[2], verifyFixture, labelPath);
        if (!std::cout) { throw std::runtime_error("cannot write console output"); }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        result = app::reportFailure("real-model-lab-failed");
    }
    return app::finishLogging(result);
}
