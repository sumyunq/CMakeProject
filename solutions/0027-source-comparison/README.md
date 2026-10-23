# 阅读源码：相关、卷积与可验证对照：完整参考解答

## 解题思路与实现

参考阅读路线：在锁定OpenCV5源码中搜索filter2D公开入口，沿参数校验、边界解析、HAL/优化分发、普通回退逐层记录文件和行号。不要直接把某个SIMD函数当唯一执行路径。先用单线程CV_32F小矩阵固定语义，再扩大核观察是否走不同实现。手写实现每个输出遍历ky/kx，越界按指定边界取值，累加double后转float；两实现NORM_INF≤1e-4。翻核负例至少一像素差>1。源码精确入口以本机5.0.0 checkout为准。

## 数值推导

相关g(x)=Σk(i)f(x+i-anchor)，卷积先翻转核。核[1,0,-1]作用于[11,12,13]，相关得到11-13=-2；翻核后得到+2。对称核翻转不变，所以不能验证方向。边界固定为BORDER_CONSTANT零填充，左端为0-f(1)，不能与默认反射边界对照。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 27`，或 `engineering_labs 27 artifacts/lesson27`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

27生成5×5矩阵0..24，手写逐行相关，并与filter2D的CV_32F输出比较。max_error应为0，中心-2，翻核中心+2。27-response-offset128.png把响应乘4再加128便于看负值；显示图不能代替浮点原始精度检验。最大绝对误差使用NORM_INF；逐元素相等只适合本例整数可精确表示的情形。

## 独立扩展的实施顺序

扩展到3×3非对称核[1,2,0;0,-1,0;0,0,3]，实现零边界和复制边界两种版本。输入使用小尺寸手算、随机固定种子和非连续ROI。将Mat深度改为8U观察负值饱和，解释为什么不能用饱和结果判断相关方向。

## 逐项验收

手算中心、边界一致、浮点误差、翻核负例、源码调用链证据各20分。合格≥80且不能漏掉边界与负例。

## 自测详解

1. 对称核足以区分相关卷积吗？不能。2. 相同中心像素能说明边界正确吗？不能，要单独验四角四边。3. 不同SIMD路径能否允许任意误差？不能，先给定数据类型与容差再验证。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。

## 任意3×3核的完整参考累加

```cpp
cv::Mat correlateZero(const cv::Mat& input, const cv::Mat& kernel) {
    if(input.empty() || input.dims!=2 || kernel.dims!=2 || input.type()!=CV_32FC1 || kernel.type()!=CV_32FC1
       || kernel.rows!=3 || kernel.cols!=3) { throw std::invalid_argument("shape/type"); }
    cv::Mat output(input.size(), CV_32F, cv::Scalar(0));
    for(int y=0; y<input.rows; ++y) {
        for(int x=0; x<input.cols; ++x) {
            double total=0;
            for(int ky=0; ky<3; ++ky) {
                for(int kx=0; kx<3; ++kx) {
                    const int sy=y+ky-1, sx=x+kx-1;
                    if(sy>=0 && sy<input.rows && sx>=0 && sx<input.cols) {
                        total+=static_cast<double>(input.at<float>(sy,sx))*kernel.at<float>(ky,kx);
                    }
                }
            }
            output.at<float>(y,x)=static_cast<float>(total);
        }
    }
    return output;
}
```
复制边界版本把sy、sx分别用std::clamp限制到有效范围，取消越界跳过，其他累加不变；对照filter2D必须传BORDER_REPLICATE。
