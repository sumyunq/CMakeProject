# 亚像素定位与测量误差预算：参考答案

一阶传播σL²=(sσp)²+(pσs)²=(.0028)²+(.001)²，σL≈.002973mm。报告约2.000mm及约.003mm标准不确定度，注明还没有包含温度和高度偏差。拟合分母绝对值低于阈值或|δ|>.5时拒绝该峰，保存失败原因，不硬截偏移后声称成功。

## 为什么这样做

对峰值及相邻采样y(-1),y(0),y(1)拟合二次曲线，顶点偏移δ=.5·(y(-1)-y(1))/(y(-1)-2y(0)+y(1))。给[7.75,9.75,9.75]，分母-2，分子项-1，得δ=.5。平坦[1,1,1]分母0，不可定位。比例s=.02mm/px，定位σx=.1px时σL=sσx=.002mm；若两个独立端点都为.1px，长度σ为sqrt(2)×.002mm。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 19 分支（16/17共享序列实验）。运行 `advanced_labs 19 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

可用imgproc的Sobel得到CV_32F梯度，沿估计法线插值采样，再拟合峰；cornerSubPix用于角点局部细化，其输入角点、灰度图、搜索窗口和终止条件必须明确。量圆可先边缘点筛选，再用core solve拟合，分析残差与离群点，而非把minEnclosingCircle包围半径当拟合半径。用有计量证书的标尺覆盖工作区域和高度，建立像素到平面坐标变换。保存原图、点集、校准版本、温度、残差、重复性，不把拟合小数位数当真实精度。

## 验收边界

报告峰偏移.5和.002mm；画折线检查顶点附近形状。程序显式识别平坦分母。自己把中心加.2后重算偏移，讨论模型与噪声敏感性。真正边缘应先构造方向梯度响应，再对响应峰拟合；不能不分对象地对原始灰度所有像素做抛物线。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
std::optional<double> peakOffset(double left, double center, double right)
{
    if (!std::isfinite(left) || !std::isfinite(center) || !std::isfinite(right)) {
        return std::nullopt;
    }
    const double denominator = left - 2 * center + right;
    if (denominator >= -1e-9) { return std::nullopt; }
    const double offset = 0.5 * (left - right) / denominator;
    if (std::abs(offset) > 0.5) { return std::nullopt; }
    return offset;
}
double lengthSigma()
{
    return std::hypot(0.02 * 0.14, 100.0 * 0.00001);
}
```

peakOffset(7.75,9.75,9.75)=.5；平坦峰无值；lengthSigma≈.00297321375。
