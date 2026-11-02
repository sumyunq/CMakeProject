// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-11-02 11:12:46 +08:00 wbliu
#pragma warning(push)
#pragma warning(disable: 4127)
#include "business/Processing.h"
#include <opencv2/imgproc.hpp>
#pragma warning(pop)

#include <stdexcept>

#include "core/diagnostics/Logger.h"

namespace workbench {
/// \brief 生成独立掩模及对应的连通域结果，处理过程中检查取消。
ProcessingResult processImage(const cv::Mat& image, const Parameters& parameters, std::stop_token stop)
{
    CVLAB_TRACE();
    if (image.empty() || image.dims != 2 || image.type() != CV_8UC1 || parameters.threshold < 0
        || parameters.threshold > 255 || parameters.minimumArea < 1 || parameters.minimumArea > 100000000) {
        throw std::invalid_argument("image or parameters");
    }
    if (stop.stop_requested()) { throw std::runtime_error("cancelled"); }
    ProcessingResult result;
    result.image = image;
    cv::threshold(image, result.mask, parameters.threshold, 255, cv::THRESH_BINARY);
    cv::Mat labels, stats, centroids;
    const int count = cv::connectedComponentsWithStats(result.mask, labels, stats, centroids);
    if (stop.stop_requested()) { throw std::runtime_error("cancelled"); }
    std::vector<uchar> retained(static_cast<std::size_t>(count), 0);
    for (int index = 1; index < count; ++index) {
        if (stop.stop_requested()) { throw std::runtime_error("cancelled"); }
        const int area = stats.at<int>(index, cv::CC_STAT_AREA);
        if (area < parameters.minimumArea) { continue; }
        if (result.rows.size() >= kMaxComponents) { throw std::length_error("too-many-components"); }
        retained[static_cast<std::size_t>(index)] = 255;
        result.rows.push_back({index, area, stats.at<int>(index, cv::CC_STAT_LEFT),
            stats.at<int>(index, cv::CC_STAT_TOP), stats.at<int>(index, cv::CC_STAT_WIDTH),
            stats.at<int>(index, cv::CC_STAT_HEIGHT)});
    }
    // 用标签查表一次遍历，避免每个被过滤对象都扫描整张图。
    for (int y = 0; y < labels.rows; ++y) {
        if (stop.stop_requested()) { throw std::runtime_error("cancelled"); }
        const auto* labelRow = labels.ptr<int>(y);
        auto* outputRow = result.mask.ptr<uchar>(y);
        for (int x = 0; x < labels.cols; ++x) { outputRow[x] = retained.at(static_cast<std::size_t>(labelRow[x])); }
    }
    return result;
}
} // namespace workbench
