# 传统机器学习：特征、训练与独立评价：参考答案

z=(14-10)/2=2。部署仍用训练均值10、标准差2；用测试均值重算会改变模型语义并引入评价污染。以批次A/B训练、C验证、D测试，同一实物全部图像留在同集合；验证选择阈值，测试只报固定阈值的混淆矩阵与置信区间。标准差为0的特征删除或固定为0并记录规则。

## 为什么这样做

用一维特征[-2,-1,1,2]和标签[-1,-1,1,1]，拟合y=w·x+b最小二乘。Σx=0、Σxy=6、Σx²=10，所以w=.6、b=0。预测[-1.2,-.6,.6,1.2]的符号正确，但数值不是概率；边界x=0应作为不确定样本。正则化将分母改为10+λ，能收缩权重却不能解决数据泄漏。

## 实现与基线对照

可直接编译运行的完整数值实现见 `../../src/app/AdvancedLabs.cpp` 中 lesson == 21 分支（16/17共享序列实验）。运行 `advanced_labs 21 <output-directory>`，检查 `metrics.txt` 与无损PNG。练习新增参数按上述公式代入，不能把基线原参数输出冒充练习结果。

## 实际应用接法

固定OpenCV5.0.0源码不含旧opencv_ml模块，不能写find_package(... ml)或声称已运行cv::ml::SVM。本课采用core数值学习器，若业务需要SVM/随机森林，可明确引入另一个经过版本锁定的机器学习库，并用同样特征契约封装。OpenCV4文档中的cv::ml::SVM::create、setKernel/train/predict仅供迁移对照，不能在本工程5.0下直接编译。core的kmeans可无监督聚类但簇编号不是良品标签。模型文件应同时包含特征顺序、归一化、阈值、训练数据版本，加载后校验维度和有限性。

## 验收边界

core的solve以SVD求权重，验收w=.6及四个符号，报告独立x=-3/3的方向；图中线只是边界示意。这个例子训练与评价规模都极小，只用于解释算法，不能声称获得工业泛化能力。加入单个离群标签后重新拟合，观察最小二乘对异常值的敏感性。

程序对PNG写入/读取、数值断言与输出流检查失败返回非零；参考数值允许浮点容差，不允许单位不明。真实输入需另外建立批次隔离的评价集。

## 练习参考实现（独立函数，C++20）

下面的函数包含所有必要标准库头文件，可放入独立练习目标调用；不依赖相机或模型。

```cpp
#include <cmath>
#include <optional>
std::optional<double> standardize(double value, double trainMean, double trainSigma)
{
    if (!std::isfinite(value) || !std::isfinite(trainMean) || !std::isfinite(trainSigma)
        || trainSigma <= 0) { return std::nullopt; }
    return (value - trainMean) / trainSigma;
}
```

standardize(14,10,2)=2；测试/部署代码只读训练参数。批次划分应写入清单并断言每个实体ID只出现于一个集合。
