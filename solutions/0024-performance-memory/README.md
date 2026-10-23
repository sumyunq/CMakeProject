# 性能与内存：先量成本再优化：完整参考解答

## 解题思路与实现

参考实现：复用cv::Mat output，在循环外保留所有权；每次threshold会复用兼容尺寸的输出。手写阈值使用for(y)取src.ptr<uchar>(y)、dst.ptr<uchar>(y)，再遍历cols；大于100赋255，否则0。不要将clone移入计时区又声称只测算法。性能报告同时列明复制包含/排除。

## 数值推导

一张128×96单通道8位图占12288字节，100次读写至少搬运2457600字节；这只是像素数据，不含分配器和缓存。延迟T=分配+拷贝+计算+等待。吞吐量为完成数量/总时间，不能用1/平均延迟代替并行流水线吞吐量。ROI每行跨度仍为母图128字节，32×32 ROI不是连续数组；连续读取1024字节会跨入错误像素。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 24`，或 `engineering_labs 24 artifacts/lesson24`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

实验先运行一次threshold预热，再计100次已复用输出的处理；打印elapsed_ms、bytes。别名data相同而clone的data不同。24-input.png是三块12×20目标；24-mask.png中白像素应为720。不要对这个极小输入承诺加速比；把128×96扩大到1920×1080，再分别统计30轮的中位数和P95。P95按排序后ceil(0.95N)-1取索引。

## 独立扩展的实施顺序

独立实现两种批处理：每帧新建输出，与保留同尺寸Mat调用create；处理100张相同图，输出完全一致。增加非连续ROI，用逐行ptr实现阈值，禁止假定isContinuous为真。记录Release构建、尺寸、线程数、迭代数和原始耗时。

## 逐项验收

逐像素一致、非连续ROI正确、空图明确拒绝各20分；可复现Release原始测量和P95各20分。合格≥80且正确性项不得为0。不得以单次计时当性能结论。

## 自测详解

1. alias改变首像素会不会影响原图？会，只有clone独立。2. ROI的total()*elemSize()能否描述整段可连续复制区？不能，它是有效像素总量。3. 时间下降但输出误差变大是否合格？除非事先定义容差，否则不合格。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。

## 逐行阈值参考代码（非连续输入也正确）

```cpp
cv::Mat thresholdRows(const cv::Mat& input) {
    if (input.empty() || input.dims!=2 || input.type()!=CV_8UC1) { throw std::invalid_argument("input"); }
    cv::Mat output(input.size(), CV_8UC1);
    for (int y=0; y<input.rows; ++y) {
        const auto* source=input.ptr<uchar>(y);
        auto* target=output.ptr<uchar>(y);
        for(int x=0; x<input.cols; ++x) { target[x]=source[x]>100 ? 255 : 0; }
    }
    return output;
}
```
固定验证：完整图白像素720；取Rect(1,1,32,32)后对照cv::threshold逐像素误差0；空Mat抛invalid_argument。计时先预热，每轮只测函数调用，保存30个独立耗时，不把PNG编码纳入算法时间。
