/**
 * @file Logger.h
 * @brief 异步文件日志与 Debug 作用域追踪；日志内部禁止递归追踪。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-10-20 16:55:25 +08:00 wbliu
 */
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>

namespace app::core {
inline constexpr int kLogRetentionDays = 7; ///< 本程序日志按最后修改时间保留的天数。
/// \brief 调用方向日志封装提交的消息严重性。
enum class LogLevel {
    kDebug, ///< 调试消息；Release 构建不入队。
    kInfo, ///< 正常运行中的关键事件。
    kWarning, ///< 可继续运行但需要关注的事件。
    kError ///< 操作失败或需要处理的错误。
};
/// \brief 日志初始化、提交或后台写入发生的错误类别。
enum class LogError {
    kDirectory, ///< 目录准备或过期日志清理失败。
    kOpen, ///< 日志文件或后端初始化失败。
    kWrite, ///< 消息提交、后台写入或刷新失败。
    kNotStarted ///< 在未启动或已停止状态提交了消息。
};
class LoggerPrivate;

/// \brief 封装进程日志的生命周期与线程安全提交，隐藏 spdlog 类型和所有权。
class Logger final {
public:
    /**
     * @brief 获取延迟创建的进程级日志器，该对象不继承 QObject。
     * @return 持续到进程退出的日志器引用，调用者不得销毁该对象。
     */
    static Logger& instance();
    /**
     * @brief 排空并停止日志后端，在进程级实例销毁时释放资源。
     * @note 析构前应已结束日志生产线程。
     */
    ~Logger();
    /**
     * @brief 禁止复制日志器及其后端所有权。
     * @note 源日志器引用为匿名参数；本函数已删除，不可调用。
     */
    Logger(const Logger&) = delete;
    /**
     * @brief 禁止通过赋值复制日志器及其后端所有权。
     * @return 本运算符已删除，不会产生返回引用。
     * @note 源日志器引用为匿名参数；本运算符不可调用。
     */
    Logger& operator=(const Logger&) = delete;
    /**
     * @brief 在目录中开始日志，清理超过保留天数的本程序日志。
     * @param directory 日志目录；调用方应使用可执行文件旁的 logs。
     * @return 初始化错误；成功返回 std::nullopt，并清空之前的错误状态。
     * @note 由应用线程串行调用；重启前先结束其他生产者，旧队列会先排空。
     */
    std::optional<LogError> start(const std::filesystem::path& directory);
    /**
     * @brief 向异步队列写入消息；可从任意工作线程调用。
     * @param level 严重性。
     * @param message Release 应只传脱敏事件和错误码。
     * @param location 自动捕获的调用位置。
     * @note 多线程可并行提交；队列满时阻塞等待，不覆盖已接受的消息。
     * @note Release 会忽略 kDebug；消息作为字面文本处理，错误可通过 lastError 查询。
     */
    void write(LogLevel level, std::string_view message,
               const std::source_location& location = std::source_location::current()) noexcept;
    /**
     * @brief 等待已接受消息完成落盘并停止，允许重复调用。
     * @note 由应用线程串行调用，调用前先结束其他生产者；不与 start 或其他 stop 并行。
     */
    void stop() noexcept;
    /**
     * @brief 读取最近错误，支持从任意线程查询。
     * @return 最近一次记录的错误，无错误时返回 std::nullopt。
     * @note 后台错误可能稍后到达；stop 完成后可检查本轮最终写入状态。
     */
    std::optional<LogError> lastError() const noexcept;

private:
    /**
     * @brief 创建空闲日志状态，由 instance 延迟调用，不立即创建文件或工作线程。
     */
    Logger();
    std::unique_ptr<LoggerPrivate> clp_state; ///< 独占后端状态；其线程池在 stop 中排空后释放。
};

/// \brief 以作用域生命周期记录 Debug 函数进入和退出，覆盖提前返回及栈展开。
class ScopeTrace final {
public:
    /**
     * @brief 记录函数进入。
     * @param location 函数、文件与行号，不包含业务数据。
     */
    explicit ScopeTrace(const std::source_location& location = std::source_location::current()) noexcept;
    /**
     * @brief Debug 中记录函数退出；Release 不产生追踪消息。
     */
    ~ScopeTrace();
    /**
     * @brief 禁止复制作用域追踪，避免重复退出记录。
     * @note 源追踪对象引用为匿名参数；本函数已删除，不可调用。
     */
    ScopeTrace(const ScopeTrace&) = delete;
    /**
     * @brief 禁止赋值作用域追踪，避免破坏进入与退出的配对关系。
     * @return 本运算符已删除，不会产生返回引用。
     * @note 源追踪对象引用为匿名参数；本运算符不可调用。
     */
    ScopeTrace& operator=(const ScopeTrace&) = delete;
private:
    std::source_location cl_location; ///< 构造时捕获的调用位置，用于配对记录进入和退出。
};
} // namespace app::core

#ifndef NDEBUG
#define CVLAB_TRACE() const app::core::ScopeTrace scopeTrace
#else
#define CVLAB_TRACE() ((void)0)
#endif
