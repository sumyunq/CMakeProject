# 检测与分割：解码、NMS、坐标映射：参考答案

r=.8，padY=160，映回[100,50,300,200]，宽200高150。mask并集=100+120-80=140，IoU=80/140≈.5714；Dice=160/220≈.7273。实现先减padding再除r，裁到原图边界后拒绝空框。空真值和空预测同时出现时明确约定忽略该类或记1，不能零除后把NaN参与均值。

## 为什么这样做

两个80×80框横向相差10，交集70×80=5600，并集6400+6400-5600=7200，IoU=7/9≈.7778。分数.9和.8，同类NMS阈值.5保留.9。不同类别通常分开NMS。输入640×640、原图800×400，letterbox比例r=.8，上下pad=160；网络框坐标(x,y)映回原图为(x/r,(y-160)/r)，不是分别除以宽高比例。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 23 分支（16/17共享序列实验）。运行 `advanced_labs 23 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

真实DNN流程沿用上课CPU基线，forward后先打印输出names与每个shape。对于明确契约[1,N,6]且每行为x1,y1,x2,y2,score,classId的模型，逐行过滤有限值和正面积，按类别组织Rect2d与scores，调用cv::dnn::NMSBoxes，再逆letterbox、裁边并保留浮点框供评价。对于语义分割[1,C,H,W] logits，在每个像素对C取argmax，再最近邻恢复标签图；不要双线性插值类别编号。实例mask还需要按模型规定的原型/系数组合及裁框，不可以把语义分割当实例分割。评价检测mAP需按IoU和置信度匹配真值，分割报逐类IoU与空类策略。

## 验收边界

输出画出两个重叠框并报告IoU；mask.png为面积400的二值区域。非重叠框交集0反例必过。baseline只有几何判定与mask面积，没有实际检测网络；要验证NMS排序/类别策略，应另实现候选列表循环并做多框测试，不能把两个框的比较当完整检测器。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <array>
#include <algorithm>
#include <optional>
#include <cmath>
std::optional<std::array<double, 4>> unletterbox(std::array<double, 4> box)
{
    for (const auto value : box) { if (!std::isfinite(value)) { return std::nullopt; } }
    box[0] = std::clamp(box[0] / .8, 0.0, 800.0);
    box[2] = std::clamp(box[2] / .8, 0.0, 800.0);
    box[1] = std::clamp((box[1] - 160) / .8, 0.0, 400.0);
    box[3] = std::clamp((box[3] - 160) / .8, 0.0, 400.0);
    if (box[2] <= box[0] || box[3] <= box[1]) { return std::nullopt; }
    return box;
}
```

unletterbox({80,200,240,320})=[100,50,300,200]；只落在padding区的框裁切后可能为空，须拒绝。
