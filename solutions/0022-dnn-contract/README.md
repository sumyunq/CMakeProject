# ONNX 推理：预处理、布局与契约：参考答案

先转RGB得到[255,127.5,0]，除255为[1,.5,0]，再减.5除.5得[1,0,-1]。实现可在CV_32FC3图上cvtColor、缩放、subtract、divide，再以不重复缩放的blobFromImage封装；或明确选择等价参数。先用这个像素作金标准单测，并把输入名、NCHW尺寸、输出语义写入随模型的契约文件。

## 为什么这样做

BGR像素[10,20,30]转RGB并除255，得到[.117647,.078431,.039216]。NCHW=[1,3,2,2]有12个float，通道的4个空间元素连续；NHWC=[1,2,2,3]总数也12，元素总数正确不代表布局正确。单通道1×1卷积权重2、偏置1作用于R值，输出1.235294。标准化若要求(x/255-mean)/std，应逐通道计算，不能只传mean却漏掉std。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 22 分支（16/17共享序列实验）。运行 `advanced_labs 22 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

完整模块dnn头opencv2/dnn.hpp：cv::dnn::Net net=cv::dnn::readNetFromONNX(path);检查empty，捕获cv::Exception；setPreferableBackend(DNN_BACKEND_OPENCV)、setPreferableTarget(DNN_TARGET_CPU)。以明确契约NCHW float32 RGB/255、输入image、输出scores为例，blobFromImage(bgr,1./255.,Size(224,224),Scalar(),true,false,CV_32F)，net.setInput(blob,"image")，net.forward("scores")，检查dims/type/总数/所有元素有限性。这只是匹配该契约的写法，真实模型须从导出说明核对名字与形状。首次forward有初始化成本，预热后计时；缺少模型、算子不支持、shape不符和不可用目标均需可见错误。

## 验收边界

离线程序只检查RGB、shape和标量卷积，不读取ONNX、不调用Net、不验证推理后端。metrics会明确记录这个边界。先用手算一个像素检查通道，再看首层前几个值；若结果整体差一个255倍通常是缩放重复或遗漏。result.png显示原始BGR色块，显示色块正常不能证明张量布局正确。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <array>
#include <cmath>
#include <optional>
std::optional<std::array<float, 3>> normalizeRgb(const std::array<float, 3>& bgr)
{
    for (const auto value : bgr) {
        if (!std::isfinite(value) || value < 0 || value > 255) { return std::nullopt; }
    }
    return std::array<float, 3>{(bgr[2] / 255.0F - .5F) / .5F,
        (bgr[1] / 255.0F - .5F) / .5F, (bgr[0] / 255.0F - .5F) / .5F};
}
```

normalizeRgb({0,127.5F,255})返回[1,0,-1]。封装blob时不能再除255；多像素NCHW偏移为((n*C+c)*H+y)*W+x。
