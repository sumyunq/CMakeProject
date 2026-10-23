# 位姿与双目三维：坐标系、视差和误差：参考答案

d=800×60/2400=20px，σZ≈48000×.5/400=60mm。输出先保留毫米，显示米时再除1000。实现拒绝d≤ε并使用double，将有效性mask与深度同时保存；不能用0深度冒充真实靠近相机的点。

## 为什么这样做

校正双目有uL=fx·X/Z+cx，uR=fx·(X-B)/Z+cx，两式相减得到d=fx·B/Z，所以Z=fx·B/d。取fx=800px、B=60mm、d=40px，则Z=1200mm；d增为40.5时Z≈1185.185mm。微分得到σZ≈fxBσd/d²，σd=.5时σZ约15mm；距离翻倍时误差约四倍。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 15 分支（16/17共享序列实验）。运行 `advanced_labs 15 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

真实OpenCV5的标定模块为calib，几何模块为geometry，立体匹配模块为stereo。opencv2/geometry.hpp提供solvePnP/projectPoints，opencv2/calib.hpp提供stereoCalibrate，opencv2/stereo.hpp提供stereoRectify，安装后以本地头文件声明为准；不要盲目沿用4.x的calib3d链接名。solvePnP输入同一坐标系物点、像素点、K、dist，检查bool并通过projectPoints验证。stereoRectify得到R1/R2/P1/P2/Q，生成映射remap后检查纵向残差；StereoSGBM的固定点视差要按其输出约定除以16再参与深度计算，缺失值先屏蔽。硬件须同步触发、固定安装、记录基线单位，移动目标不能用先后两张照片假装同时。

## 验收边界

metrics记录1200和约1185.185，图像显示两条投射线。d=0必须判为不可测或无穷远，负视差要检查左右顺序和有效区，不能用绝对值掩盖错误。尝试d=20、10，先手算再执行公式。这里没有实际立体匹配，不验证纹理、左右一致性与遮挡。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
std::optional<double> depthSigma(double disparity, double sigmaDisparity)
{
    if (!std::isfinite(disparity) || !std::isfinite(sigmaDisparity)
        || disparity <= 1e-9 || sigmaDisparity < 0) { return std::nullopt; }
    constexpr double focal = 800.0, baselineMm = 60.0;
    return focal * baselineMm * sigmaDisparity / (disparity * disparity);
}
```

depthSigma(20,.5)为60；depthSigma(0,.5)无值。深度与标准差应使用同一长度单位。
