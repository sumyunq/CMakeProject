// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:55:25 +08:00 wbliu
#include "core/diagnostics/Logger.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <regex>
#include <sstream>
#include <stdexcept>

#include <spdlog/async_logger.h>
#include <spdlog/details/thread_pool.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace app::core {
namespace {
constexpr int kNoError = -1;
constexpr std::size_t kAsyncQueueCapacity = 8192;
constexpr std::size_t kAsyncWorkerCount = 1;
constexpr auto kLogPattern = "%Y-%m-%d %H:%M:%S.%e [%l] [%s:%# %!] %v";

/// \brief 生成精确到秒的本地日志文件名；时间转换失败交由初始化入口报告。
std::string timestamp()
{
    const auto clockTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm local{};
    if (localtime_s(&local, &clockTime) != 0) {
        throw std::runtime_error("Cannot format log time");
    }
    std::ostringstream output;
    output << std::put_time(&local, "%Y-%m-%d-%H-%M-%S");
    return output.str();
}

/// \brief 将封装的严重性映射到 spdlog，保持调用方不依赖第三方类型。
spdlog::level::level_enum backendLevel(LogLevel level) noexcept
{
    switch (level) {
    case LogLevel::kDebug: return spdlog::level::debug;
    case LogLevel::kInfo: return spdlog::level::info;
    case LogLevel::kWarning: return spdlog::level::warn;
    case LogLevel::kError: return spdlog::level::err;
    }
    return spdlog::level::err;
}

/// \brief 仅清理本程序时间命名、修改时间超过保留期的日志，失败不静默忽略。
bool removeExpiredLogs(const std::filesystem::path& directory)
{
    const std::wregex ownLogName(LR"(^\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2}\.log$)");
    const auto cutoff = std::filesystem::file_time_type::clock::now() - std::chrono::days(kLogRetentionDays);
    std::error_code error;
    auto iterator = std::filesystem::directory_iterator(directory, error);
    const auto end = std::filesystem::directory_iterator{};
    for (; iterator != end && !error; iterator.increment(error)) {
        const auto& entry = *iterator;
        const bool regular = entry.is_regular_file(error);
        if (error) { return false; }
        if (!regular || !std::regex_match(entry.path().filename().wstring(), ownLogName)) {
            continue;
        }
        const auto modified = entry.last_write_time(error);
        if (error) { return false; }
        if (modified < cutoff && (!std::filesystem::remove(entry.path(), error) || error)) {
            return false;
        }
    }
    return !error;
}
} // namespace

/// \brief 拥有本日志器专属的 spdlog 后端与错误状态，不注册全局日志器。
class LoggerPrivate {
public:
    std::mutex cl_mutex; ///< 同步后端发布、入队和停止，不在错误回调中获取。
    std::shared_ptr<spdlog::details::thread_pool> clp_pool; ///< 专属有界队列及单消费者线程。
    std::shared_ptr<spdlog::async_logger> clp_logger; ///< 已完成初始化才对生产线程可见。
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> clp_sink; ///< 用于排空后同步复核 flush。
    std::atomic<int> cl_error{kNoError}; ///< 后台可报告错误，负一表示当前无错误。

    void reportWriteFailure() noexcept;
};

/// \brief 后台错误用原子状态及固定标准错误消息报告，避免递归写入日志。
void LoggerPrivate::reportWriteFailure() noexcept
{
    if (cl_error.exchange(static_cast<int>(LogError::kWrite)) != static_cast<int>(LogError::kWrite)) {
        std::fputs("CVLab: log write failed; check logs directory.\n", stderr);
    }
}

/// \brief 创建不持有文件或线程的空闲日志状态。
Logger::Logger() : clp_state(std::make_unique<LoggerPrivate>()) {}

/// \brief 正常析构时完成队列排空，不影响进程中其他 spdlog 日志器。
Logger::~Logger() { stop(); }

/// \brief 延迟构造非 QObject 单例，避免静态 QObject 的初始化顺序问题。
Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

/// \brief 串行准备目录和保留期，完整初始化专属 spdlog 后端后再发布给生产线程。
std::optional<LogError> Logger::start(const std::filesystem::path& directory)
{
    stop();
    auto& state = *clp_state;
    state.cl_error = kNoError;
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    if (error || !std::filesystem::is_directory(directory, error) || error) {
        state.cl_error = static_cast<int>(LogError::kDirectory);
        return LogError::kDirectory;
    }
    try {
        if (!removeExpiredLogs(directory)) {
            state.cl_error = static_cast<int>(LogError::kDirectory);
            return LogError::kDirectory;
        }
        const auto path = directory / (timestamp() + ".log");
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.native(), false);
        auto pool = std::make_shared<spdlog::details::thread_pool>(kAsyncQueueCapacity, kAsyncWorkerCount);
        auto logger = std::make_shared<spdlog::async_logger>("cvlab", sink, pool, spdlog::async_overflow_policy::block);
        logger->set_pattern(kLogPattern);
        logger->set_error_handler([&state](const std::string&) { state.reportWriteFailure(); });
#ifdef NDEBUG
        logger->set_level(spdlog::level::info);
#else
        logger->set_level(spdlog::level::debug);
#endif
        {
            std::lock_guard lock(state.cl_mutex);
            state.clp_sink = std::move(sink);
            state.clp_pool = std::move(pool);
            state.clp_logger = std::move(logger);
        }
        return std::nullopt;
    } catch (...) {
        state.cl_error = static_cast<int>(LogError::kOpen);
        return LogError::kOpen;
    }
}

/// \brief 将消息按字面量格式化并入队；满队列等待消费者，不覆盖已接受消息。
void Logger::write(LogLevel level, std::string_view message, const std::source_location& location) noexcept
{
#ifdef NDEBUG
    if (level == LogLevel::kDebug) { return; }
#endif
    auto& state = *clp_state;
    try {
        std::lock_guard lock(state.cl_mutex);
        if (!state.clp_logger) {
            state.cl_error = static_cast<int>(LogError::kNotStarted);
            return;
        }
        const spdlog::source_loc source(location.file_name(), static_cast<int>(location.line()), location.function_name());
        // 消息作为格式参数传入，消息自身的花括号不会被当作格式表达式。
        state.clp_logger->log(source, backendLevel(level), "{}", message);
    } catch (...) {
        state.reportWriteFailure();
    }
}

/// \brief 停止接收后排空专属线程池，再同步检查落盘；不能只调用异步 flush 后立即退出。
void Logger::stop() noexcept
{
    auto& state = *clp_state;
    try {
        std::shared_ptr<spdlog::async_logger> logger;
        std::shared_ptr<spdlog::details::thread_pool> pool;
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> sink;
        {
            std::lock_guard lock(state.cl_mutex);
            logger = std::move(state.clp_logger);
            pool = std::move(state.clp_pool);
            sink = std::move(state.clp_sink);
        }
        if (logger) {
            logger->flush();
            // 单消费者的终止消息排在 flush 之后；线程池析构会等待已入队任务完成。
            pool.reset();
            sink->flush();
        }
    } catch (...) {
        state.reportWriteFailure();
    }
}

/// \brief 原子读取最近错误；成功重启时清空，停止本身不会抹去失败记录。
std::optional<LogError> Logger::lastError() const noexcept
{
    const auto value = clp_state->cl_error.load();
    if (value == kNoError) { return std::nullopt; }
    return static_cast<LogError>(value);
}

/// \brief Debug 中记录函数进入；Release 不产生作用域追踪消息。
ScopeTrace::ScopeTrace(const std::source_location& location) noexcept : cl_location(location)
{
#ifndef NDEBUG
    Logger::instance().write(LogLevel::kDebug, "enter", cl_location);
#endif
}

/// \brief 所有离开路径通过析构记录函数退出，包含提前返回及栈展开。
ScopeTrace::~ScopeTrace()
{
#ifndef NDEBUG
    Logger::instance().write(LogLevel::kDebug, "exit", cl_location);
#endif
}
} // namespace app::core
