# 会话上下文

## 目标与已确认决策
- 面向能独立开发 Qt Widgets 应用的学习者，完整、分步、纵深学习 OpenCV，覆盖图像、工业视觉、视频、AI、性能与源码。
- Qt 6.8.3 msvc2022_64；OpenCV 5.0.0 官方固定源码；C++20；MSVC v143 14.44.35207 x64；CMake 3.30.5；Ninja 1.12.1。
- 完整路线、逐课交付；演示加独立练习；首批环境验证与 Mat 入门。不得把代理演示/测试通过记为学习者掌握。
- QSS 独立文件、C++ 加载；手动集中 connect；业务模型是状态来源。
- 原《AIAgent使用习惯记录.mm》只读，未修改。习惯已整理到 AGENTS.md。
- 首批验收后本地中文提交，不推送；保留原 Git 历史。工作分支 feat/opencv-learning，基线 57eacfc。

## 当前交付状态
1. 规则、学习目标、官方资料、完整课程地图及两课 HTML 已完成。
2. 两种 OpenCV 配置已从固定源码构建安装；公共/本机预设分离并验证。
3. 图像属性、独立 QImage 转换、环境诊断、Mat 实验、最小 Widgets 验证窗口已完成。
4. Debug/Release 各 6 组 CTest 通过；干净 PATH 的 Release 部署通过；独立审查无阻塞发现。最后进行文档/注释收尾、最终复验及本地提交；提交完成情况以 git log/status 核对。

## 重要实现裁定与错误修复
- 使用现有工作目录的独立分支，不额外创建用户难以找到的目录。
- 5.0.0 是 annotated tag：标签对象 9e2ede9628a55ec2742a1b180d3e69b0322281b9，实际提交 40738fb16ceddb5fb3fea747585f7ce6abb0605b。校验实际 HEAD 和版本头。
- 官方源码采用 blob:none + sparse checkout。imgproc 必要依赖 geometry → flann → core；还需 doc/docs_sphinx 根 CMake 入口，即使 BUILD_DOCS=OFF。上游源码保持干净。
- MSVC 编译发现上游 geometry/shapedescr.cpp 的 M_PI 缺定义，通过 OPENCV_EXTRA_CXX_FLAGS=/D_USE_MATH_DEFINES 修复构建，不修改官方代码。
- 颜色转换不能先用 formatted=image 作为 cvtColor 目标：会原地改写源。回归曾检测 norm=82，修复为空目标后变为 0，灰度路径仅用只读视图并 copy。
- CMake 公共预设不能引用用户文件的配置；采用公共隐藏基类，实际 configure/build/test 预设放在忽略提交的用户文件。
- 日志基础设施不递归追踪自身；启动前/停止后的追踪也不可能入队。应用启动后、停止前的作用域覆盖提前返回，Release 编译消除。
- Qt offscreen 默认缺中文字体；只为窗口测试设置系统 Fonts 路径并验证中文字形。QSS 指定雅黑，已检查中文正常的截图。
- 部署的交互 GUI 使用正常窗口检查 MainWindowHandle 并正常关闭；后台辅助服务仍使用隐藏窗口。
- 最终部署验证通过后发现脚本未恢复调用进程 PATH，导致同一 PowerShell 后续找不到 git。本批补 try/finally 恢复 PATH 与 Qt 环境变量，并用部署前后 PATH 相等作回归验证。
- PATH 恢复回归已通过，后续 git 命令也正常。收尾将清理逻辑覆盖整个部署过程（含前置失败），保存最终验证状态并创建本地提交。

## 验证证据
- build/verification/logger-red.txt：日志 3 个行为先失败；logger-green.txt：实现后通过。
- learning-red.txt/window-red.txt：未实现时图像 13 个、窗口 2 个预期失败；源像素不变与字体回归也观察过失败。
- 最终双配置结果：各 6 组 CTest 全部通过；图像 QtTest 24、日志 5、窗口 5 条通过记录（均含初始化/清理）。6 个公开头文件独立编译。
- 浏览器检查了入口导航、第一课答案展开和第二课页面；VerifyLessons.py 检查全部本地链接、共享样式与 HTML 结构。
- Release 部署到忽略提交的 dist 时间戳目录；PATH 只保留系统目录，诊断报告实际 Qt 6.8.3/OpenCV 5.0.0/C++20/x64，Mat 输出匹配教材，GUI 正常启动关闭。
- 独立只读代码审查未发现阻塞问题；已补 Python 前置说明、灰度/BGRA 非连续 ROI 测试。
- 未找到 wbliu-code-check 工作流，按 AGENTS.md 自检并独立审查；没有运行 qt-cpp-review/qt-cpp-view。

## 后续规则与下一步
- 2026-10-20：用户使用 Visual Studio 打开出现 CMake 精确版本错误。实测 VS 内置 4.3.1-msvc1，原本机预设未指定 IDE 的 cmakeExecutable，也未声明 external x64/工具集环境；VS 确实把内置 CMake 写入了缓存。保持既定 3.30.5 基线，补齐 IDE 预设，不删除用户 .vs 设置。先写预设生成回归测试，再修复并复验。
- 预设回归已观察先失败后通过；Debug 6/6 测试通过。Release 旧缓存因编译器路径斜杠/大小写差异触发自动重置，统一 CMAKE_CXX_COMPILER 为正斜杠规范路径，重新配置验证；用户在 VS 执行删除缓存并重新配置可清除旧状态。
- VS 配置修复结果：已重新生成本机预设，显式选择 CMake 3.30.5、Ninja、x64/v143 14.44.35207 和对应 DLL 路径；Debug/Release 均重新构建且各 6/6 CTest 通过，预设生成回归也通过。未操纵 VS 原生界面，用户需在 VS 重新配置，不能声称已实际验证 VS 的 F5 会话。
- 每次修改批次前更新本文件；上下文压缩后完整重读，再看 Git 状态、最近提交及验证报告，避免重复施工。
- 用户的学习状态保持“未开始”。从 lessons/0001-environment.html 开始；收到用户自己的输出、练习和解释后再更新学习状态。
- 后续新课逐步扩展模块，重新记录构建选项与验证；不自动升级固定版本。
- 未覆盖另一台干净 Windows 安装验收、相机、真实工业精度、AI 模型、GPU；不宣称这些已验证。
维护时间：2026-10-20 16:01:47 +08:00 wbliu。
