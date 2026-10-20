# OpenCV 视觉学习工程

面向已有 Qt Widgets 开发能力的学习者。完整路线、逐课深入：原理、实验、失败案例、独立练习与复盘。

**学习入口：[本地课程](lessons/index.html)** · [课程地图](docs/curriculum.md) · [进度](learning-records/progress.md) · [官方资料](RESOURCES.md)

## 固定基线

Qt 6.8.3 msvc2022_64 / OpenCV 5.0.0 / C++20 / MSVC v143 14.44.35207 x64 / CMake 3.30.5 / Ninja 1.12.1。

OpenCV 固定提交 `40738fb16ceddb5fb3fea747585f7ce6abb0605b`，对应正式标签 5.0.0。首批只构建 core、imgproc、imgcodecs；Qt 自己显示图像，不启用 OpenCV HighGUI/Qt 后端。

## 首次准备

使用 PowerShell 7，从仓库根目录执行。默认测试还需要 Python 3.9 或更高版本（仅标准库，用于检查课程链接，本机已具备），不需要 Python OpenCV。首次源码获取需要网络，完成后课程和演示不需要网络、摄像头或模型。

运行 `scripts/InitializeLocal.ps1`，提供三个参数：
- `-QtRoot`：包含 bin/qmake.exe 的 Qt 6.8.3 msvc2022_64 前缀。
- `-VisualStudioRoot`：包含 Common7/Tools 的 VS 安装目录，须已安装 v143 14.44.35207。
- `-ToolsRoot`：包含 CMake_64 与 Ninja 的 Qt Tools 目录。

脚本生成忽略提交的 CMakeUserPresets.json。当前机器已配置好时无需重复初始化。公共预设不含个人安装路径，不修改全局 PATH。

```powershell
./scripts/PrepareDependencies.ps1
./scripts/Build.ps1 -Configuration Debug
./scripts/Build.ps1 -Configuration Release
```

准备依赖时会验证源码提交、干净状态和版本头。官方源码按首批模块稀疏检出；imgproc 必要依赖 geometry、flann 也会构建。MSVC 下通过上游构建选项启用 _USE_MATH_DEFINES，解决 5.0.0 geometry 的 M_PI 编译问题，未修改第三方源码。后续扩展模块需要显式增加检出目录与 BUILD_LIST，并重新验收。日志在 build/verification；每个外部命令检查退出码，失败停止。

## 运行

```powershell
./scripts/Run.ps1 -Configuration Debug -Program environment_probe
./scripts/Run.ps1 -Configuration Debug -Program mat_demo
./scripts/Run.ps1 -Configuration Debug -Program vision_viewer
```

诊断程序显示实际版本与基础运算；Mat 演示展示共享与独立复制；Widgets 窗口显示已知红绿蓝色块和图像属性。三者都从生成图像开始，不读未知大文件。

## 验证与部署

Build.ps1 默认运行 CTest（可用 -SkipTests 仅构建，但不能据此声称验收）。测试覆盖像素转换与所有权、非连续 ROI、无效输入、并发日志、错误目录、保留期和窗口显示。各公开头文件有独立编译检查。

```powershell
./scripts/Deploy.ps1
```

先完成 Release 构建。部署脚本创建独立时间戳目录，准备 Qt/OpenCV 运行库，然后以去掉开发路径的 PATH 运行诊断和 Mat 演示，启动并正常关闭窗口。它不替代另一台干净 Windows 机器的安装验收。

日志在各可执行文件旁 logs，异步输出、正常退出排空，保留 7 天；Debug 记录应用函数进入/退出，Release 不生成函数追踪。文件不可写时向标准错误报告；应用启动失败时明确显示错误。Release 日志不记录用户数据或完整业务路径。

## 独立练习

见 exercises 下的两课任务。练习不自动加入默认构建，也不预先附完整答案；完成后提交源码、预测、实际结果和失败案例复盘。示例测试通过不会把你的学习状态改为“已验收”。

目录遵守 src 分层、tests、resources、docs、cmake、scripts、3rdparty 约定。依赖源码、构建目录、本机预设、部署副本及日志都不进入提交。当前实施记录见 docs/seession_context.md。
