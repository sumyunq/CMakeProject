// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "app/ApplicationRuntime.h"
#include "core/diagnostics/Logger.h"
#include "presentation/widgets/MainWindow.h"

/// @brief 仅应用线程创建 Widgets，正常关闭后排空日志。
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    try {
        if (!app::initializeLogging()) {
            QMessageBox::critical(nullptr, QObject::tr("启动失败"), QObject::tr("无法创建日志，请检查程序目录的写入权限。"));
            return 1;
        }
        int result = 0;
        {
            CVLAB_TRACE();
            app::presentation::MainWindow window;
            window.show();
            result = application.exec();
        }
        return app::finishLogging(result);
    } catch (...) {
        const auto result = app::reportFailure("viewer-startup-exception");
        QMessageBox::critical(nullptr, QObject::tr("启动失败"), QObject::tr("图像或资源初始化失败，请检查日志。"));
        return app::finishLogging(result);
    }
}
