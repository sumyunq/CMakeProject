/*!
 * @file Logger.h
 * @brief 异步文件日志与 Debug 作用域追踪；日志内部禁止递归追踪。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:01:48 +08:00 wbliu
 */
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>

namespace app::core {
inline constexpr int kLogRetentionDays = 7;
enum class LogLevel { kDebug, kInfo, kWarning, kError };
enum class LogError { kDirectory, kOpen, kWrite, kNotStarted };
class LoggerPrivate;

class Logger final {
public:
    /// @brief 获取非 QObject 的进程级日志器；生命周期持续到进程退出。
    static Logger& instance();
    /// @brief 排空并停止线程；应在其他日志生产线程结束后销毁。
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    /*!
     * @brief 在目录中开始日志，清理超过保留天数的本程序日志。
     * @param directory 日志目录；调用方应使用可执行文件旁的 logs。
     * @return 初始化错误；成功返回空。仅在没有其他生产者时调用。
     */
    std::optional<LogError> start(const std::filesystem::path& directory);
    /*!
     * @brief 向异步队列写入消息；可从任意工作线程调用。
     * @param level 严重性。
     * @param message Release 应只传脱敏事件和错误码。
     * @param location 自动捕获的调用位置。
     */
    void write(LogLevel level, std::string_view message,
               const std::source_location& location = std::source_location::current()) noexcept;
    /// @brief 等待已接收消息全部落盘并停止；需先结束其他生产者，可重复调用。
    void stop() noexcept;
    /// @brief 返回最近错误；无错误为空；可从任意线程查询。
    std::optional<LogError> lastError() const noexcept;

private:
    Logger();
    std::unique_ptr<LoggerPrivate> clp_state;
};

class ScopeTrace final {
public:
    /*!
     * @brief 记录函数进入。
     * @param location 函数、文件与行号，不包含业务数据。
     */
    explicit ScopeTrace(const std::source_location& location = std::source_location::current()) noexcept;
    /// @brief 记录函数退出，包含提前返回与栈展开路径。
    ~ScopeTrace();
    ScopeTrace(const ScopeTrace&) = delete;
    ScopeTrace& operator=(const ScopeTrace&) = delete;
private:
    std::source_location cl_location;
};
} // namespace app::core

#ifndef NDEBUG
#define CVLAB_TRACE() const app::core::ScopeTrace scopeTrace
#else
#define CVLAB_TRACE() ((void)0)
#endif
