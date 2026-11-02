/**
 * @file main.cpp
 * @brief 独立图像处理工作台，后台执行并按版本丢弃过期结果。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-11-02 11:12:46 +08:00 wbliu
 */
#include "ui_workbench.h"

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <mutex>
#include <optional>
#include <thread>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QMetaObject>
#include <QtCore/QThread>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QSaveFile>
#include <QtCore/QTemporaryDir>
#include <QtCore/QSignalBlocker>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
// OpenCV 5.0.0 的 Rect_<double>::area 在 Debug 头文件内触发 MSVC C4127。
#pragma warning(push)
#pragma warning(disable: 4127)
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#pragma warning(pop)
#include "business/Processing.h"
#include "infrastructure/ParametersFile.h"
#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"
namespace {
using workbench::Parameters;
using workbench::loadParameters;
using workbench::saveParameters;
/// \brief 界面持有独立图像副本，避免Mat临时内存失效。
QPixmap display(const cv::Mat& image)
{
    CVLAB_TRACE();
    const QImage view(image.data, image.cols, image.rows,
                      static_cast<qsizetype>(image.step), QImage::Format_Grayscale8);
    const auto owned = view.copy();
    if (owned.isNull()) { throw std::runtime_error("image-copy"); }
    return QPixmap::fromImage(owned);
}
}
/// \brief 运行工作台，关闭时等待后台任务退出。
int runWorkbench(QApplication& application)
{
    CVLAB_TRACE();
    QWidget window;
    Ui::Workbench ui;
    ui.setupUi(&window);
    QFile style(QStringLiteral(":/workbench/workbench.qss"));
    if (!style.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(&window, QObject::tr("资源错误"), QObject::tr("无法加载界面样式"));
        return 1;
    }
    window.setStyleSheet(QString::fromUtf8(style.readAll()));
    if (style.error() != QFileDevice::NoError) { return 1; }
    auto& original = *ui.original;
    auto& mask = *ui.mask;
    auto& status = *ui.status;
    auto& threshold = *ui.threshold;
    auto& minimumArea = *ui.minimumArea;
    auto& components = *ui.components;
    auto& open = *ui.open;
    auto& save = *ui.save;
    auto& cancel = *ui.cancel;
    auto& reset = *ui.reset;
    auto& saveConfig = *ui.saveConfig;
    auto& openConfig = *ui.openConfig;
    Parameters parameters;
    threshold.setRange(0, 255);
    threshold.setValue(parameters.threshold);
    minimumArea.setRange(1, 100000000);
    minimumArea.setValue(parameters.minimumArea);
    minimumArea.setPrefix(QObject::tr("最小面积："));
    components.setColumnCount(5);
    components.setHorizontalHeaderLabels({QObject::tr("编号"), QObject::tr("面积"), QObject::tr("左"), QObject::tr("上"), QObject::tr("宽×高")});
    components.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    cv::Mat input(96, 128, CV_8UC1, cv::Scalar(20));
    for (int index = 0; index < 3; ++index) {
        input(cv::Rect(10 + 36 * index, 30, 12, 20)).setTo(220);
    }
    cv::Mat result;
    QString inputPath;
    struct Request {
        cv::Mat image; ///< 提交时独立复制的输入或待保存掩模。
        QString path; ///< 非空时在后台读取图像。
        QString exportPath; ///< 非空时在后台原子导出PNG。
        Parameters parameters; ///< 提交时的参数快照。
        std::stop_token cancellation; ///< 替换或取消请求时通知工作线程。
        std::uint64_t revision = 0; ///< 用于丢弃过期结果的版本。
    };
    struct Answer {
        cv::Mat image; ///< 与计算结果关联的输入快照。
        cv::Mat mask; ///< 独立拥有像素的二值结果。
        std::vector<std::array<int, 6>> rows; ///< 编号、面积和包围框属性。
        std::uint64_t revision = 0; ///< 对应提交版本。
        bool exported = false; ///< 后台PNG导出是否成功。
        std::string error; ///< 内部固定错误事件，不包含用户路径。
    };
    std::mutex queueMutex;
    std::condition_variable_any ready;
    std::optional<Request> pending;
    std::stop_source activeCancellation;
    std::atomic<std::uint64_t> startedRevision{0};
    std::atomic<std::uint64_t> completedRevision{0};
    std::atomic<std::uint64_t> revision{0};
    std::uint64_t acceptedRevision = 0;
    std::atomic<bool> postFailed{false};
    const auto setInteractive = [&](bool enabled) {
        threshold.setEnabled(enabled); minimumArea.setEnabled(enabled); open.setEnabled(enabled);
        reset.setEnabled(enabled);
        openConfig.setEnabled(enabled); saveConfig.setEnabled(enabled); cancel.setEnabled(enabled);
    };
    std::jthread worker([&](std::stop_token stop) {
        CVLAB_TRACE();
        for (;;) {
            std::optional<Request> request;
            {
                std::unique_lock lock(queueMutex);
                if (!ready.wait(lock, stop, [&] { return pending.has_value(); })) { return; }
                request = std::move(pending);
                pending.reset();
            }
            startedRevision = request->revision;
            Answer answer;
            answer.revision = request->revision;
            try {
                if (!request->exportPath.isEmpty()) {
                    std::vector<uchar> encoded;
                    if (!cv::imencode(".png", request->image, encoded)) { throw std::runtime_error("encode"); }
                    QSaveFile file(request->exportPath);
                    const auto bytes = static_cast<qint64>(encoded.size());
                    if (!file.open(QIODevice::WriteOnly)
                        || file.write(reinterpret_cast<const char*>(encoded.data()), bytes) != bytes) { throw std::runtime_error("write"); }
                    if (stop.stop_requested() || revision.load() != request->revision) { file.cancelWriting(); continue; }
                    if (!file.commit()) { throw std::runtime_error("commit"); }
                    answer.exported = true;
                } else {
                    if (!request->path.isEmpty()) {
                        QFile file(request->path);
                        if (!file.open(QIODevice::ReadOnly) || file.size() <= 0 || file.size() > 64 * 1024 * 1024) { throw std::runtime_error("image-read"); }
                        const auto bytes = file.readAll();
                        if (file.error() != QFileDevice::NoError || bytes.isEmpty()) { throw std::runtime_error("image-read"); }
                        const std::vector<uchar> encoded(bytes.begin(), bytes.end());
                        request->image = cv::imdecode(encoded, cv::IMREAD_GRAYSCALE);
                        if (request->image.empty() || request->image.total() > 16000000) { throw std::runtime_error("image-decode"); }
                    }
                    const auto processed = workbench::processImage(request->image, request->parameters, request->cancellation);
                    answer.image = processed.image;
                    answer.mask = processed.mask;
                    answer.rows = processed.rows;
                }
            } catch (const std::length_error&) { answer.error = "too-many-components"; }
            catch (const std::exception&) { answer.error = request->exportPath.isEmpty() ? "processing-failed" : "export-failed"; }
            completedRevision = request->revision;
            if (stop.stop_requested()) { return; }
            const bool posted = QMetaObject::invokeMethod(&window, [&, answer = std::move(answer)]() mutable {
                CVLAB_TRACE();
                try {
                    if (answer.revision != revision) { return; }
                    if (!answer.error.empty()) {
                        app::core::Logger::instance().write(app::core::LogLevel::kError, "workbench-processing-failed");
                        if (answer.error == "export-failed") { status.setText(QObject::tr("导出失败，请检查路径")); }
                        else if (answer.error == "too-many-components") { status.setText(QObject::tr("对象数量超过1000，请提高最小面积或调整阈值")); }
                        else { status.setText(QObject::tr("处理失败，请检查图像与参数")); }
                        inputPath.clear();
                        acceptedRevision = answer.revision;
                        if (answer.error == "export-failed") { setInteractive(true); }
                        save.setEnabled(answer.error == "export-failed" && !result.empty());
                        return;
                    }
                    if (answer.exported) {
                        status.setText(QObject::tr("导出成功"));
                        setInteractive(true);
                        acceptedRevision = answer.revision;
                        save.setEnabled(true);
                        return;
                    }
                    result = std::move(answer.mask);
                    input = answer.image;
                    inputPath.clear();
                    components.setRowCount(0);
                    for (const auto& component : answer.rows) {
                        const int row = components.rowCount();
                        components.insertRow(row);
                        const QStringList values = {QString::number(component[0]), QString::number(component[1]),
                            QString::number(component[2]), QString::number(component[3]),
                            QStringLiteral("%1×%2").arg(component[4]).arg(component[5])};
                        for (int column = 0; column < values.size(); ++column) {
                            auto item = std::make_unique<QTableWidgetItem>(values.at(column));
                            components.setItem(row, column, item.release());
                        }
                    }
                    original.setPixmap(display(answer.image));
                    mask.setPixmap(display(result));
                    status.setText(QObject::tr("对象 %1；面积 %2 像素").arg(components.rowCount()).arg(cv::countNonZero(result)));
                    acceptedRevision = answer.revision;
                    save.setEnabled(true);
                } catch (const std::exception&) {
                    app::core::Logger::instance().write(app::core::LogLevel::kError, "workbench-display-failed");
                    status.setText(QObject::tr("显示失败"));
                }
            }, Qt::QueuedConnection);
            if (!posted) { postFailed = true; app::core::Logger::instance().write(app::core::LogLevel::kError, "workbench-queue-failed"); }
        }
    });
    const auto refresh = [&] {
        CVLAB_TRACE();
        try {
            Request request;
            request.image = input.clone();
            request.path = inputPath;
            request.parameters = parameters;
            {
                std::lock_guard lock(queueMutex);
                activeCancellation.request_stop();
                activeCancellation = std::stop_source{};
                request.cancellation = activeCancellation.get_token();
                request.revision = ++revision;
                pending = std::move(request); // 单一待处理槽：新的参数覆盖尚未开始的旧请求。
            }
            save.setEnabled(false);
            status.setText(QObject::tr("处理中"));
            ready.notify_one();
        } catch (const std::exception&) {
            app::core::Logger::instance().write(app::core::LogLevel::kError, "workbench-submit-failed");
            status.setText(QObject::tr("无法提交处理任务"));
            save.setEnabled(false);
        }
    };
    const auto cancelPending = [&] {
        CVLAB_TRACE();
        {
            std::lock_guard lock(queueMutex);
            activeCancellation.request_stop();
            ++revision;
            pending.reset();
        }
        inputPath.clear();
        status.setText(QObject::tr("已取消；保留上一次结果"));
        save.setEnabled(false);
    };
    QObject::connect(&cancel, &QPushButton::clicked, &window, cancelPending);
    const auto submitExport = [&](const QString& path) {
        CVLAB_TRACE();
        try {
            Request request;
            request.image = result.clone();
            if (request.image.empty()) { throw std::runtime_error("empty result"); }
            request.exportPath = path;
            request.revision = ++revision;
            { std::lock_guard lock(queueMutex); pending = std::move(request); }
            save.setEnabled(false);
            setInteractive(false);
            status.setText(QObject::tr("正在导出；提交期间不可取消"));
            ready.notify_one();
        } catch (const std::exception&) {
            app::core::Logger::instance().write(app::core::LogLevel::kError, "workbench-export-submit-failed");
            status.setText(QObject::tr("无法提交导出任务"));
            save.setEnabled(false);
        }
    };
    QObject::connect(&reset, &QPushButton::clicked, &window, [&] {
        CVLAB_TRACE();
        parameters = Parameters{};
        const QSignalBlocker thresholdBlocker(&threshold);
        const QSignalBlocker areaBlocker(&minimumArea);
        threshold.setValue(parameters.threshold);
        minimumArea.setValue(parameters.minimumArea);
        refresh();
    });
    QObject::connect(&threshold, &QSpinBox::valueChanged, &window, [&](int value) {
        CVLAB_TRACE();
        parameters.threshold = value;
        refresh();
    });
    QObject::connect(&minimumArea, &QSpinBox::valueChanged, &window, [&](int value) {
        CVLAB_TRACE();
        parameters.minimumArea = value;
        refresh();
    });
    QObject::connect(&open, &QPushButton::clicked, &window, [&] {
        CVLAB_TRACE();
        const auto path = QFileDialog::getOpenFileName(&window, QObject::tr("打开图像"));
        if (path.isEmpty()) { return; }
        inputPath = path;
        refresh();
    });
    QObject::connect(&saveConfig, &QPushButton::clicked, &window, [&] {
        CVLAB_TRACE();
        const auto path = QFileDialog::getSaveFileName(&window, QObject::tr("保存参数"), QString(), QStringLiteral("JSON (*.json)"));
        if (!path.isEmpty() && !saveParameters(path, parameters)) {
            QMessageBox::warning(&window, QObject::tr("保存失败"), QObject::tr("无法写入参数文件"));
        }
    });
    QObject::connect(&openConfig, &QPushButton::clicked, &window, [&] {
        CVLAB_TRACE();
        const auto path = QFileDialog::getOpenFileName(&window, QObject::tr("打开参数"), QString(), QStringLiteral("JSON (*.json)"));
        if (path.isEmpty()) { return; }
        if (!loadParameters(path, parameters)) {
            QMessageBox::warning(&window, QObject::tr("打开失败"), QObject::tr("参数文件无效或无法读取"));
            return;
        }
        const QSignalBlocker thresholdBlocker(&threshold);
        const QSignalBlocker areaBlocker(&minimumArea);
        threshold.setValue(parameters.threshold);
        minimumArea.setValue(parameters.minimumArea);
        refresh();
    });
    QObject::connect(&save, &QPushButton::clicked, &window, [&] {
        CVLAB_TRACE();
        const auto path = QFileDialog::getSaveFileName(&window, QObject::tr("保存PNG"), QString(), QStringLiteral("PNG (*.png)"));
        if (!path.isEmpty()) { submitExport(path); }
    });
    refresh();
    const auto waitForResult = [&] {
        QElapsedTimer timer;
        timer.start();
        while (acceptedRevision != revision && timer.elapsed() < 10000 && !postFailed) {
            QCoreApplication::processEvents();
            QThread::msleep(1);
        }
        return acceptedRevision == revision && !postFailed;
    };
    int exitCode = 0;
    if (application.arguments().contains(QStringLiteral("--self-test"))) {
        if (!waitForResult() || cv::countNonZero(result) != 720 || original.pixmap().isNull() || mask.pixmap().isNull()) { exitCode = 2; }
        bool emptyRejected = false;
        try { static_cast<void>(workbench::processImage(cv::Mat{}, parameters, {})); }
        catch (const std::invalid_argument&) { emptyRejected = true; }
        if (!emptyRejected) { exitCode = 19; }
        std::stop_source stopSource;
        stopSource.request_stop();
        bool stopRejected = false;
        try { static_cast<void>(workbench::processImage(input, parameters, stopSource.get_token())); }
        catch (const std::runtime_error&) { stopRejected = true; }
        if (!stopRejected) { exitCode = 20; }
        cv::Mat tooMany(100, 100, CV_8UC1, cv::Scalar(0));
        for (int y = 1; y < tooMany.rows; y += 3) {
            auto* row = tooMany.ptr<uchar>(y);
            for (int x = 1; x < tooMany.cols; x += 3) { row[x] = 255; }
        }
        bool limitRejected = false;
        try { static_cast<void>(workbench::processImage(tooMany, parameters, {})); }
        catch (const std::length_error&) { limitRejected = true; }
        if (!limitRejected) { exitCode = 28; }
        threshold.setValue(250);
        if (!waitForResult() || parameters.threshold != 250 || cv::countNonZero(result) != 0) { exitCode = 3; }
        threshold.setValue(100);
        if (!waitForResult() || parameters.threshold != 100 || cv::countNonZero(result) != 720) { exitCode = 4; }
        QTemporaryDir temporary;
        if (!temporary.isValid()) { exitCode = 7; }
        else {
            const auto configPath = temporary.filePath(QStringLiteral("参数-测试.json"));
            Parameters loaded;
            if (!saveParameters(configPath, parameters) || !loadParameters(configPath, loaded)
                || loaded.threshold != 100 || loaded.minimumArea != 1) { exitCode = 8; }
            Parameters badParameters = loaded;
            badParameters.threshold = 256;
            if (saveParameters(configPath, badParameters) || !loadParameters(configPath, loaded)
                || loaded.threshold != 100 || loaded.minimumArea != 1) { exitCode = 21; }
            QFile invalid(configPath);
            if (!invalid.open(QIODevice::WriteOnly | QIODevice::Truncate)
                || invalid.write("{\"version\":1,\"threshold\":256,\"minimumArea\":1}") < 0) { exitCode = 10; }
            invalid.close();
            if (loadParameters(configPath, loaded) || loaded.threshold != 100 || loaded.minimumArea != 1) { exitCode = 11; }
        }
        minimumArea.setValue(241);
        if (!waitForResult() || components.rowCount() != 0 || cv::countNonZero(result) != 0) { exitCode = 5; }
        minimumArea.setValue(240);
        if (!waitForResult() || components.rowCount() != 3 || cv::countNonZero(result) != 720
            || components.item(0, 4)->text() != QStringLiteral("12×20")) { exitCode = 6; }
        threshold.setValue(250);
        threshold.setValue(100);
        if (!waitForResult() || cv::countNonZero(result) != 720) { exitCode = 12; }
        const auto originalInput = input.clone();
        refresh();
        input.setTo(0);
        if (!waitForResult() || cv::countNonZero(result) != 720
            || original.pixmap().toImage().pixelColor(0, 0).red() != 20) { exitCode = 13; }
        input = originalInput;
        cv::Mat many(1800, 1800, CV_8UC1, cv::Scalar(0));
        for (int y = 1; y < many.rows; y += 3) {
            auto* row = many.ptr<uchar>(y);
            for (int x = 1; x < many.cols; x += 3) { row[x] = 255; }
        }
        input = many;
        minimumArea.setValue(2);
        const auto heavyRevision = revision.load();
        QElapsedTimer startTimer; startTimer.start();
        while (startedRevision.load() != heavyRevision && startTimer.elapsed() < 5000) { QCoreApplication::processEvents(); QThread::msleep(1); }
        if (startedRevision.load() != heavyRevision) { exitCode = 22; }
        if (completedRevision.load() == heavyRevision) { exitCode = 24; }
        cancel.click();
        input = originalInput;
        minimumArea.setValue(240);
        if (!waitForResult() || cv::countNonZero(result) != 720) { exitCode = 23; }
        const auto outputPath = temporary.filePath(QStringLiteral("掩模-测试.png"));
        submitExport(outputPath);
        if (cancel.isEnabled() || threshold.isEnabled() || minimumArea.isEnabled()) { exitCode = 25; }
        if (!waitForResult() || status.text() != QObject::tr("导出成功")) { exitCode = 16; }
        if (!cancel.isEnabled() || !threshold.isEnabled()) { exitCode = 26; }
        QFile outputFile(outputPath);
        if (!outputFile.open(QIODevice::ReadOnly)) { exitCode = 17; }
        else {
            const auto bytes = outputFile.readAll();
            const std::vector<uchar> encoded(bytes.begin(), bytes.end());
            const auto decoded = cv::imdecode(encoded, cv::IMREAD_GRAYSCALE);
            if (decoded.empty() || cv::countNonZero(decoded) != 720) { exitCode = 18; }
        }
        submitExport(temporary.filePath(QStringLiteral("missing-directory/result.png")));
        if (!waitForResult() || status.text() != QObject::tr("导出失败，请检查路径")
            || !save.isEnabled() || cv::countNonZero(result) != 720) { exitCode = 27; }
        inputPath = temporary.filePath(QStringLiteral("missing.png"));
        refresh();
        if (!waitForResult() || status.text() != QObject::tr("处理失败，请检查图像与参数")
            || cv::countNonZero(result) != 720) { exitCode = 15; }
        refresh();
        cancel.click();
        QCoreApplication::processEvents();
        if (acceptedRevision == revision || status.text() != QObject::tr("已取消；保留上一次结果")) { exitCode = 14; }
        inputPath.clear();
        input = originalInput;
        reset.click();
        if (!waitForResult() || parameters.threshold != 100 || parameters.minimumArea != 1
            || threshold.value() != 100 || minimumArea.value() != 1 || cv::countNonZero(result) != 720) { exitCode = 29; }
        if (exitCode == 0) { std::cout << "PASS: bounded worker, stale results, cancel, model, JSON and independent image\n"; }
    } else {
        window.show();
        exitCode = application.exec();
    }
    { std::lock_guard lock(queueMutex); activeCancellation.request_stop(); }
    worker.request_stop();
    ready.notify_all();
    worker.join();
    return exitCode;
}

/// \brief 初始化日志和 Qt 应用，并在追踪作用域结束后排空日志。
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    if (!app::initializeLogging()) { return 1; }
    int exitCode = 1;
    {
        CVLAB_TRACE();
        try { exitCode = runWorkbench(application); }
        catch (const std::exception&) { exitCode = app::reportFailure("workbench-failed"); }
    }
    return app::finishLogging(exitCode);
}
