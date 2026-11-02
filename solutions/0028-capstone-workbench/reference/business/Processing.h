/**
 * @file Processing.h
 * @brief 图像工作台的无界面业务模型及处理接口。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-11-02 11:12:46 +08:00 wbliu
 */
#pragma once

#include <array>
#include <cstddef>
#include <stop_token>
#include <vector>

#include <opencv2/core/mat.hpp>

namespace workbench {
inline constexpr std::size_t kMaxComponents = 1000; ///< 工作台最多接受的前景连通域数量。
/// \brief 工作台的处理参数快照，不保存任何界面对象。
struct Parameters {
    int threshold = 100; ///< 分割阈值，范围0到255。
    int minimumArea = 1; ///< 连通域最小像素面积。
};

/// \brief 包含输入快照、独立掩模和连通域属性的处理结果。
struct ProcessingResult {
    cv::Mat image; ///< 保留对调用者输入快照的引用计数所有权。
    cv::Mat mask; ///< 独立像素存储的处理结果。
    std::vector<std::array<int, 6>> rows; ///< 每行依次为编号、面积、左、上、宽、高。
};

/**
 * @brief 阈值分割并过滤小面积连通域，不访问界面。
 * @param image 非空二维CV_8UC1图像，调用期间不得被并发修改。
 * @param parameters 阈值和最小面积的不可变快照。
 * @param stop 协作取消令牌，在每个连通域之间检查。
 * @return 持有输入引用及独立结果的处理结果。
 * @note 参数无效、处理失败、取消或结果超过 kMaxComponents 时抛出异常；由调用方在线程边界捕获。
 */
ProcessingResult processImage(const cv::Mat& image, const Parameters& parameters, std::stop_token stop);
} // namespace workbench
