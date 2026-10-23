# 本课程使用 OpenCV 5 时怎样读旧教程

固定依据为仓库中的 5.0.0 tag 源码，不跟随在线滚动文档自动升级。本表按实际模块树核对。

| 旧教程常见写法 | 当前阅读方法 |
|---|---|
| features2d | 5.0.0 模块树为 features；核对 opencv2/features.hpp、opencv_features 目标及算法参数 |
| calib3d | 功能拆到 calib、geometry、stereo；按实际函数声明选择头文件和链接库，不能只全局替换字符串 |
| cv::ml、opencv_ml | 当前固定主仓没有 ml 模块；第21课提供数值分类器，SVM/随机森林需选择额外库或独立4.x环境，不能伪装为5原生能力 |
| cv::Mat 二维图像 | 仍需明确dims、channels及step；5扩展张量语义不表示普通图像自动具有模型的NCHW布局 |
| DNN模型能读取就能预测 | 需要检查opset、算子、输入形状/布局、预处理、输出定义及后端；第22课和原生实验分别验证数值契约与实际加载 |
| HighGUI窗口与Qt窗口混用 | 本项目GUI由Qt Widgets负责；默认不启用OpenCV的Qt HighGUI后端 |
| CUDA示例 | 必须核对相应模块、设备与依赖；CPU/UMat不自动等于CUDA执行 |

## 自主核对步骤

在固定源码 `modules` 下搜索函数名；看头文件声明及所属目录，再查该模块 CMakeLists 的依赖。用最小输入编译运行，检查返回类型、异常和数值；保留旧教程链接及你修订的理由。高级安装的构建记录保存模块与选项。

官方入口：[4到5迁移说明](https://github.com/opencv/opencv/wiki/OpenCV-4-to-5-migration)、[5.0.0模块树](https://github.com/opencv/opencv/tree/5.0.0/modules)。本地源码优先于未经版本核对的博客；每课的引用和原生示例补充具体API。
