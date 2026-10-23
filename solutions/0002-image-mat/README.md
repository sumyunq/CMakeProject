# 02 图像与 Mat 参考解答

下面是独立练习的一种完整数值方案。先完成自己的预测，再对照。

选4×6灰度矩阵全为1，ROI=Rect(1,1,3,2)，浅拷贝alias=parent，独立副本independent=parent.clone()。parent的行步长6字节；ROI每行3字节但行步长仍6，共2行所以非连续。alias也是4×6；independent的像素起初与parent相同但存储独立。

依次执行ROI.setTo(7)、alias.at<uchar>(0,0)=9、independent.at<uchar>(3,5)=20。parent与alias的(0,0)为9，ROI对应6个像素为7，其他像素仍1；independent除了(3,5)=20其余全1。ROI像素和为42，parent像素和为24+6×6+8=68，independent像素和为24+19=43。

逐行实现应为：

```cpp
double sumRows(const cv::Mat& image)
{
    if (image.empty() || image.dims != 2 || image.type() != CV_8UC1) {
        throw std::invalid_argument("requires nonempty 2D CV_8UC1");
    }
    double sum = 0;
    for (int row = 0; row < image.rows; ++row) {
        const auto* pixels = image.ptr<uchar>(row);
        for (int column = 0; column < image.cols; ++column) {
            sum += pixels[column];
        }
    }
    return sum;
}
```

此函数放进自己的练习.cpp，包含opencv2/core.hpp与stdexcept，按项目目标显式加入构建。与`cv::sum(roi)[0]`比较，二者应为42；再测clone后的ROI，同样为42。空矩阵、CV_8UC3输入应明确拒绝。

安全反例：假设ROI连续，连续读roi.data前6字节会把第一行ROI右侧的父图像素读进来；本例这6字节为7,7,7,1,1,1，总和24，不是42。这里仍在父图有效存储中，能够安全演示逻辑错误；不要继续读到父图范围之外。

外部内存构造Mat不自动延长其所有者生命周期；引用计数不保证共享像素并发读写安全。解释这两点，并证明你自己的数据方案也满足预测，即可自行验收。
