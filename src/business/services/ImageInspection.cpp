// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include "business/services/ImageInspection.h"

#include <opencv2/core.hpp>

#include "core/diagnostics/Logger.h"

namespace app::business {
/// @brief 只读取元数据；空矩阵和张量不是本课二维图像接口的有效输入。
std::optional<ImageProperties> inspectImage(const cv::Mat& image)
{
    CVLAB_TRACE();
    if (image.empty() || image.dims != 2) {
        return std::nullopt;
    }
    ImageProperties properties;
    properties.width = image.cols;
    properties.height = image.rows;
    properties.channels = image.channels();
    properties.depth = image.depth();
    properties.rowStrideBytes = image.step[0];
    properties.continuous = image.isContinuous();
    return properties;
}
} // namespace app::business
