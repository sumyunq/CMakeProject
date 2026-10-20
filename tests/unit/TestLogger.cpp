// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
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
    QVERIFY(!contents.contains("[DEBUG]"));
#else
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

QTEST_GUILESS_MAIN(TestLogger)
#include "TestLogger.moc"
