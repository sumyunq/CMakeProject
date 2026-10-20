// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include "app/ApplicationRuntime.h"

#include <filesystem>
#include <iostream>

#include <QtCore/QCoreApplication>

#include "core/diagnostics/Logger.h"

namespace app {
/// @brief 必须先初始化日志，后创建任何作用域追踪。
bool initializeLogging()
{
    const std::filesystem::path executableDirectory(QCoreApplication::applicationDirPath().toStdWString());
    const auto error = core::Logger::instance().start(executableDirectory / "logs");
    if (error) {
        std::cerr << "Cannot initialize logs; error=" << static_cast<int>(*error) << '\n';
        return false;
    }
    {
        CVLAB_TRACE();
        core::Logger::instance().write(core::LogLevel::kInfo, "application-started");
    }
    return true;
}

/// @brief 报告可定位但不包含用户数据的固定事件。
int reportFailure(const char* event)
{
    CVLAB_TRACE();
    core::Logger::instance().write(core::LogLevel::kError, event);
    std::cerr << "Operation failed: " << event << '\n';
    return 1;
}

/// @brief 停止日志器自身不再创建追踪，防止停止后再次入队。
int finishLogging(int exitCode)
{
    core::Logger::instance().write(core::LogLevel::kInfo, "application-stopping");
    core::Logger::instance().stop();
    if (core::Logger::instance().lastError()) {
        std::cerr << "Log output failed.\n";
        return 1;
    }
    return exitCode;
}
} // namespace app
