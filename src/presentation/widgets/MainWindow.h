/*!
 * @file MainWindow.h
 * @brief 首批课程的图像与元数据验证窗口。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:01:48 +08:00 wbliu
 */
#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>

namespace app::presentation {
class MainWindowPrivate;
class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    /*!
     * @brief 创建验证窗口，仅在 GUI 线程调用。
     * @param parent 可选 Qt 父窗口；内部控件由 Qt 对象树管理。
     * @note 初始图像由模型生成，不读取外部文件。
     */
    explicit MainWindow(QWidget* parent = nullptr);
    /// @brief 释放非 QObject 状态，子控件由 Qt 父对象清理。
    ~MainWindow() override;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
private:
    void initializeStyle();
    void initializeConnections();
    void initializeImage();
    std::unique_ptr<MainWindowPrivate> clp_state;
};
} // namespace app::presentation
