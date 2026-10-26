# OpenCV 视觉学习工程

面向已有 Qt Widgets 开发能力的学习者。完整路线、逐课深入：原理、实验、失败案例、独立练习与复盘。

**30课完整自学内容已提供**，不等待逐课解锁。先看[自主学习与验收指南](docs/self-study-guide.md)；每课都有独立练习及单独存放的参考解答，学习状态由自己的证据记录。

**学习入口：[本地课程](lessons/index.html)** · [课程地图](docs/curriculum.md) · [进度](learning-records/progress.md) · [官方资料](RESOURCES.md)

配套阅读：[Qt 与 OpenCV 英文词汇学习手册](docs/qt-opencv-vocabulary.md)，包含分主题词表、API 拆词、报错句式和独立自测。

易混术语另见[关键术语辨析](docs/terminology/critical-terms.md)：按概念分义，附词性、双语例句、来源与校核状态；[CSV术语库](docs/terminology/critical-terms.csv)用于维护和字段映射。

还有[数学速查](docs/math-reference.md)、[算法纵深研读](docs/algorithm-deep-dive.md)、[排错指南](docs/troubleshooting.md)、[4/5迁移](docs/opencv5-migration.md)和[原生高级API实验](docs/native-labs.md)。课程03～30使用生成数据，可离线完成：

```powershell
./scripts/Build.ps1 -Configuration Debug
./scripts/RunLab.ps1 -Lesson 03
./scripts/RunLab.ps1 -Lesson 30
```

默认实验输出到build/lab-output。完整模块使用独立full-install安装，准备方法见原生实验页；教材明确区分离线数值实验、原生API验证和真实硬件成果。综合项目的独立实现仍由学习者完成，参考程序不等同于生产系统。

## 固定基线

Qt 6.8.3 msvc2022_64 / OpenCV 5.0.0 / C++20 / MSVC v143 14.44.35207 x64 / CMake 3.30.5 / Ninja 1.12.1。

日志使用 spdlog 1.17.0（vcpkg port 1），由 vcpkg.json 固定版本与注册表基线。库以静态链接、动态 CRT 构建，匹配现有 Qt/OpenCV 工具集；不额外部署 spdlog DLL。

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

准备依赖时会验证源码提交、干净状态和版本头。默认Foundation按首批模块稀疏检出；imgproc必要依赖geometry、flann也会构建。`-Profile Full`使用独立目录增加特征、标定、双目、视频和DNN，见原生实验说明。MSVC通过上游构建选项启用_USE_MATH_DEFINES，完整构建另用项目钩子处理MLAS无汇编回退，不修改第三方源码。日志在build/verification；每个外部命令检查退出码，失败停止。

首次准备也会调用本机 Visual Studio 的 vcpkg 安装日志库。已有 OpenCV 时可单独运行 `./scripts/PrepareLoggerDependencies.ps1`；安装、下载与编译产物都位于忽略提交的 build 目录，需 Visual Studio 已安装 vcpkg 组件。

该脚本在 3rdparty 中稀疏获取同一固定 vcpkg baseline，复用已安装的官方工具并校验副本；避免下载完整历史。新版 vcpkg 打包脚本需要较新的 CMake 功能，会优先使用 VS 内置且支持该功能的辅助 CMake，否则使用 vcpkg 官方管理的辅助工具；这些仅用于依赖打包，工程及 IDE 的 CMake 3.30.5 预设不变。脚本退出时恢复调用环境。

## 运行

### 用 Visual Studio 打开

打开包含本 README、CMakeLists.txt 的仓库根目录。配置选择 `local-debug`（显示为 Qt 6.8.3 / OpenCV 5.0.0 / x64 Debug）或 `local-release`。

本机预设显式指定已验证的 CMake 3.30.5、Ninja 和 MSVC 14.44.35207；IDE 的 external 架构/工具集元数据负责选择 x64/v143 环境。不要直接使用 VS 自动生成的默认配置或内置其他版本 CMake。

如果此前已出现版本错误，预设更新后在 VS 执行一次“删除缓存并重新配置”。配置成功后把启动项选为 `vision_viewer.exe`（窗口）、`environment_probe.exe`（诊断）或 `mat_demo.exe`（矩阵实验）。本机预设还提供相应配置的 Qt/OpenCV DLL 搜索路径，不需要修改全局 PATH。

若换机器，重新运行 InitializeLocal.ps1 生成本机预设。`.vs` 和 `out` 是 IDE 本地状态，不进入提交。

### 命令行运行

```powershell
./scripts/Run.ps1 -Configuration Debug -Program environment_probe
./scripts/Run.ps1 -Configuration Debug -Program mat_demo
./scripts/Run.ps1 -Configuration Debug -Program vision_viewer
```

诊断程序显示实际版本与基础运算；Mat 演示展示共享与独立复制；Widgets 窗口显示已知红绿蓝色块和图像属性。三者都从生成图像开始，不读未知大文件。

## 验证与部署

Build.ps1默认运行37项CTest（可用-SkipTests仅构建，但不能据此声称验收），含28项课程数值实验、3项无效课号和原6项基础验证。各公开头文件有独立编译检查。答案文档的代码另用CheckReferenceAnswers.ps1验证，不加入默认构建。

```powershell
./scripts/Deploy.ps1
```

先完成Release构建。部署脚本创建独立时间戳目录，准备Qt/OpenCV运行库，然后以去掉开发路径的PATH运行诊断、Mat和28项课程实验，启动并正常关闭窗口。它不替代另一台干净Windows机器的安装验收。

日志在各可执行文件旁 logs，由 spdlog 专属线程池异步输出；8192 容量、单消费者、满队列阻塞不丢弃。正常退出先请求 flush，再等待线程池排空并复核落盘；保留 7 天。Debug 记录应用函数进入/退出，Release 不生成函数追踪。日志级别使用 spdlog 标准名称。文件不可写时向标准错误报告；应用启动失败时明确显示错误。Release 调用方只写脱敏事件，不记录用户数据或完整业务路径。

## 独立练习

exercises下提供30课任务，solutions下单独提供对应参考答案。先独立完成，再对答案并按自学指南记录预测、源码、结果与失败案例；不需要回到会话才能继续。练习与答案不自动加入默认演示构建。示例测试通过不会把你的学习状态改为“已验收”。

目录遵守 src 分层、tests、resources、docs、cmake、scripts、3rdparty 约定。依赖源码、构建目录、本机预设、部署副本及日志都不进入提交。当前实施记录见 docs/seession_context.md。
