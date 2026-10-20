/*!
 * @file ImageConversion.h
 * @brief OpenCV 像素到 Qt 显示图像的所有权边界。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:01:48 +08:00 wbliu
 */
#pragma once

#include <variant>

#include <QtGui/QImage>

namespace cv { class Mat; }

namespace app::presentation {
enum class ImageConversionError { kEmpty, kUnsupportedType, kInvalidDimensions, kAllocationFailure };
using ImageConversionResult = std::variant<QImage, ImageConversionError>;
/*!
 * @brief 将二维 CV_8UC1/3/4 图像转换成独立拥有像素的 QImage。
 * @param image 灰度、BGR 或 BGRA 输入；调用期间不得被其他线程修改。
 * @return QImage 或明确错误；空输入、维度或类型错误不抛异常。
 * @note 不保存输入引用；返回值可在输入销毁后使用，可在工作线程构建。
 */
ImageConversionResult toOwnedQImage(const cv::Mat& image);
} // namespace app::presentation
