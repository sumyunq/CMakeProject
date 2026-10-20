// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include <iostream>

#include <QtCore/QCoreApplication>

#include <opencv2/core.hpp>

#include "app/ApplicationRuntime.h"
#include "business/services/ImageInspection.h"
#include "core/diagnostics/Logger.h"

namespace {
constexpr int kRows = 3;
constexpr int kColumns = 4;
constexpr uchar kAliasValue = 9;
constexpr uchar kRoiValue = 7;

/// @brief 与第二课逐步预测对应的可运行实验。
int demonstrateMat()
{
    CVLAB_TRACE();
    cv::Mat original = cv::Mat::zeros(kRows, kColumns, CV_8UC1);
    auto alias = original;
    const auto independent = original.clone();
    alias.at<uchar>(0, 1) = kAliasValue;
    auto roi = original(cv::Rect(1, 1, 2, 2));
    roi.setTo(kRoiValue);
    const auto properties = app::business::inspectImage(roi);
    if (!properties) { return app::reportFailure("roi-inspection-failed"); }
    std::cout << "original / alias:\n" << original << "\nindependent:\n" << independent
              << "\nroi:\n" << roi << "\nROI width=" << properties->width
              << " height=" << properties->height << " channels=" << properties->channels
              << " rowStrideBytes=" << properties->rowStrideBytes
              << " continuous=" << std::boolalpha << properties->continuous << '\n';
    if (original.at<uchar>(0, 1) != kAliasValue || independent.at<uchar>(0, 1) != 0
        || original.at<uchar>(1, 1) != kRoiValue || properties->continuous || !std::cout) {
        return app::reportFailure("mat-prediction-mismatch");
    }
    return 0;
}
} // namespace

/// @brief 建立应用日志上下文后运行实验。
int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    try {
        if (!app::initializeLogging()) { return 1; }
        int result = 0;
        {
            CVLAB_TRACE();
            result = demonstrateMat();
        }
        return app::finishLogging(result);
    } catch (...) {
        return app::finishLogging(app::reportFailure("mat-demo-exception"));
    }
}
