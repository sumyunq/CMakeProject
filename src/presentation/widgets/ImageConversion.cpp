// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include "presentation/widgets/ImageConversion.h"

#include <limits>
#include <new>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "core/diagnostics/Logger.h"

namespace app::presentation {
/// @brief 按显式通道语义转换并复制，使返回值与 OpenCV 数据生命周期分离。
ImageConversionResult toOwnedQImage(const cv::Mat& image)
{
    CVLAB_TRACE();
    if (image.empty()) {
        return ImageConversionError::kEmpty;
    }
    if (image.dims != 2 || image.step[0] > static_cast<std::size_t>(std::numeric_limits<qsizetype>::max())) {
        return ImageConversionError::kInvalidDimensions;
    }
    if (image.type() != CV_8UC1 && image.type() != CV_8UC3 && image.type() != CV_8UC4) {
        return ImageConversionError::kUnsupportedType;
    }
    try {
        cv::Mat formatted;
        auto format = QImage::Format_Grayscale8;
        if (image.type() == CV_8UC3) {
            cv::cvtColor(image, formatted, cv::COLOR_BGR2RGB);
            format = QImage::Format_RGB888;
        } else if (image.type() == CV_8UC4) {
            cv::cvtColor(image, formatted, cv::COLOR_BGRA2RGBA);
            format = QImage::Format_RGBA8888;
        } else {
            formatted = image;
        }
        const auto* pixels = formatted.data;
        const QImage view(pixels, formatted.cols, formatted.rows, static_cast<qsizetype>(formatted.step[0]), format);
        auto owned = view.copy();
        if (owned.isNull()) {
            return ImageConversionError::kAllocationFailure;
        }
        return owned;
    } catch (const cv::Exception&) {
        core::Logger::instance().write(core::LogLevel::kError, "image-conversion-failed");
        return ImageConversionError::kAllocationFailure;
    } catch (const std::bad_alloc&) {
        core::Logger::instance().write(core::LogLevel::kError, "image-allocation-failed");
        return ImageConversionError::kAllocationFailure;
    }
}
} // namespace app::presentation
