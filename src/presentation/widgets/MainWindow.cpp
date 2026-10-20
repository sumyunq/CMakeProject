// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include "presentation/widgets/MainWindow.h"

#include <optional>
#include <stdexcept>

#include <QtCore/QFile>
#include <QtGui/QPixmap>
#include <QtWidgets/QPushButton>

#include <opencv2/core.hpp>

#include "business/services/ImageInspection.h"
#include "core/diagnostics/Logger.h"
#include "presentation/widgets/ImageConversion.h"
#include "ui_MainWindow.h"

namespace app::presentation {
namespace {
constexpr int kImageWidth = 240;
constexpr int kImageHeight = 120;
constexpr int kColorWidth = 80;
constexpr int kFullChannel = 255;
} // namespace

class MainWindowPrivate {
public:
    Ui::MainWindow ui;
    cv::Mat image;
    std::optional<business::ImageProperties> properties;
};

/// @brief 先生成布局和状态，再应用样式、连接与初始图像。
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , clp_state(std::make_unique<MainWindowPrivate>())
{
    CVLAB_TRACE();
    clp_state->ui.setupUi(this);
    initializeStyle();
    initializeConnections();
    initializeImage();
}

/// @brief 非 QObject 状态独立释放，UI 子控件保留 Qt 对象树所有权。
MainWindow::~MainWindow() { CVLAB_TRACE(); }

/// @brief 只从内嵌资源加载样式，资源失败交由入口报告。
void MainWindow::initializeStyle()
{
    CVLAB_TRACE();
    QFile style(QStringLiteral(":/learning/learning.qss"));
    if (!style.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("style-resource-open-failed");
    }
    const auto content = style.readAll();
    if (style.error() != QFileDevice::NoError || content.isEmpty()) {
        throw std::runtime_error("style-resource-read-failed");
    }
    setStyleSheet(QString::fromUtf8(content));
}

/// @brief 同线程 UI 连接集中初始化，不使用 on_object_signal 自动槽。
void MainWindow::initializeConnections()
{
    CVLAB_TRACE();
    connect(clp_state->ui.closeButton, &QPushButton::clicked, this, &QWidget::close);
}

/// @brief 图像和属性保存在模型状态，控件仅展示，不反向充当数据来源。
void MainWindow::initializeImage()
{
    CVLAB_TRACE();
    auto& image = clp_state->image;
    image = cv::Mat(kImageHeight, kImageWidth, CV_8UC3);
    image.colRange(0, kColorWidth).setTo(cv::Scalar(0, 0, kFullChannel));
    image.colRange(kColorWidth, kColorWidth * 2).setTo(cv::Scalar(0, kFullChannel, 0));
    image.colRange(kColorWidth * 2, kImageWidth).setTo(cv::Scalar(kFullChannel, 0, 0));
    clp_state->properties = business::inspectImage(image);
    const auto converted = toOwnedQImage(image);
    if (!clp_state->properties || !std::holds_alternative<QImage>(converted)) {
        throw std::runtime_error("sample-image-conversion-failed");
    }
    clp_state->ui.imageLabel->setPixmap(QPixmap::fromImage(std::get<QImage>(converted)));
    const auto& properties = *clp_state->properties;
    clp_state->ui.propertiesLabel->setText(
        tr("宽 %1 × 高 %2 像素 · %3 通道 · CV_8UC3 · 行步长 %4 字节")
            .arg(properties.width).arg(properties.height).arg(properties.channels)
            .arg(static_cast<qulonglong>(properties.rowStrideBytes)));
}
} // namespace app::presentation
