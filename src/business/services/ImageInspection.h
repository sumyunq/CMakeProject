/**
 * @file ImageInspection.h
 * @brief 检查二维图像属性。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:55:25 +08:00 wbliu
 */
#pragma once

#include <optional>

#include "business/models/ImageProperties.h"

namespace cv { class Mat; }

namespace app::business {
/**
 * @brief 读取二维非空图像的元数据，不复制或修改像素。
 * @param image 调用期间保持有效且不被并发修改的矩阵。
 * @return 图像属性；空矩阵或非二维矩阵返回 std::nullopt。
 * @note 不持有 image；不同、无并发修改的输入可并行调用。
 */
std::optional<ImageProperties> inspectImage(const cv::Mat& image);
} // namespace app::business
