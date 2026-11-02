# 干净 Windows 复现

以下步骤需要 Windows x64、PowerShell 7、Git、Python 3，以及固定版本的工具。路径由使用者选择；不修改系统 PATH。安装 Visual Studio 2022 的 C++ 桌面开发组件及 **MSVC v143 14.44.35207 x64**（在安装器的单个组件中选择对应版本），确认 `VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe` 存在。安装 CMake 3.30.5、Ninja 1.12.1，按 InitializeLocal 要求分别放在工具目录的 `CMake_64/bin` 和 `Ninja`。

Qt 6.8.3 必须是 msvc2022_64 桌面构建，包含 Qt Test 和 windeployqt。通过 Qt 官方安装器获取；旧版可用性、账号与许可取决于 Qt 的发行政策，无法获取时停止并记录，不替换版本。安装器中的许可证文件也必须保留供部署使用。

在仓库根目录运行（将三个路径替换为实际安装位置）：

```powershell
./scripts/InitializeLocal.ps1 -QtRoot <Qt目录> -VisualStudioRoot <VS目录> -ToolsRoot <工具目录>
./scripts/PrepareLoggerDependencies.ps1
./scripts/PrepareDependencies.ps1 -Profile Foundation -Configuration Both
./scripts/PrepareDependencies.ps1 -Profile Full -Configuration Both
./scripts/Preflight.ps1 -Profile Foundation
./scripts/Preflight.ps1 -Profile Full
./scripts/Build.ps1 -Configuration Debug
./scripts/Build.ps1 -Configuration Release
./scripts/CheckReferenceAnswers.ps1 -Configuration Debug
./scripts/CheckReferenceAnswers.ps1 -Configuration Release
./scripts/BuildNativeLabs.ps1 -Configuration Debug
./scripts/BuildNativeLabs.ps1 -Configuration Release
./scripts/BuildWorkbenchReference.ps1 -Configuration Debug
./scripts/BuildWorkbenchReference.ps1 -Configuration Release
./scripts/Deploy.ps1
```

真实模型实验在首次下载完成后可离线执行；另运行：

```powershell
./scripts/PrepareRealModel.ps1
./scripts/BuildNativeLabs.ps1 -Configuration Debug -WithModel
./scripts/BuildNativeLabs.ps1 -Configuration Release -WithModel
```

预检只读取配置并检查版本及依赖记录；Full 与 Foundation 安装目录独立。构建和依赖准备会写入仓库的 build 目录。部署脚本在干净 PATH 下验证非 GUI 程序并检查窗口；交互式窗口需要桌面会话。第二台干净 Windows 的部署验证尚未完成。

CI 托管检查仅检查资料，不代表编译通过。完整自托管构建必须手动在 feat/opencv-learning 分支触发，且专用 runner 已配置固定工具链、`cvlab-locked` 标签和交互式桌面。须在 runner 实际的仓库工作目录执行上述初始化和两种配置的依赖准备，确保忽略提交的 CMakeUserPresets.json 和 build/deps 已存在；它们不会由 checkout 自动提供。此工作流关闭 checkout 自动清理以保留这些目录，切勿把该 runner 用于不可信仓库或分支。未配置 runner 时完整 CI 尚未运行。
