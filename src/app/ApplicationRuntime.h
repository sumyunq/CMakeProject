/**
 * @file ApplicationRuntime.h
 * @brief 应用入口的日志生命周期与失败报告。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:55:25 +08:00 wbliu
 */
#pragma once

namespace app {
/**
 * @brief 在可执行文件旁的 logs 目录初始化日志，并记录应用启动事件。
 * @return 日志初始化成功返回 true；后端报告初始化错误时向标准错误输出并返回 false。
 * @note 仅在应用线程调用；应先构造 QCoreApplication 或 QApplication。
 */
bool initializeLogging();
/**
 * @brief 记录固定错误事件，不输出业务路径或异常敏感内容。
 * @param event 非空的固定内部事件代码，不允许传入用户输入。
 * @return 固定失败退出码 1，供应用入口返回。
 */
int reportFailure(const char* event);
/**
 * @brief 排空日志并检查后台错误；在所有作用域追踪和生产线程结束后调用。
 * @param exitCode 原退出码。
 * @return 日志失败时非零，否则原退出码。
 */
int finishLogging(int exitCode);
} // namespace app
