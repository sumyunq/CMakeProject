// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:55:25 +08:00 wbliu
#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTemporaryDir>
#include <QtTest/QtTest>
#include "core/diagnostics/Logger.h"
class TestLogger : public QObject {
    Q_OBJECT
private slots:
    void logger_concurrentWrites_stopDrainsQueue();
    void logger_invalidDirectory_reportsError();
    void logger_retention_removesOnlyExpiredProgramLogs();
    void logger_queuePressure_stopPreservesAllMessages();
    void logger_restart_drainsOldDestination();
    void logger_unicodePathAndMessage_preservesLiteralText();
    void logger_notStarted_reportsError();
};
void TestLogger::logger_concurrentWrites_stopDrainsQueue()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    auto& logger = app::core::Logger::instance();
    QVERIFY(!logger.start(temporary.path().toStdWString()).has_value());
    {
        const app::core::ScopeTrace trace;
        logger.write(app::core::LogLevel::kInfo, "main-marker");
    }
    std::vector<std::jthread> producers;
    for (int producer = 0; producer < 4; ++producer) {
        producers.emplace_back([&logger]() {
            for (int message = 0; message < 20; ++message) {
                logger.write(app::core::LogLevel::kInfo, "worker-marker");
            }
        });
    }
    producers.clear();
    logger.stop();
    QVERIFY(!logger.lastError().has_value());
    const auto files = QDir(temporary.path()).entryList({QStringLiteral("*.log")}, QDir::Files);
    QCOMPARE(files.size(), 1);
    QFile output(temporary.filePath(files.front()));
    QVERIFY(output.open(QIODevice::ReadOnly));
    const auto contents = output.readAll();
    QCOMPARE(contents.count("worker-marker"), 80);
    QVERIFY(contents.contains("main-marker"));
#ifdef NDEBUG
    QVERIFY(!contents.toUpper().contains("[DEBUG]"));
#else
    QVERIFY(contents.toUpper().contains("[DEBUG]"));
    QVERIFY(contents.contains("enter"));
    QVERIFY(contents.contains("exit"));
#endif
    logger.stop();
}
void TestLogger::logger_invalidDirectory_reportsError()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    QFile file(temporary.filePath(QStringLiteral("blocked")));
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.close();
    auto& logger = app::core::Logger::instance();
    const auto error = logger.start(file.fileName().toStdWString());
    QVERIFY(error.has_value());
    QCOMPARE(*error, app::core::LogError::kDirectory);
    logger.stop();
}
void TestLogger::logger_retention_removesOnlyExpiredProgramLogs()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    const std::filesystem::path directory(temporary.path().toStdWString());
    const auto expired = directory / "2020-01-01-00-00-00.log";
    const auto unrelated = directory / "notes.log";
    const auto recent = directory / "2026-10-20-00-00-00.log";
    for (const auto& path : {expired, unrelated, recent}) {
        std::ofstream fixture(path);
        QVERIFY(fixture.good());
    }
    const auto oldTime = std::filesystem::file_time_type::clock::now() - std::chrono::days(8);
    std::filesystem::last_write_time(expired, oldTime);
    std::filesystem::last_write_time(unrelated, oldTime);
    auto& logger = app::core::Logger::instance();
    QVERIFY(!logger.start(directory).has_value());
    logger.stop();
    QVERIFY(!std::filesystem::exists(expired));
    QVERIFY(std::filesystem::exists(unrelated));
    QVERIFY(std::filesystem::exists(recent));
}

void TestLogger::logger_queuePressure_stopPreservesAllMessages()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    auto& logger = app::core::Logger::instance();
    QVERIFY(!logger.start(temporary.path().toStdWString()).has_value());
    constexpr int kProducerCount = 4;
    constexpr int kMessagesPerProducer = 3000;
    {
        std::vector<std::jthread> producers;
        for (int producer = 0; producer < kProducerCount; ++producer) {
            producers.emplace_back([&logger]() {
                for (int message = 0; message < kMessagesPerProducer; ++message) {
                    logger.write(app::core::LogLevel::kInfo, "pressure-marker");
                }
            });
        }
    }
    logger.stop();
    QVERIFY(!logger.lastError().has_value());
    const auto files = QDir(temporary.path()).entryList({QStringLiteral("*.log")}, QDir::Files);
    QCOMPARE(files.size(), 1);
    QFile output(temporary.filePath(files.front()));
    QVERIFY(output.open(QIODevice::ReadOnly));
    QCOMPARE(output.readAll().count("pressure-marker"), kProducerCount * kMessagesPerProducer);
}
void TestLogger::logger_restart_drainsOldDestination()
{
    QTemporaryDir first;
    QTemporaryDir second;
    QVERIFY(first.isValid());
    QVERIFY(second.isValid());
    auto& logger = app::core::Logger::instance();
    QVERIFY(!logger.start(first.path().toStdWString()).has_value());
    logger.write(app::core::LogLevel::kInfo, "first-destination");
    QVERIFY(!logger.start(second.path().toStdWString()).has_value());
    logger.write(app::core::LogLevel::kWarning, "second-destination");
    logger.stop();
    for (const auto* directory : {&first, &second}) {
        const auto files = QDir(directory->path()).entryList({QStringLiteral("*.log")}, QDir::Files);
        QCOMPARE(files.size(), 1);
        QFile output(directory->filePath(files.front()));
        QVERIFY(output.open(QIODevice::ReadOnly));
        const auto content = output.readAll();
        QCOMPARE(content.contains("first-destination"), directory == &first);
        QCOMPARE(content.contains("second-destination"), directory == &second);
    }
}
void TestLogger::logger_unicodePathAndMessage_preservesLiteralText()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    const auto directory = temporary.filePath(QStringLiteral("日志目录"));
    auto& logger = app::core::Logger::instance();
    QVERIFY(!logger.start(directory.toStdWString()).has_value());
    logger.write(app::core::LogLevel::kError, "literal {id} 50% 中文");
    logger.stop();
    QVERIFY(!logger.lastError().has_value());
    const auto files = QDir(directory).entryList({QStringLiteral("*.log")}, QDir::Files);
    QCOMPARE(files.size(), 1);
    QFile output(QDir(directory).filePath(files.front()));
    QVERIFY(output.open(QIODevice::ReadOnly));
    const auto content = output.readAll();
    QVERIFY(content.contains("literal {id} 50% 中文"));
    QVERIFY(content.contains("TestLogger.cpp:"));
    QVERIFY(content.contains("logger_unicodePathAndMessage_preservesLiteralText"));
    QVERIFY(content.toUpper().contains("[ERROR]"));
}
void TestLogger::logger_notStarted_reportsError()
{
    QTemporaryDir temporary;
    QVERIFY(temporary.isValid());
    auto& logger = app::core::Logger::instance();
    logger.stop();
    logger.write(app::core::LogLevel::kInfo, "after-stop");
    QCOMPARE(logger.lastError(), std::optional(app::core::LogError::kNotStarted));
    QVERIFY(!logger.start(temporary.path().toStdWString()).has_value());
    QVERIFY(!logger.lastError().has_value());
    logger.stop();
}

QTEST_GUILESS_MAIN(TestLogger)
#include "TestLogger.moc"
