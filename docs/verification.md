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
