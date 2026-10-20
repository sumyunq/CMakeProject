/**
 * @file ImageProperties.h
 * @brief 图像元数据，不保存 UI 状态。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:55:25 +08:00 wbliu
 */
#pragma once

#include <cstddef>

namespace app::business {
/// \brief 二维图像的元数据快照，不持有像素缓冲区或界面控件。
struct ImageProperties {
    int width = 0;                 ///< 列数，单位像素。
    int height = 0;                ///< 行数，单位像素。
    int channels = 0;              ///< 每个像素的通道数。
    int depth = 0;                 ///< OpenCV 通道深度编码。
    std::size_t rowStrideBytes = 0; ///< 相邻行首地址间隔，单位字节。
    bool continuous = false;       ///< 像素数据是否连续。
};
} // namespace app::business
