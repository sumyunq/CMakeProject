# 首批验收与自检

日期：2026-10-20；环境：Windows 11 x64。本机安装路径仅在忽略提交的用户预设与构建日志中。

## 结果
- Qt 6.8.3、OpenCV 5.0.0、C++20（202002）、MSVC 14.44.35207 工具集/x64 实测通过；实际编译器完整版本 194435229。
- Debug、Release 均构建通过；每种配置 6/6 组 CTest 通过：日志、图像、窗口、诊断、Mat 演示、课程检查。
- 每种配置的 QtTest 记录：图像 24、日志 5、窗口 5 条通过（包含初始化与清理，不等于 34 个独立行为）。
- 6 个公开头文件分别独立包含编译通过；源文件均显式加入 CMake。
- 灰度/BGR/BGRA 的颜色、透明度、非连续 ROI、源数据不变、源修改/销毁后的独立像素均验证。
- 空图像、不支持的类型、多维张量返回明确结果；图像属性支持 16 位输入，显示转换明确拒绝自动截断。
- 日志验证 4 个生产线程的 80 条消息落盘、停止幂等、错误目录、7 天清理、Release 无 DEBUG 消息。
- 窗口验证红绿蓝色块、业务属性、中文字形、关闭连接；已检查 viewer-preview.png。
- 课程已在浏览器验证导航和答案展开；全部本地资源及链接检查通过。学习状态仍为“未开始”。
- Release 三个程序均在独立部署目录和仅系统 PATH 下运行；窗口正常创建关闭。不是另一台全新机器验收。
- 部署脚本结束后恢复调用进程 PATH 与 Qt 环境变量，已核对部署前后 PATH 相等。

## spdlog 迁移追加验收
- 后端固定为 spdlog 1.17.0#1，通过固定 vcpkg baseline 与项目 triplet 构建静态库、动态 CRT；实际编译器仍为 14.44.35207。
- 独立线程池容量 8192、单消费者、满队列阻塞。停止时请求 flush，销毁线程池等待排空，再同步复核 sink；不使用全局 shutdown。
- Debug/Release 各 6/6 组 CTest 通过；日志专项各 9 条记录通过（含初始化/清理），覆盖 12000 条并发消息、重启排空隔离、中文目录与字面消息、未启动错误、保留期及 Release 过滤。
- 静态链接后的 Release 干净 PATH 部署复验通过，许可证随部署保存；无需 spdlog DLL。
- 依赖准备不再拉取 vcpkg 完整历史，而是稀疏获取同一固定 baseline；复用官方已安装工具，副本与辅助工具都在忽略提交的目录。
- 新版 vcpkg 的内部 SPDX 打包脚本需要 STRING_ENCODE，内部使用 VS 已有且具备此能力的 CMake；项目和 IDE 预设仍使用 3.30.5。未手工修改第三方源码，包含官方 vcpkg port 1 自带补丁。
- 独立只读审查未发现阻塞问题；目前没有直接注入磁盘满或 flush 失败的测试，不将目录错误测试等同于覆盖所有磁盘故障。

## 可复现命令
```powershell
./scripts/PrepareDependencies.ps1
./scripts/Build.ps1 -Configuration Debug
./scripts/Build.ps1 -Configuration Release
python scripts/VerifyLessons.py
./scripts/Deploy.ps1
```
前置：按 README 生成用户预设；PowerShell 7、Python 3.9+。原始证据在忽略提交的 build/verification，CTest 明细在各构建目录。

## 失败与回归
- VS 打开项目使用其内置 CMake 4.3.1-msvc1，与 3.30.5 基线冲突。已在本机预设指定 IDE 的 cmakeExecutable、编译器和运行库路径，在公共预设声明 external x64/v143 工具集；不升级或降级 VS。预设生成测试先失败后通过，现已加入 Build.ps1 默认验证流程，双配置原有 6 组 CTest 再次通过。原生 VS 界面重配和 F5 会话需用户实际确认。
- 未实现桩的图像、窗口契约先失败，再实现到通过。
- cvtColor 复用共享源缓冲区曾改写源图像，由回归测试发现并修复。
- 离屏中文缺字由字体测试发现，提供系统字体目录后通过。
- 官方 OpenCV 的 Windows 数学常量问题通过依赖构建选项处理，源码未改；必要依赖包括 geometry/flann。

## [自检] 本次变更
- 新增分层源码、显式构建、脚本、资源、测试、教材与学习记录；无既有业务接口或二进制兼容性破坏。
- 内存：自有代码无裸 new/delete；Qt 生成子控件由父对象管理；Mat/QImage 所有权有回归。
- 线程：GUI 仅主线程；日志队列受互斥保护；关闭前等待后台写入。
- 错误处理：脚本检查退出码；资源初始化异常在入口处理；日志错误可查询且失败退出。
- 独立只读审查无阻塞问题。未找到 wbliu-code-check，已按规范替代自检；未运行 qt-cpp-review。
- Qt offscreen 的 propagateSizeHints 提示不影响图像、字体和关闭断言。可选 Vulkan SDK 未安装，当前 Widgets/CPU 课程不需要它。
- 限制：未验证另一台全新 Windows、相机、真实工业精度或 GPU。没有推送远程。

## Doxygen 注释统一追加验收
- 按用户提供的 K03 规范逐项核对 src 下 6 个头文件、8 个实现文件，外部原件保持不变；项目约定保存于 docs/conventions/doxygen.md。
- 严格候选扫描 M1～M5 全零；人工核对参数、返回值、生命周期与实现。删除的匿名拷贝操作以 note 如实说明，不改签名。
- 相对日志迁移提交 5108632，全部 14 个源文件去除注释后的可执行 token 一致；枚举仅按逐项尾注需要拆行，没有接口或逻辑变化。
- 最终 Debug/Release 均重新构建，各 6/6 CTest 通过；6 个公开头文件独立包含编译通过。检查证据保存在 build/verification/doxygen-final.log 和 spdlog-final-*.log。
- 自检：修改仅涉及注释及对应规范记录，源文件无需新增构建条目。未找到 wbliu-code-check，以规范自检和独立只读审查替代；未运行 qt-cpp-review。
