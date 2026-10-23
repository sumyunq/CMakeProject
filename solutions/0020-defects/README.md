# 缺陷检测与误报漏报的代价：参考答案

precision=.75，recall=.9，F1=2TP/(2TP+FP+FN)=36/44≈.8182。在验证集遍历阈值，计算总成本10FN+FP，并同时满足召回下限与人工复核产能；最终阈值锁定后在独立测试批次评价。不要根据测试结果反复调阈值，需另取验证批次。

## 为什么这样做

灰度模板100，10×10区域变160，绝对差60，阈值20后正像素100。阈值提高到70会完全漏掉。真实缺陷12个、系统报10个，其中正确8个，则TP=8、FP=2、FN=4，precision=8/10=.8，recall=8/12≈.667。全是良品的数据集即使全判正常也可能99%准确，不能因此认为检出能力强。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 20 分支（16/17共享序列实验）。运行 `advanced_labs 20 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

实际基础链采用absdiff、threshold、morphologyEx、connectedComponentsWithStats。stats包含面积、包围框，centroids是几何质心；边缘ROI要与工艺定义对齐。配准可以用已知定位标记或特征几何，变换失败和低置信度应进入人工复核。评价数据按批次、相机、日期、缺陷类型分层，阈值只在验证集选择。保留测试集锁定一次评价，同时记录每产品误报率、每缺陷召回率与吞吐延迟。掩膜像素IoU与产品合格判定是不同层次指标。

## 验收边界

程序输出面积100的二值result.png，模板自身差分为0作为良品反例，metrics给出precision和recall。再给整体灰度加10与加30分别比较阈值20下的误报，观察它不是局部缺陷。不要把这个单个合成方块的通过率当真实生产准确率。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <optional>
#include <array>
std::optional<std::array<double, 3>> classificationMetrics(int tp, int fp, int fn)
{
    const double predictedPositive = static_cast<double>(tp) + fp;
    const double actualPositive = static_cast<double>(tp) + fn;
    if (tp < 0 || fp < 0 || fn < 0 || predictedPositive == 0 || actualPositive == 0) {
        return std::nullopt;
    }
    const double precision = static_cast<double>(tp) / predictedPositive;
    const double recall = static_cast<double>(tp) / actualPositive;
    const double f1 = 2.0 * tp / (2.0 * tp + fp + fn);
    return std::array<double, 3>{precision, recall, f1};
}
```

classificationMetrics(18,6,2)返回[.75,.9,.81818]；分母为0需约定策略并显式处理，不隐式制造NaN。大量数据实际应避免计数int溢出。
