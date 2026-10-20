// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
// 2026-10-20 15:36:00 +08:00 wbliu
#include <filesystem>
#include <fstream>
#include <iterator>
#include <thread>
#include <vector>

#include <QtCore/QTemporaryDir>
#include <QtGui/QColor>
#include <QtTest/QtTest>

#include <opencv2/core.hpp>

#include "business/services/ImageInspection.h"
#include "core/diagnostics/Logger.h"
#include "presentation/widgets/ImageConversion.h"

class TestLearning : public QObject {
    Q_OBJECT
private slots:
    void inspectImage_inputs_reportsProperties_data();
    void inspectImage_inputs_reportsProperties();
    void inspectImage_volume_returnsNoProperties();
    void toOwnedQImage_supportedPixels_preservesColor_data();
    void toOwnedQImage_supportedPixels_preservesColor();
    void toOwnedQImage_invalidInputs_returnsError_data();
    void toOwnedQImage_invalidInputs_returnsError();
    void toOwnedQImage_sourceLifetime_ownsPixels_data();
    void toOwnedQImage_sourceLifetime_ownsPixels();
    void toOwnedQImage_sourcePixels_remainUnchanged_data();
    void toOwnedQImage_sourcePixels_remainUnchanged();
    void toOwnedQImage_volume_returnsDimensionError();
    void toOwnedQImage_nonContinuousRoi_preservesStride_data();
    void toOwnedQImage_nonContinuousRoi_preservesStride();
    void matExperiment_copyAndRoi_matchesPredictions();
};

void TestLearning::inspectImage_inputs_reportsProperties_data()
{
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("columns");
    QTest::addColumn<int>("type");
    QTest::addColumn<bool>("valid");
    QTest::newRow("empty") << 0 << 0 << CV_8UC1 << false;
    QTest::newRow("bgr") << 2 << 4 << CV_8UC3 << true;
    QTest::newRow("gray16") << 1 << 5 << CV_16UC1 << true;
}
void TestLearning::inspectImage_inputs_reportsProperties()
{
    QFETCH(int, rows); QFETCH(int, columns); QFETCH(int, type); QFETCH(bool, valid);
    const cv::Mat image(rows, columns, type);
    const auto properties = app::business::inspectImage(image);
    QCOMPARE(properties.has_value(), valid);
    if (valid) {
        QCOMPARE(properties->width, columns);
        QCOMPARE(properties->height, rows);
        QCOMPARE(properties->channels, image.channels());
        QCOMPARE(properties->depth, image.depth());
        QCOMPARE(properties->rowStrideBytes, image.step[0]);
        QVERIFY(properties->continuous);
    }
}
void TestLearning::toOwnedQImage_supportedPixels_preservesColor_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QColor>("expected");
    QTest::newRow("gray") << CV_8UC1 << QColor(17, 17, 17, 255);
    QTest::newRow("bgr") << CV_8UC3 << QColor(99, 31, 17, 255);
    QTest::newRow("bgra") << CV_8UC4 << QColor(99, 31, 17, 127);
}
void TestLearning::inspectImage_volume_returnsNoProperties()
{
    const int dimensions[] = {2, 3, 4};
    const cv::Mat volume(3, dimensions, CV_8UC1);
    QVERIFY(!app::business::inspectImage(volume).has_value());
}
void TestLearning::toOwnedQImage_supportedPixels_preservesColor()
{
    QFETCH(int, type); QFETCH(QColor, expected);
    const cv::Mat pixels(2, 3, type, cv::Scalar(17, 31, 99, 127));
    const auto converted = app::presentation::toOwnedQImage(pixels);
    QVERIFY(std::holds_alternative<QImage>(converted));
    const auto& image = std::get<QImage>(converted);
    QCOMPARE(image.size(), QSize(3, 2));
    QCOMPARE(image.pixelColor(2, 1), expected);
}
void TestLearning::toOwnedQImage_invalidInputs_returnsError_data()
{
    QTest::addColumn<int>("type"); QTest::addColumn<bool>("empty");
    QTest::addColumn<int>("error");
    using Error = app::presentation::ImageConversionError;
    QTest::newRow("empty") << CV_8UC1 << true << static_cast<int>(Error::kEmpty);
    QTest::newRow("twoChannels") << CV_8UC2 << false << static_cast<int>(Error::kUnsupportedType);
    QTest::newRow("sixteenBit") << CV_16UC3 << false << static_cast<int>(Error::kUnsupportedType);
    QTest::newRow("float") << CV_32FC1 << false << static_cast<int>(Error::kUnsupportedType);
}
void TestLearning::toOwnedQImage_invalidInputs_returnsError()
{
    QFETCH(int, type); QFETCH(bool, empty); QFETCH(int, error);
    const auto pixels = empty ? cv::Mat{} : cv::Mat(2, 3, type);
    const auto converted = app::presentation::toOwnedQImage(pixels);
    QVERIFY(std::holds_alternative<app::presentation::ImageConversionError>(converted));
    QCOMPARE(static_cast<int>(std::get<app::presentation::ImageConversionError>(converted)), error);
}
void TestLearning::toOwnedQImage_sourceLifetime_ownsPixels_data()
{
    toOwnedQImage_supportedPixels_preservesColor_data();
}
void TestLearning::toOwnedQImage_sourceLifetime_ownsPixels()
{
    QFETCH(int, type); QFETCH(QColor, expected);
    QImage owned;
    {
        cv::Mat original(2, 3, type, cv::Scalar(17, 31, 99, 127));
        auto converted = app::presentation::toOwnedQImage(original);
        QVERIFY(std::holds_alternative<QImage>(converted));
        owned = std::get<QImage>(converted);
        original.setTo(cv::Scalar(0, 0, 0));
        QCOMPARE(owned.pixelColor(0, 0), expected);
    }
    QCOMPARE(owned.pixelColor(0, 0), expected);
}
void TestLearning::toOwnedQImage_volume_returnsDimensionError()
{
    const int dimensions[] = {2, 3, 4};
    const cv::Mat volume(3, dimensions, CV_8UC1);
    const auto result = app::presentation::toOwnedQImage(volume);
    QVERIFY(std::holds_alternative<app::presentation::ImageConversionError>(result));
    QCOMPARE(std::get<app::presentation::ImageConversionError>(result),
             app::presentation::ImageConversionError::kInvalidDimensions);
}
void TestLearning::toOwnedQImage_sourcePixels_remainUnchanged_data()
{
    toOwnedQImage_supportedPixels_preservesColor_data();
}
void TestLearning::toOwnedQImage_sourcePixels_remainUnchanged()
{
    QFETCH(int, type);
    const cv::Mat source(2, 3, type, cv::Scalar(17, 31, 99, 127));
    const auto snapshot = source.clone();
    const auto result = app::presentation::toOwnedQImage(source);
    QVERIFY(std::holds_alternative<QImage>(result));
    QCOMPARE(cv::norm(source, snapshot, cv::NORM_INF), 0.0);
}
void TestLearning::toOwnedQImage_nonContinuousRoi_preservesStride_data()
{
    toOwnedQImage_supportedPixels_preservesColor_data();
}
void TestLearning::toOwnedQImage_nonContinuousRoi_preservesStride()
{
    QFETCH(int, type); QFETCH(QColor, expected);
    cv::Mat parent(4, 8, type, cv::Scalar(0, 0, 0, 0));
    auto roi = parent(cv::Rect(2, 1, 3, 2));
    roi.setTo(cv::Scalar(17, 31, 99, 127));
    QVERIFY(!roi.isContinuous());
    const auto properties = app::business::inspectImage(roi);
    QVERIFY(properties.has_value());
    QCOMPARE(properties->rowStrideBytes, std::size_t(8) * parent.elemSize());
    QVERIFY(!properties->continuous);
    auto converted = app::presentation::toOwnedQImage(roi);
    QVERIFY(std::holds_alternative<QImage>(converted));
    const auto& owned = std::get<QImage>(converted);
    QCOMPARE(owned.size(), QSize(3, 2));
    QCOMPARE(owned.pixelColor(0, 0), expected);
    QCOMPARE(owned.pixelColor(2, 1), expected);
}
void TestLearning::matExperiment_copyAndRoi_matchesPredictions()
{
    cv::Mat original = cv::Mat::zeros(3, 4, CV_8UC1);
    auto alias = original;
    auto independent = original.clone();
    alias.at<uchar>(0, 1) = 9;
    QCOMPARE(original.at<uchar>(0, 1), uchar(9));
    QCOMPARE(independent.at<uchar>(0, 1), uchar(0));
    auto roi = original(cv::Rect(1, 1, 2, 2));
    roi.setTo(7);
    QCOMPARE(original.at<uchar>(1, 1), uchar(7));
    QCOMPARE(original.at<uchar>(0, 0), uchar(0));
    QVERIFY(!roi.isContinuous());
}
QTEST_MAIN(TestLearning)
#include "TestLearning.moc"
