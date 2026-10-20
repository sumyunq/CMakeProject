// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include "core/diagnostics/Logger.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>

namespace app::core {
namespace {
constexpr int kNoError = -1;

/// @brief 创建线程安全的本地时间字符串；失败通过异常由调用方统一报告。
std::string timestamp(const char* pattern)
{
    const auto now = std::chrono::system_clock::now();
    const auto clockTime = std::chrono::system_clock::to_time_t(now);
    std::tm local{};
    if (localtime_s(&local, &clockTime) != 0) {
        throw std::runtime_error("Cannot format log time");
    }
    std::ostringstream output;
    output << std::put_time(&local, pattern);
    return output.str();
}

/// @brief 返回与严重性对应的固定文本，不包含业务数据。
const char* levelName(LogLevel level) noexcept
{
    switch (level) {
    case LogLevel::kDebug: return "DEBUG";
    case LogLevel::kInfo: return "INFO";
    case LogLevel::kWarning: return "WARNING";
    case LogLevel::kError: return "ERROR";
    }
    return "ERROR";
}
} // namespace

class LoggerPrivate {
public:
    std::mutex mutex;
    std::condition_variable ready;
    std::deque<std::string> queue;
    std::ofstream sink;
    std::jthread worker;
    std::atomic<int> error{kNoError};
    bool accepting = false;

    /// @brief 将无法落盘的错误报告到标准错误，避免日志故障静默丢失。
    void reportWriteFailure() noexcept
    {
        if (error.exchange(static_cast<int>(LogError::kWrite)) != static_cast<int>(LogError::kWrite)) {
            std::fputs("CVLab: log write failed; check logs directory.\n", stderr);
        }
    }

    /// @brief 消费已接收消息；停止后仍排空队列。
    void consume() noexcept
    {
        try {
            for (;;) {
                std::unique_lock lock(mutex);
                ready.wait(lock, [this]() { return !queue.empty() || !accepting; });
                if (queue.empty() && !accepting) {
                    break;
                }
                auto line = std::move(queue.front());
                queue.pop_front();
                lock.unlock();
                sink << line << '\n';
                if (!sink) {
                    reportWriteFailure();
                }
            }
            sink.flush();
            if (!sink) {
                reportWriteFailure();
            }
        } catch (...) {
            reportWriteFailure();
        }
    }
};

/// @brief 初始化空闲的日志状态，不创建 QObject 或文件。
Logger::Logger() : clp_state(std::make_unique<LoggerPrivate>()) {}

/// @brief 正常退出时停止线程并完成落盘。
Logger::~Logger() { stop(); }

/// @brief 延迟构造非 QObject 单例，避免静态 QObject 初始化顺序问题。
Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

/// @brief 建立输出目录并清理本程序的过期日志；初始化与重启由应用线程串行执行。
std::optional<LogError> Logger::start(const std::filesystem::path& directory)
{
    stop();
    auto& state = *clp_state;
    state.error = kNoError;
    std::error_code error;
    std::filesystem::create_directories(directory, error);
    if (error || !std::filesystem::is_directory(directory, error) || error) {
        state.error = static_cast<int>(LogError::kDirectory);
        return LogError::kDirectory;
    }
    try {
        const std::regex ownLogName(R"(^\d{4}-\d{2}-\d{2}-\d{2}-\d{2}-\d{2}\.log$)");
        const auto cutoff = std::filesystem::file_time_type::clock::now() - std::chrono::days(kLogRetentionDays);
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (!entry.is_regular_file() || !std::regex_match(entry.path().filename().string(), ownLogName)) {
                continue;
            }
            if (entry.last_write_time() < cutoff) {
                if (!std::filesystem::remove(entry.path(), error) || error) {
                    state.error = static_cast<int>(LogError::kDirectory);
                    return LogError::kDirectory;
                }
            }
        }
        state.sink.clear();
        state.sink.open(directory / (timestamp("%Y-%m-%d-%H-%M-%S") + ".log"), std::ios::app);
        if (!state.sink) {
            state.error = static_cast<int>(LogError::kOpen);
            return LogError::kOpen;
        }
        state.accepting = true;
        state.worker = std::jthread([&state]() { state.consume(); });
        return std::nullopt;
    } catch (...) {
        state.error = static_cast<int>(LogError::kOpen);
        state.accepting = false;
        if (state.sink.is_open()) {
            state.sink.close();
        }
        return LogError::kOpen;
    }
}

/// @brief 格式化后入队；消息不持有调用方字符串或业务对象。
void Logger::write(LogLevel level, std::string_view message, const std::source_location& location) noexcept
{
#ifdef NDEBUG
    if (level == LogLevel::kDebug) {
        return;
    }
#endif
    auto& state = *clp_state;
    try {
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
        std::ostringstream line;
        line << timestamp("%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << milliseconds
             << " [" << levelName(level) << "] [" << std::filesystem::path(location.file_name()).filename().string()
             << ':' << location.line() << ' ' << location.function_name() << "] " << message;
        {
            std::lock_guard lock(state.mutex);
            if (!state.accepting) {
                state.error = static_cast<int>(LogError::kNotStarted);
                return;
            }
            state.queue.push_back(line.str());
        }
        state.ready.notify_one();
    } catch (...) {
        state.reportWriteFailure();
    }
}

/// @brief 先停止接收，再等待队列清空；不得在生产者仍运行时重启。
void Logger::stop() noexcept
{
    auto& state = *clp_state;
    try {
        {
            std::lock_guard lock(state.mutex);
            state.accepting = false;
        }
        state.ready.notify_one();
        if (state.worker.joinable()) {
            state.worker.join();
        }
        if (state.sink.is_open()) {
            state.sink.close();
            if (state.sink.fail()) {
                state.reportWriteFailure();
            }
        }
    } catch (...) {
        state.reportWriteFailure();
    }
}

/// @brief 以原子状态报告后台错误，避免跨线程读写竞争。
std::optional<LogError> Logger::lastError() const noexcept
{
    const auto value = clp_state->error.load();
    if (value == kNoError) {
        return std::nullopt;
    }
    return static_cast<LogError>(value);
}

/// @brief Debug 中记录进入；Release 不生成函数追踪消息。
ScopeTrace::ScopeTrace(const std::source_location& location) noexcept : cl_location(location)
{
#ifndef NDEBUG
    Logger::instance().write(LogLevel::kDebug, "enter", cl_location);
#endif
}

/// @brief 所有离开路径由析构自动记录。
ScopeTrace::~ScopeTrace()
{
#ifndef NDEBUG
    Logger::instance().write(LogLevel::kDebug, "exit", cl_location);
#endif
}
} // namespace app::core
