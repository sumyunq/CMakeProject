# 工业成像：镜头、光源、曝光与平场：参考答案

固定曝光增益和相机，依次改变光源入射角、偏振片夹角、漫射覆盖，分别记录缺陷与背景均值差/背景噪声得到CNR；每组同时保存良品和缺陷样本。g=.02低于例如.1的有效门限，mask标为无效并拒绝用于测量；若仅展示可局部插值，但不能将插值像素算真实测量。

## 为什么这样做

线性未饱和成像可写I(x)=g(x)S(x)+b(x)。暗场估计b，均匀白板估计g，校正S=(I-b)/g。例S=100、g=.5、b=10，测得60，校正(60-10)/.5=100。g=.75时测得85仍恢复100；g接近0会放大噪声，必须有效mask。8位测量300会截到255，反算最多245/g，已经不可逆。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 18 分支（16/17共享序列实验）。运行 `advanced_labs 18 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

硬件准备清单为固定机械安装、稳定光源电流、触发控制、镜头光圈焦点锁紧、遮环境光和可靠长度标尺。先用imread读取无压缩原始图并检查depth/channels，imgproc做灰度及必要滤波，core的subtract/divide做float平场。工业相机常需厂商SDK获取Bayer/mono12/mono16与硬件时间戳；VideoCapture不保证支持每种工业相机。Bayer去马赛克用cvtColor且明确BG/GB/RG排列；12位数据放16位容器不等于已经乘满65535。按黑白参考建立归一化，禁止在转8位后再评估传感器饱和。

## 验收边界

实验构造从.5到接近1的照明增益，暗偏置10，恢复后与100的最大误差接近0；result.png保存渐变观测。加入暗场误差2，左侧g=.5位置物方误差为4，右侧约2，说明平场没有消灭标定误差。保存原始暗场、白场和时间温度条件，避免只保存校正图。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
std::optional<double> correctFlat(double measured, double dark, double gain)
{
    if (!std::isfinite(measured) || !std::isfinite(dark) || !std::isfinite(gain)
        || gain < 0.1 || measured >= 255.0 || measured < 0.0) { return std::nullopt; }
    return (measured - dark) / gain;
}
```

correctFlat(60,10,.5)为100；gain=.02拒绝；255按可能饱和拒绝。16位相机需用其实际饱和码值替换255。
