# 相机标定：从针孔到可验证参数：参考答案

f=400、cx=160；像素内参随横向缩放变化，归一化畸变系数不因单纯像素缩放改变。若同时缩放y则fy、cy也同比变化。实现将u矩阵乘.5后重新cv::solve并检查两参数；图像尺寸也记录为原来一半。独立保留新K，不能覆盖原始分辨率的K。

## 为什么这样做

已知四个归一化横坐标[-.2,-.1,.1,.2]，对应像素[160,240,400,480]。建立A每行为[x,1]，求A·[f,cx]ᵀ=u。两端差320像素除以.4得到f=800，再代入160=800×(-.2)+cx，得cx=320。SVD残差接近0。这是已知深度和姿态的线性内参子问题；单张未知姿态平面不能据此恢复所有内外参与畸变。完整标定通过多视角约束与非线性优化求解。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 14 分支（16/17共享序列实验）。运行 `advanced_labs 14 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

真实采集固定焦距、关闭自动对焦，打印或购买尺寸可靠的平面棋盘格，量相邻内角点间距，记录图像尺寸和曝光。采集至少十余种倾角、距离、边缘覆盖，不要连拍相似姿态凑数量。OpenCV5使用opencv2/objdetect.hpp的findChessboardCorners查角点，opencv2/calib.hpp负责参数标定，imgproc的cornerSubPix细化；calibrateCamera接收每视图vector<Point3f>与vector<Point2f>、Size、K、dist并返回RMS；逐视图projectPoints重投影再算误差。输出K、dist、图像尺寸、板格毫米尺寸与每视图误差。去畸变需验证模型和有效区域，裁剪后更新主点。

## 验收边界

输出metrics.txt应包含f=800、cx=320和接近零的残差；result.png画四个观测位置。将所有x设为1时A两列相关，第二奇异值接近0：即使solve给出最小范数解，也必须按秩不足拒绝。再在边缘像素加入2像素偏差，比较平均残差与最大残差，思考为什么均方误差会掩盖局部失真。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <array>
#include <cmath>
#include <stdexcept>
// 已知归一化横坐标的两参数最小二乘，返回[f,cx]。
std::array<double, 2> fitHalfResolution()
{
    constexpr std::array<double, 4> x{-0.2, -0.1, 0.1, 0.2};
    constexpr std::array<double, 4> u{80, 120, 200, 240};
    double xx = 0, xu = 0, sx = 0, su = 0;
    for (std::size_t i = 0; i < x.size(); ++i) {
        xx += x[i] * x[i]; xu += x[i] * u[i]; sx += x[i]; su += u[i];
    }
    const double determinant = 4 * xx - sx * sx;
    if (std::abs(determinant) < 1e-12) { throw std::runtime_error("rank deficient"); }
    return {(4 * xu - sx * su) / determinant, (xx * su - sx * xu) / determinant};
}
```

结果为[400,160]。这是二维正规方程的教学展开；实际病态问题使用SVD而不是直接正规方程。
