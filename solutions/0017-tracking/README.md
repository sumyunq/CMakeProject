# 光流、目标关联与 Kalman 跟踪：参考答案

创新20、S=5、NIS=400/5=80，大于9，拒绝更新并保留预测位置100，记录离群计数。缺失帧只执行predict，累计missCount；达到应用定义的最大消失时长后结束轨迹。使用时间阈值比仅用帧数更适合变帧率。

## 为什么这样做

恒速状态[x,v]，F=[[1,dt],[0,1]]，预测x=Fx，P=FPFᵀ+Q。观测位置z，H=[1,0]，K=PHᵀ/(HPHᵀ+R)，修正x=x+K(z-Hx)。简化一维位置先验100、方差4、观测102、方差1，K=4/5=.8，后验101.6、方差.8。R大意味着更不信任测量；Q大意味着更允许运动偏离。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 17 分支（16/17共享序列实验）。运行 `advanced_labs 17 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

OpenCV5 video模块的opencv2/video/tracking.hpp提供calcOpticalFlowPyrLK与KalmanFilter。PyrLK输入前后同尺寸8位灰度、prevPts，输出nextPts/status/err；先筛status再检查误差与图像边界，反向跟踪并计算往返距离。KalmanFilter(4,2,0,CV_32F)设状态[x,y,vx,vy]，每次dt更新transitionMatrix位置速度项，设置measurementMatrix、processNoiseCov、measurementNoiseCov、errorCovPost及statePost。predict后只有有效门控观测才correct。关联用运动门控、外观和分配策略，连续漏检超阈值删除轨迹，新检测建立新ID；界面线程不直接访问可变滤波器。

## 验收边界

合成圆心速度500px/s，矩计算先检查m00>0才除；空白帧代表漏检，只预测而不假装观测为0。报告K=.8、修正101.6。独立比较给一帧检测加20像素跳变时无门控的偏移和有门控的拒绝。当前程序演示质心和一维更新，不调用真实光流。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
// 无测量/门控拒绝返回无更新；调用者保留predict的状态和协方差。
std::optional<double> correctGated(double prior, double variance,
                                  double observation, double noise)
{
    if (!std::isfinite(prior) || !std::isfinite(variance) || !std::isfinite(observation)
        || !std::isfinite(noise) || variance < 0 || noise <= 0) { return std::nullopt; }
    const double innovation = observation - prior;
    const double total = variance + noise;
    if (innovation * innovation / total > 9.0) { return std::nullopt; }
    return prior + variance / total * innovation;
}
```

correctGated(100,4,120,1)拒绝；观测102时返回101.6。实际滤波器还须更新协方差，遗漏这步会改变下一次增益。
