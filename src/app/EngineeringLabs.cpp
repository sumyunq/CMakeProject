/**
 * @file EngineeringLabs.cpp
 * @brief 第24至30课离线工程实验；失败以非零退出码报告。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
// Updated: 2026-10-23 16:44:36 +08:00 wbliu
 */
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <QtCore/QCoreApplication>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry.hpp>
#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"
namespace {
/// \brief 将数值契约失败转换为入口可处理的错误。
void require(bool condition, const char* message)
{
    CVLAB_TRACE();
    if (!condition) { throw std::runtime_error(message); }
}
/// \brief 写出并重新解码图片，验证尺寸和像素一致。
void save(const std::filesystem::path& directory, const std::string& name, const cv::Mat& image)
{
    CVLAB_TRACE();
    const auto path = (directory / name).string();
    require(cv::imwrite(path, image), "image-write");
    const auto read = cv::imread(path, cv::IMREAD_UNCHANGED);
    require(!read.empty() && read.size() == image.size() && read.type() == image.type(), "image-read");
    require(cv::norm(read, image, cv::NORM_INF) == 0.0, "png-roundtrip");
}
/// \brief 生成三个不接触的精确面积目标。
cv::Mat objects()
{
    CVLAB_TRACE();
    cv::Mat image(96, 128, CV_8UC1, cv::Scalar(20));
    for (int index = 0; index < 3; ++index) {
        image(cv::Rect(10 + index * 36, 30, 12, 20)).setTo(220);
    }
    return image;
}
/// \brief 计算掩模的前景连通区域数。
int count(const cv::Mat& mask)
{
    CVLAB_TRACE();
    cv::Mat labels;
    return cv::connectedComponents(mask, labels, 8, CV_32S) - 1;
}
/// \brief 对比别名、深拷贝和预分配，计时仅作为本机观察值。
void profile(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    const auto image = objects();
    const auto alias = image;
    const auto copy = image.clone();
    require(alias.data == image.data && copy.data != image.data, "ownership");
    const auto roi = image(cv::Rect(1, 1, 32, 32));
    require(!roi.isContinuous(), "roi-stride");
    cv::Mat result;
    cv::threshold(image, result, 100, 255, cv::THRESH_BINARY);
    const auto start = std::chrono::steady_clock::now();
    constexpr int kIterations = 100;
    for (int iteration = 0; iteration < kIterations; ++iteration) {
        cv::threshold(image, result, 100, 255, cv::THRESH_BINARY);
    }
    const auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
    require(cv::countNonZero(result) == 720, "threshold-area");
    std::cout << "iterations=100 bytes=" << image.total() * image.elemSize() << " elapsed_ms=" << elapsed << '\n';
    save(directory, "24-input.png", image);
    save(directory, "24-mask.png", result);
}
/// \brief 运行容量二的生产消费队列，stop_token 唤醒取消路径。
void pipeline(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    std::mutex mutex;
    std::condition_variable_any available;
    std::deque<int> queue;
    bool done = false;
    std::size_t peak = 0;
    int sum = 0;
    std::jthread producer([&](std::stop_token stop) {
        CVLAB_TRACE();
        for (int value = 0; value < 20; ++value) {
            std::unique_lock lock(mutex);
            if (!available.wait(lock, stop, [&] { return queue.size() < 2; })) { return; }
            queue.push_back(value);
            peak = std::max(peak, queue.size());
            available.notify_all();
        }
        { std::lock_guard lock(mutex); done = true; }
        available.notify_all();
    });
    std::jthread consumer([&](std::stop_token stop) {
        CVLAB_TRACE();
        for (;;) {
            std::unique_lock lock(mutex);
            if (!available.wait(lock, stop, [&] { return done || !queue.empty(); })) { return; }
            if (queue.empty() && done) { return; }
            sum += queue.front();
            queue.pop_front();
            available.notify_all();
        }
    });
    producer.join();
    consumer.join();
    require(sum == 190 && peak <= 2 && queue.empty(), "bounded-pipeline");
    bool cancelled = false;
    std::jthread blocked([&](std::stop_token stop) {
        CVLAB_TRACE();
        std::unique_lock lock(mutex);
        cancelled = !available.wait(lock, stop, [] { return false; });
    });
    blocked.request_stop();
    blocked.join();
    require(cancelled, "cancel-wakeup");
    std::cout << "sum=" << sum << " peak=" << peak << " cancelled=" << cancelled << '\n';
    save(directory, "25-input.png", objects());
}
/// \brief 模拟设备传输的CPU深拷贝，不能作为GPU验证。
void transfer(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    const auto host = objects();
    auto deviceSurrogate = host.clone();
    cv::Mat output;
    cv::threshold(deviceSurrogate, output, 100, 255, cv::THRESH_BINARY);
    require(cv::countNonZero(output) == 720, "cpu-surrogate-area");
    require(host.data != deviceSurrogate.data, "transfer-ownership");
    require(cv::imread((directory / "missing-fixture.png").string()).empty(), "missing-input");
    save(directory, "26-host.png", host);
    save(directory, "26-result.png", output);
    std::cout << "backend=CPU-surrogate GPU_VALIDATED=false\n";
}
/// \brief 用非对称核对照朴素相关与filter2D，并揭示卷积翻核错误。
void compare(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    cv::Mat input(5, 5, CV_32FC1);
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) { input.at<float>(y, x) = static_cast<float>(y * 5 + x); }
    }
    cv::Mat kernel(1, 3, CV_32F);
    kernel.at<float>(0, 0) = 1.0F;
    kernel.at<float>(0, 1) = 0.0F;
    kernel.at<float>(0, 2) = -1.0F;
    cv::Mat actual;
    cv::filter2D(input, actual, CV_32F, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
    cv::Mat expected = cv::Mat::zeros(input.size(), CV_32F);
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            const float left = x > 0 ? input.at<float>(y, x - 1) : 0.0F;
            const float right = x < 4 ? input.at<float>(y, x + 1) : 0.0F;
            expected.at<float>(y, x) = left - right;
        }
    }
    const auto error = cv::norm(expected, actual, cv::NORM_INF);
    require(error < 1e-6 && actual.at<float>(2, 2) == -2.0F, "correlation-equivalence");
    cv::Mat flipped;
    cv::flip(kernel, flipped, 1);
    cv::Mat wrong;
    cv::filter2D(input, wrong, CV_32F, flipped, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
    require(cv::norm(wrong, actual, cv::NORM_INF) > 1.0, "asymmetric-negative");
    cv::Mat visible;
    actual.convertTo(visible, CV_8U, 4, 128);
    save(directory, "27-response-offset128.png", visible);
    std::cout << "max_error=" << error << " center=-2 flipped_center=2\n";
}
/// \brief 离线工作台业务核心：阈值、计数、参数失败和导出。
void workbench(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    const auto input = objects();
    cv::Mat mask;
    cv::threshold(input, mask, 100, 255, cv::THRESH_BINARY);
    require(count(mask) == 3 && cv::countNonZero(mask) == 720, "workbench-count");
    cv::Mat wrong;
    cv::threshold(input, wrong, 250, 255, cv::THRESH_BINARY);
    require(count(wrong) == 0, "workbench-bad-threshold");
    save(directory, "28-input.png", input);
    save(directory, "28-mask.png", mask);
    save(directory, "28-bad-threshold.png", wrong);
    std::cout << "count=3 area=720 threshold250_count=0\n";
}
/// \brief 工业测量：像素面积、标定宽度和缺陷异或检验。
void inspect(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    cv::Mat reference(80, 100, CV_8UC1, cv::Scalar(0));
    reference(cv::Rect(20, 20, 40, 20)).setTo(255);
    auto defective = reference.clone();
    defective(cv::Rect(30, 25, 4, 5)).setTo(0);
    cv::Mat difference;
    cv::bitwise_xor(reference, defective, difference);
    require(cv::countNonZero(reference) == 800 && cv::countNonZero(difference) == 20, "defect-area");
    const auto bounds = cv::boundingRect(reference);
    constexpr double kMmPerPixel = 0.1;
    require(std::abs(bounds.width * kMmPerPixel - 4.0) < 1e-9, "measurement");
    cv::Mat shifted(reference.size(), reference.type(), cv::Scalar(0));
    reference(cv::Rect(0, 0, 99, 80)).copyTo(shifted(cv::Rect(1, 0, 99, 80)));
    cv::Mat alignmentError;
    cv::bitwise_xor(reference, shifted, alignmentError);
    require(cv::countNonZero(alignmentError) == 40, "alignment-negative");
    save(directory, "29-reference.png", reference);
    save(directory, "29-defective.png", defective);
    save(directory, "29-difference.png", difference);
    save(directory, "29-misalignment.png", alignmentError);
    std::cout << "width_mm=4 defect_pixels=20 shifted_false_pixels=40\n";
}
/// \brief 用生成帧替代模型和相机，验证检测跟踪契约及丢检恢复。
void video(const std::filesystem::path& directory)
{
    CVLAB_TRACE();
    int detections = 0;
    int previousX = 0;
    int previousFrame = 0;
    for (int frame = 0; frame < 10; ++frame) {
        cv::Mat input(64, 96, CV_8UC1, cv::Scalar(10));
        if (frame != 5) { input(cv::Rect(10 + 3 * frame, 20, 10, 10)).setTo(230); }
        cv::Mat mask;
        cv::threshold(input, mask, 100, 255, cv::THRESH_BINARY);
        const bool detected = cv::countNonZero(mask) != 0;
        if (detected) {
            const auto bounds = cv::boundingRect(mask);
            if (detections > 0) { require(bounds.x - previousX == 3 * (frame - previousFrame), "velocity"); }
            previousX = bounds.x;
            previousFrame = frame;
            ++detections;
        } else { require(frame == 5, "missing-frame-contract"); }
        save(directory, "30-frame-" + std::to_string(frame) + ".png", input);
        save(directory, "30-mask-" + std::to_string(frame) + ".png", mask);
    }
    require(detections == 9 && previousX == 37, "tracking-recovery");
    std::cout << "detections=9 missed=1 speed_px_per_frame=3 final_x=37 backend=fixture-detector\n";
}
} // namespace
/// \brief 建立日志上下文，严格校验课号并调用独立实验。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int result = 0;
    try {
        CVLAB_TRACE();
        require(argc == 3, "usage-engineering_labs-ID-outputdir");
        const std::string id(argv[1]);
        const std::filesystem::path directory(argv[2]);
        std::filesystem::create_directories(directory);
        if (id == "24") { profile(directory); }
        else if (id == "25") { pipeline(directory); }
        else if (id == "26") { transfer(directory); }
        else if (id == "27") { compare(directory); }
        else if (id == "28") { workbench(directory); }
        else if (id == "29") { inspect(directory); }
        else if (id == "30") { video(directory); }
        else { throw std::invalid_argument("lesson-id"); }
        require(static_cast<bool>(std::cout), "console-output");
    } catch (const std::exception& error) {
        std::cerr << "Engineering lab: " << error.what() << '\n';
        result = app::reportFailure("engineering-lab-failed");
    }
    return app::finishLogging(result);
}
