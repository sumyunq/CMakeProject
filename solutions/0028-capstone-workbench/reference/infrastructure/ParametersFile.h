/**
 * @file ParametersFile.h
 * @brief 工作台参数文件读写接口。
 * @copyright Copyright (c) 2026 wbliu
 * SPDX-License-Identifier: MIT
 * @date 2026-11-02 11:12:46 +08:00 wbliu
 */
#pragma once

class QString;
namespace workbench {
struct Parameters;
/**
 * @brief 原子保存参数到JSON文件。
 * @param path 输出文件路径，可含Unicode字符。
 * @param parameters 待保存的参数。
 * @return 提交成功为true，任何文件错误为false。
 * @note 无共享可变状态；同一路径的并发写入由调用者协调。
 */
bool saveParameters(const QString& path, const Parameters& parameters);
/**
 * @brief 校验并加载JSON参数，失败时不修改原参数。
 * @param path 输入文件路径，可含Unicode字符。
 * @param parameters 成功时替换的调用方参数。
 * @return 文件与全部字段有效为true，否则false。
 * @note 调用者负责同步对同一参数对象的访问。
 */
bool loadParameters(const QString& path, Parameters& parameters);
} // namespace workbench
