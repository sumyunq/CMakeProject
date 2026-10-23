# 原生高级模块实验与真实环境接入

默认30课的合成实验不需要额外模块。想实际运行ORB、PnP、视频编码解码、LK和ONNX时，用这一独立环境；不覆盖基础安装。

## 准备与运行

从仓库根目录执行，首次获取固定版本源码需要网络：

```powershell
./scripts/PrepareDependencies.ps1 -Profile Full -Configuration Release
./scripts/BuildNativeLabs.ps1 -Configuration Release
```

Full源码仍为5.0.0固定提交，构建core/imgproc/imgcodecs/features/calib/stereo/objdetect/video/videoio/dnn及必要依赖。安装在`build/deps/full-install/Release`。辅助vcpkg仍只负责spdlog。若需要Debug，两个命令都明确使用Debug；不要交叉混用运行库。

MSVC扩展构建明确声明Windows/AMD64，并将ASM编译器置为NOTFOUND，触发上游MLAS的无汇编回退分支，DNN使用内置SGEMM。原因是固定版本的vendored MLAS包含GNU `.S` 内核，不能直接当MSVC MASM编译；默认环境还可能遗漏处理器名称，造成错误地启用但不链接内核。此选项不改上游源码，不影响基础安装；CPU数值验证必须继续通过。

第二个脚本生成极小ONNX，再构建`examples/native`并运行5组CTest。输出在`build/native-release/output`，模型在该构建目录的`relu.onnx`。日志和原始构建证据在`build/verification`，正常错误必须非零退出。

Full默认串行构建，避免DNN链接与其他编译同时争用内存；内存充足时可显式传`-Parallel 4`。2026-10-23已完成本机Release安装及5组原生实验；Debug扩展配置、真实相机、GPU及第三方检测模型没有在本批验收。

| 实验 | 实际使用的API | 固定输入和验收 |
|---|---|---|
| features | ORB、BFMatcher、NORM_HAMMING | 固定随机纹理与自身副本，至少50个匹配、距离0 |
| camera | projectPoints、solvePnP | 8个已知3D点投影再恢复，重投影RMS小于0.001像素 |
| video | VideoWriter、VideoCapture、内置MJPEG | 本地生成12帧160×120视频，解码恰好12帧，缺失文件拒绝 |
| flow | goodFeaturesToTrack、calcOpticalFlowPyrLK | 已知平移(3,2)，至少10点有效，残差小于0.1像素 |
| dnn | readNetFromONNX、setInput、forward | FLOAT32形状[1,3,2,2]，y=max(0,x)，12个输出误差小于1e-6；缺模型拒绝 |

单独重跑可从构建目录使用CTest，例如 `ctest --test-dir build/native-release -R native_dnn --output-on-failure`；使用锁定CMake同目录的ctest，或执行BuildNativeLabs.ps1。默认脚本负责运行时PATH。

## 极小模型是什么

`GenerateTinyOnnx.py`用Python标准库按ONNX protobuf字段编码一个ReLU节点，无外部权重和下载。输入与输出明确为NCHW的1×3×2×2，opset13、IR7。它验证真实加载和推理接口；不是训练得到的检测器，不能用它宣称目标检测或语义分割已部署。

本版本生成130字节，SHA256为`e0d45120f14d08ad45271ad037d8e68a70ac84e240206ec669009b2834aa6379`；模型本身在忽略提交的构建目录中生成，不需要另找下载链接。

换模型前保存：来源、许可证、SHA256、opset、输入名、布局、尺寸、颜色顺序、归一化、输出名与形状、类别表和坐标定义。先与导出框架对同一输入逐元素比较，再写后处理，最后测完整数据集；不要先在Qt窗口里猜框的位置。第22、23、30课提供契约与后处理练习。

## 相机接入

先让厂商SDK或系统工具能稳定采集，记录设备、驱动、像素格式、曝光、增益、触发方式和时间戳单位。用VideoCapture时显式尝试对应后端，检查isOpened与每次read；工业SDK将其缓冲区包装Mat时必须明确复制或持有期限。关闭自动曝光后采集尺度件和失败样例，重新计算第14、19课的独立误差。模拟投影的极小误差不代表真实镜头标定精度。

## 外部视频编解码

本基线关闭FFmpeg，原生实验使用OpenCV内置MJPEG，不假设H.264可用。若业务要读H.264，单独启用并核对FFmpeg来源和许可证，记录getBuildInformation及实际backend；构建和部署都需重验。读取失败不一定是视频路径错误，也可能是后端或编码格式不支持。时序处理中用帧携带的时间，不把处理循环速度当采集FPS。

## GPU接入

默认关闭CUDA/OpenCL。硬件到位后先记录设备、驱动、运行库与模块支持，再跑官方最小例；检查后端是否真的启用，保留CPU数值对照与容差。测量必须包含上传、执行、下载和同步；不能把异步提交时间当完成时间。第26课提供对照与部署验收，不承诺任意设备上的加速比。

参考依据：[固定DNN声明](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp)、[固定视频模块](https://github.com/opencv/opencv/tree/5.0.0/modules/videoio)、[ONNX规范](https://github.com/onnx/onnx/blob/main/onnx/onnx.proto)。
