/*!
 * @file ApplicationRuntime.h
 * @brief 应用入口的日志生命周期与失败报告。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:01:48 +08:00 wbliu
 */
#pragma once

namespace app {
/// @brief 在可执行文件旁初始化日志；失败向标准错误报告，返回 false。仅应用线程调用。
bool initializeLogging();
/*!
 * @brief 记录固定错误事件，不输出业务路径或异常敏感内容。
 * @param event 固定的内部事件代码，不允许用户输入。
 * @return 非零退出码。
 */
int reportFailure(const char* event);
/*!
 * @brief 排空日志并检查后台错误；在所有作用域追踪和生产线程结束后调用。
 * @param exitCode 原退出码。
 * @return 日志失败时非零，否则原退出码。
 */
int finishLogging(int exitCode);
} // namespace app
