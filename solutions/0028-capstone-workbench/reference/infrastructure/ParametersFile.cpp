// Copyright (c) 2026 wbliu. SPDX-License-Identifier: MIT
// Updated: 2026-11-02 11:12:46 +08:00 wbliu
#include "infrastructure/ParametersFile.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QSaveFile>

#pragma warning(push)
#pragma warning(disable: 4127)
#include "business/Processing.h"
#pragma warning(pop)
#include "core/diagnostics/Logger.h"

namespace workbench {
/// \brief 通过临时文件与原子提交保护已存在的参数文件。
bool saveParameters(const QString& path, const Parameters& parameters)
{
    CVLAB_TRACE();
    if (parameters.threshold < 0 || parameters.threshold > 255
        || parameters.minimumArea < 1 || parameters.minimumArea > 100000000) { return false; }
    QJsonObject object;
    object.insert(QStringLiteral("version"), 1);
    object.insert(QStringLiteral("threshold"), parameters.threshold);
    object.insert(QStringLiteral("minimumArea"), parameters.minimumArea);
    const auto bytes = QJsonDocument(object).toJson();
    QSaveFile file(path);
    return file.open(QIODevice::WriteOnly) && file.write(bytes) == bytes.size() && file.commit();
}

/// \brief 验证全部参数之后一次性更新模型。
bool loadParameters(const QString& path, Parameters& parameters)
{
    CVLAB_TRACE();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) { return false; }
    constexpr qint64 kMaximumConfigBytes = 64 * 1024;
    if (file.size() <= 0 || file.size() > kMaximumConfigBytes) { return false; }
    const auto bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) { return false; }
    QJsonParseError error;
    const auto document = QJsonDocument::fromJson(bytes, &error);
    if (error.error != QJsonParseError::NoError || !document.isObject()) { return false; }
    const auto object = document.object();
    const auto version = object.value(QStringLiteral("version"));
    const auto threshold = object.value(QStringLiteral("threshold"));
    const auto minimumArea = object.value(QStringLiteral("minimumArea"));
    if (!version.isDouble() || version.toDouble() != 1 || !threshold.isDouble() || !minimumArea.isDouble()) { return false; }
    const double thresholdValue = threshold.toDouble();
    const double areaValue = minimumArea.toDouble();
    if (thresholdValue < 0 || thresholdValue > 255 || areaValue < 1 || areaValue > 100000000
        || thresholdValue != static_cast<int>(thresholdValue) || areaValue != static_cast<int>(areaValue)) { return false; }
    Parameters candidate;
    candidate.threshold = static_cast<int>(thresholdValue);
    candidate.minimumArea = static_cast<int>(areaValue);
    parameters = candidate;
    return true;
}
} // namespace workbench
