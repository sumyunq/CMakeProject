# GPU成本模型与Windows部署：完整参考解答

## 解题思路与实现

参考答卷：先使用windeployqt处理Qt插件，再依据dumpbin /DEPENDENTS递归收集OpenCV与运行时；绝不混用Debug和Release。创建干净部署目录，清除开发PATH后启动；缺qwindows时输出诊断，缺图像编解码时检查插件/构建。可选GPU验收分为能力探测、上传、多核驻留、下载同步、精度对照五步；任何一步失败走CPU并在报告写backend。当前默认core/imgproc/imgcodecs构建没有CUDA目标，因此不能把头文件存在视为GPU可用。

## 数值推导

Tgpu=Tupload+Tkernel+Tdownload+Tsync；Tcpu=直接计算。1080p三通道单帧6220800字节，若有效传输为6GB/s，单向理论下限约1.04ms，往返2.08ms，还没计调用和同步。让多步算法驻留设备可摊薄传输，但不能减少最终显示同步。速度与可部署性是两个独立验收项。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 26`，或 `engineering_labs 26 artifacts/lesson26`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

26仅用clone模拟上传所有权，再threshold生成26-result.png；26-host.png保持不变，白像素720。输出明确标记CPU-surrogate和GPU_VALIDATED=false；不存在的输入路径必须返回空而被检查。这个实验不调用CUDA/OpenCL，不证明设备可用、驱动兼容或GPU精度。

## 独立扩展的实施顺序

编写部署清单：Release程序、QtCore/Gui/Widgets、platforms/qwindows.dll、OpenCV实际导入DLL和MSVC运行时。在无开发环境Windows虚拟机运行环境探针和26实验；用依赖检查器确认x64且没有Debug DLL。若要GPU扩展，另建可选目标并记录驱动、设备、OpenCV构建选项与CPU回退。

## 逐项验收

CPU输出正确20分、明确标记未验证GPU20分、部署依赖及位数20分、干净机记录20分、缺依赖和缺文件失败可见20分。GPU加速只作为额外实验，不计入离线必修通过条件。

## 自测详解

1. UMat是否保证GPU执行？不保证，后端可能CPU回退。2. 只测异步核提交耗时准确吗？不准确，需要等待设备完成。3. 开发机启动成功能否证明部署完成？不能，需要清空开发环境的干净机器。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。
