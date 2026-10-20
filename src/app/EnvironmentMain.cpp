// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include <climits>
#include <iostream>
#include <string_view>

#include <QtCore/QCoreApplication>
#include <QtCore/QtGlobal>

#include <opencv2/core.hpp>

#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"

namespace {
constexpr long kCxx20 = 202002L;
constexpr float kFirstValue = 1.0F;
constexpr float kSecondValue = 2.0F;
constexpr float kExpectedValue = 3.0F;

/// @brief 输出编译与运行时证据，检查实际库版本和一次基础运算。
int probeEnvironment()
{
    CVLAB_TRACE();
    const cv::Mat first(2, 2, CV_32FC1, cv::Scalar(kFirstValue));
    const cv::Mat second(2, 2, CV_32FC1, cv::Scalar(kSecondValue));
    const cv::Mat sum = first + second;
    std::cout << "Qt headers: " << QT_VERSION_STR << "\nQt runtime: " << qVersion()
              << "\nOpenCV headers: " << CV_VERSION << "\nOpenCV runtime: " << cv::getVersionString()
              << "\nC++: " << __cplusplus << "\nArchitecture: " << sizeof(void*) * CHAR_BIT
              << "-bit\nMSVC full version: " << _MSC_FULL_VER
#ifdef NDEBUG
              << "\nConfiguration: Release"
#else
              << "\nConfiguration: Debug"
#endif
              << "\nMatrix check: " << sum.at<float>(0, 0) << '\n';
    if (std::string_view(qVersion()) != "6.8.3" || cv::getVersionString() != "5.0.0"
        || __cplusplus < kCxx20 || sizeof(void*) * CHAR_BIT != 64
        || sum.at<float>(0, 0) != kExpectedValue || !std::cout) {
        return app::reportFailure("environment-verification-failed");
    }
    return 0;
}
} // namespace

/// @brief 日志与追踪作用域均在正常退出前完成。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    try {
        if (!app::initializeLogging()) { return 1; }
        int result = 0;
        {
            CVLAB_TRACE();
            result = probeEnvironment();
        }
        return app::finishLogging(result);
    } catch (...) {
        return app::finishLogging(app::reportFailure("environment-exception"));
    }
}
