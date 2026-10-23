/**
 * @file main.cpp
 * @brief 独立工作台参考，合成图同步小任务；不冒充后台取消验收。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-23 16:44:36 +08:00 wbliu
 */
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <QtCore/QFile>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
namespace {
/// \brief 学习参数模型，控件只编辑和显示它。
struct Parameters {
    int threshold = 100; ///< 分割阈值，闭区间0到255。
};
/// \brief 把业务输入和参数转为唯一结果，不访问界面。
cv::Mat process(const cv::Mat& input, int threshold)
{
    if (input.empty() || input.type() != CV_8UC1 || threshold < 0 || threshold > 255) {
        throw std::invalid_argument("image or threshold");
    }
    cv::Mat output;
    cv::threshold(input, output, threshold, 255, cv::THRESH_BINARY);
    return output;
}
/// \brief 界面持有独立图像副本，避免Mat临时内存失效。
QPixmap display(const cv::Mat& image)
{
    const QImage view(image.data, image.cols, image.rows,
                      static_cast<qsizetype>(image.step), QImage::Format_Grayscale8);
    return QPixmap::fromImage(view.copy());
}
}
/// \brief 集中连接参数、文件操作与结果展示，异常不跨信号槽。
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    QWidget window;
    window.setWindowTitle(QObject::tr("图像处理工作台参考"));
    QVBoxLayout layout(&window);
    QHBoxLayout views;
    QLabel original;
    QLabel mask;
    QLabel status;
    QSpinBox threshold;
    Parameters parameters;
    QPushButton open(QObject::tr("打开灰度图"));
    QPushButton save(QObject::tr("导出掩模"));
    threshold.setRange(0, 255);
    threshold.setValue(parameters.threshold);
    views.addWidget(&original);
    views.addWidget(&mask);
    layout.addLayout(&views);
    layout.addWidget(&threshold);
    layout.addWidget(&open);
    layout.addWidget(&save);
    layout.addWidget(&status);
    cv::Mat input(96, 128, CV_8UC1, cv::Scalar(20));
    for (int index = 0; index < 3; ++index) {
        input(cv::Rect(10 + 36 * index, 30, 12, 20)).setTo(220);
    }
    cv::Mat result;
    const auto refresh = [&] {
        try {
            result = process(input, parameters.threshold);
            cv::Mat labels;
            const int objects = cv::connectedComponents(result, labels) - 1;
            original.setPixmap(display(input));
            mask.setPixmap(display(result));
            status.setText(QObject::tr("对象 %1；面积 %2 像素").arg(objects).arg(cv::countNonZero(result)));
        } catch (const std::exception&) {
            QMessageBox::warning(&window, QObject::tr("处理失败"), QObject::tr("请检查图像及参数"));
        }
    };
    QObject::connect(&threshold, &QSpinBox::valueChanged, &window, [&](int value) {
        parameters.threshold = value;
        refresh();
    });
    QObject::connect(&open, &QPushButton::clicked, &window, [&] {
        const auto path = QFileDialog::getOpenFileName(&window, QObject::tr("打开图像"));
        if (path.isEmpty()) { return; }
        try {
            QFile file(path);
            if (!file.open(QIODevice::ReadOnly)) { throw std::runtime_error("open"); }
            const auto bytes = file.readAll();
            if (file.error() != QFileDevice::NoError || bytes.isEmpty()) { throw std::runtime_error("read"); }
            const std::vector<uchar> encoded(bytes.begin(), bytes.end());
            auto candidate = cv::imdecode(encoded, cv::IMREAD_GRAYSCALE);
            if (candidate.empty()) { throw std::runtime_error("decode"); }
            input = candidate;
            refresh();
        } catch (const std::exception&) {
            QMessageBox::warning(&window, QObject::tr("打开失败"), QObject::tr("文件无法读取或不是有效图像"));
        }
    });
    QObject::connect(&save, &QPushButton::clicked, &window, [&] {
        const auto path = QFileDialog::getSaveFileName(&window, QObject::tr("保存PNG"), QString(), QStringLiteral("PNG (*.png)"));
        if (path.isEmpty()) { return; }
        try {
            std::vector<uchar> bytes;
            if (!cv::imencode(".png", result, bytes)) { throw std::runtime_error("encode"); }
            QFile file(path);
            const auto size = static_cast<qint64>(bytes.size());
            if (!file.open(QIODevice::WriteOnly) || file.write(reinterpret_cast<const char*>(bytes.data()), size) != size
                || !file.flush()) { throw std::runtime_error("write"); }
            status.setText(QObject::tr("导出成功"));
        } catch (const std::exception&) {
            QMessageBox::warning(&window, QObject::tr("保存失败"), QObject::tr("无法编码或写入PNG"));
        }
    });
    refresh();
    if (application.arguments().contains(QStringLiteral("--self-test"))) {
        if (cv::countNonZero(result) != 720 || original.pixmap().isNull() || mask.pixmap().isNull()) { return 2; }
        threshold.setValue(250);
        if (parameters.threshold != 250 || cv::countNonZero(result) != 0) { return 3; }
        threshold.setValue(100);
        if (parameters.threshold != 100 || cv::countNonZero(result) != 720) { return 4; }
        std::cout << "PASS: Qt parameter signal, model, image and mask display contracts\n";
        return 0;
    }
    window.show();
    return application.exec();
}
