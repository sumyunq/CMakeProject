# 视频输入输出与时间戳：参考答案

速度(140-110)/(.20-.14)=500px/s。实时显示队列满时丢最旧未显示帧并累计drop计数，采集序号不重排。离线存档则采用背压或明确报吞吐不足。Writer关闭后用Capture读回全部帧，逐帧检查empty/尺寸，核对期望数量并用容差比对有损画面；不能要求有损视频像素完全相等。

## 为什么这样做

四帧采集时间为0、.04、.10、.14秒，目标x为40、60、90、110。相邻速度分别20/.04、30/.06、20/.04，全部500px/s。如果统一假定25fps，中间那段会算750px/s。时间差≤0必须拒绝；不可把系统时钟调回导致的负dt送入运动模型。本地时长使用steady_clock，设备硬件时间戳另保留其时基与转换关系。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 16 分支（16/17共享序列实验）。运行 `advanced_labs 16 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

完整依赖需videoio与video，Windows摄像头先确认系统隐私权限、驱动、设备索引和分辨率。include opencv2/videoio.hpp；cv::VideoCapture cap; if(!cap.open(0,cv::CAP_MSMF))报告失败；if(!cap.read(frame)||frame.empty())结束或按策略重试；getBackendName记录后端。CAP_PROP_POS_MSEC在文件中可作媒体时间线参考，但后端不保证所有属性有效。VideoWriter需open(path,fourcc,fps,Size,isColor)，检查isOpened，每帧尺寸类型一致；结束release，再重新VideoCapture读回检查帧数、尺寸和时间。FFmpeg/GStreamer或系统后端可用性取决于编译配置，fourcc成功不代表磁盘文件最终可解码。

## 验收边界

程序生成frame0.png到frame3.png，逐张写入再读回比较像素，报告真实时间差速度500。result.png只是摘要画布；本实验验证图像序列和时序运算，不验证任何视频编解码器、容器或摄像头。手动删除一帧后重跑读取逻辑应显式报缺失，不应复制上一帧伪造观测。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
std::optional<double> velocity(double x0, double t0, double x1, double t1)
{
    if (!std::isfinite(x0) || !std::isfinite(x1) || !std::isfinite(t0)
        || !std::isfinite(t1) || t1 <= t0) { return std::nullopt; }
    return (x1 - x0) / (t1 - t0);
}
```

velocity(110,.14,140,.20)为500；重复时间戳无值。容量为2的队列push前若满则pop_front，并增加丢帧计数，不能修改硬件序号。
