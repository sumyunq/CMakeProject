/**
 * @file MainWindow.h
 * @brief 首批课程的图像与元数据验证窗口。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:55:25 +08:00 wbliu
 */
#pragma once

#include <memory>

#include <QtWidgets/QMainWindow>

namespace app::presentation {
class MainWindowPrivate;
/// \brief 在 GUI 线程展示程序生成的测试图像和业务层提供的元数据。
class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief 创建验证窗口，仅在 GUI 线程调用。
     * @param parent 可选 Qt 父窗口；内部控件由 Qt 对象树管理。
     * @note 初始图像由模型生成，不读取外部文件。
     */
    explicit MainWindow(QWidget* parent = nullptr);
    /**
     * @brief 释放独占的非 QObject 状态，子控件交由 Qt 父对象清理。
     */
    ~MainWindow() override;
    /**
     * @brief 禁止复制窗口及其 QObject 所有权。
     * @note 源窗口引用为匿名参数；本函数已删除，不可调用。
     */
    MainWindow(const MainWindow&) = delete;
    /**
     * @brief 禁止通过赋值复制窗口及其 QObject 所有权。
     * @return 本运算符已删除，不会产生返回引用。
     * @note 源窗口引用为匿名参数；本运算符不可调用。
     */
    MainWindow& operator=(const MainWindow&) = delete;
private:
    /**
     * @brief 从内嵌资源读取并应用 QSS，加载失败向构造调用方抛出异常。
     */
    void initializeStyle();
    /**
     * @brief 集中连接关闭按钮等同线程 UI 交互，不依赖自动槽命名。
     */
    void initializeConnections();
    /**
     * @brief 初始化 BGR 测试图像和属性模型，再更新图像与属性显示。
     */
    void initializeImage();
    std::unique_ptr<MainWindowPrivate> clp_state; ///< 独占图像、属性及 UI 引用，子控件由当前窗口管理。
};
} // namespace app::presentation
