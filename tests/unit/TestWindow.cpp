// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-10-20 16:01:48 +08:00 wbliu
#include <QtCore/QDir>
#include <QtGui/QColor>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtTest/QtTest>

#include "presentation/widgets/MainWindow.h"

class TestWindow : public QObject {
    Q_OBJECT
private slots:
    void show_generatedImage_displaysKnownColors();
    void closeButton_clicked_closesWindow();
    void show_chineseLabels_hasRequiredGlyph();
};

void TestWindow::show_generatedImage_displaysKnownColors()
{
    app::presentation::MainWindow window;
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    const auto* label = window.findChild<QLabel*>(QStringLiteral("imageLabel"));
    QVERIFY(label != nullptr);
    const auto pixels = label->pixmap().toImage();
    QCOMPARE(pixels.size(), QSize(240, 120));
    QCOMPARE(pixels.pixelColor(10, 10), QColor(255, 0, 0));
    QCOMPARE(pixels.pixelColor(90, 10), QColor(0, 255, 0));
    QCOMPARE(pixels.pixelColor(170, 10), QColor(0, 0, 255));
    const auto* properties = window.findChild<QLabel*>(QStringLiteral("propertiesLabel"));
    QVERIFY(properties != nullptr);
    QVERIFY(properties->text().contains(QStringLiteral("240")));
    QVERIFY(properties->text().contains(QStringLiteral("120")));
    QVERIFY(properties->text().contains(QStringLiteral("CV_8UC3")));
    QVERIFY(window.grab().save(QStringLiteral("viewer-preview.png")));
}
void TestWindow::closeButton_clicked_closesWindow()
{
    app::presentation::MainWindow window;
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    auto* button = window.findChild<QPushButton*>(QStringLiteral("closeButton"));
    QVERIFY(button != nullptr);
    QTest::mouseClick(button, Qt::LeftButton);
    QVERIFY(!window.isVisible());
}
void TestWindow::show_chineseLabels_hasRequiredGlyph()
{
    app::presentation::MainWindow window;
    const auto* title = window.findChild<QLabel*>(QStringLiteral("titleLabel"));
    QVERIFY(title != nullptr);
    QVERIFY(QFontMetrics(title->font()).inFontUcs4(U'像'));
}
QTEST_MAIN(TestWindow)
#include "TestWindow.moc"
