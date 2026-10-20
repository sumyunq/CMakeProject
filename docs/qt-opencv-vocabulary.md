# Qt 与 OpenCV 英文词汇学习手册

适用于本工程的 Qt 6.8.3、OpenCV 5.0.0 和 C++20。这里的 Open 指 OpenCV；OpenGL、OpenCL 是其他技术，见后面的缩写表。

学习目标是看懂官方文档、函数名、参数和报错，并能用自己的话解释代码。先学图像与内存词汇，再随课程学习算法术语，不必一次背完。

## 学习方法与顺序

1. 每次选 8～12 个词：看英文说中文，再遮住中文解释用途。
2. 找到对应 API 或项目代码，读出函数名里的动词和名词。
3. 用一个小例子解释词义；能预测结果比记住翻译更重要。
4. 次日、一周后再测。不会的词标记“待复习”，能解释且能使用后再标“已掌握”。

本表没有自动认定你已经掌握任何词。建议在本地笔记记录：`英文词 | 自己的解释 | 代码例子 | 复习日期 | 状态`。

| 学习批次 | 先掌握的词 | 对应材料 |
|---|---|---|
| 环境验证课 | configure、build、compile、link、runtime、dependency、Debug、Release | [第一课](../lessons/0001-environment.html) |
| Mat 入门课 | image、pixel、matrix、row、column、channel、depth、stride、ROI、shallow copy、deep copy、ownership | [第二课](../lessons/0002-image-mat.html) |
| Qt 显示边界 | widget、signal、slot、lifetime、buffer、pixel format、implicit sharing | 本手册的 Qt 词表与易混点 |
| 后续课程 | 滤波、几何、视频、工业视觉、AI、性能相关词 | [课程地图](curriculum.md) |

## 图像与 Mat 的核心词汇

| 英文 | 中文与实际含义 | API 或阅读提示 |
|---|---|---|
| image | 图像；这里通常表示二维像素数据 | `cv::Mat image` |
| pixel | 像素；一个位置可以包含多个通道值 | `pixel value` 是像素值 |
| matrix / matrices | 矩阵／矩阵的复数 | `cv::Mat` 可表示图像，也可表示一般矩阵 |
| row / rows | 行；二维图像中对应高度方向 | `image.rows`；访问时通常先写行索引 |
| column / cols | 列；对应宽度方向 | `image.cols` |
| width / height | 宽／高 | `cv::Size(width, height)` |
| coordinate | 坐标 | `cv::Point(x, y)` 中 x 对应列，y 对应行 |
| index | 索引，下标 | `image.at<uchar>(row, col)` |
| channel | 通道；每个像素的分量 | 灰度通常 1 通道，BGR 通常 3 通道 |
| depth | 每通道元素的数据类型类别 | `depth()`；不是整张图像的总位数 |
| type | 类型，组合了深度和通道数 | `CV_8UC3` 表示 8 位无符号、3 通道 |
| unsigned | 无符号 | 8 位无符号值范围为 0～255 |
| grayscale | 灰度图像 | 常见输入类型 `CV_8UC1` |
| color space | 色彩空间 | RGB、HSV；不要与通道排列或存储类型混为一谈 |
| BGR / RGB | 蓝绿红／红绿蓝的通道顺序 | 常见 OpenCV 彩图按 BGR 排列；Mat 本身不记录颜色语义 |
| alpha | 透明度分量 | BGRA 中的 A；不要混淆直通透明度与预乘透明度 |
| range | 范围、区间 | 索引范围常为左闭右开；数值范围取决于类型 |
| region of interest / ROI | 感兴趣区域 | `image(cv::Rect(x, y, width, height))` |
| rectangle / Rect | 矩形 | ROI 的位置与大小 |
| dimensions | 维度 | 二维矩阵与三维张量的“维数”不同于通道数 |
| empty | 空的，没有有效元素 | 读取或处理前检查 `empty()` |
| element | 元素 | `elemSize()` 是一个完整元素的字节数，含全部通道 |
| stride / step | 步长，相邻行起点的字节距离 | 二维 Mat 用 `step[0]`；QImage 用 `bytesPerLine()` |
| continuous | 连续的 | `isContinuous()`；二维图像行间没有额外间隙 |
| padding | 填充字节 | 行末可能有为对齐保留的空间 |

### 将类型名称拆开读

`CV_8UC3` → `8`：每通道 8 位；`U`：unsigned；`C3`：3 channels。每像素 3 字节，但整行步长未必等于宽度乘 3。

`CV_32FC1` → 每通道 32 位 floating point（浮点数），1 通道。不能直接当作 8 位灰度显示，需要明确显示范围和转换方式。

## 内存与所有权

| 英文 | 中文与实际含义 | API 或阅读提示 |
|---|---|---|
| buffer | 缓冲区；连续或按步长组织的存储空间 | `data` 指向像素区域 |
| header | 头部、描述信息 | Mat 的尺寸、类型、步长和数据指针等信息 |
| ownership | 所有权；谁负责释放资源 | 用外部内存构造 Mat 时，必须核对释放责任 |
| lifetime | 生命周期 | 指针有效期必须覆盖实际访问期 |
| reference count | 引用计数 | OpenCV 管理的数据可被多个 Mat 共享；外部缓冲区有不同约束 |
| shallow copy | 浅拷贝；复制描述信息，共享像素 | `auto alias = original;` |
| deep copy | 深拷贝；复制像素到独立存储 | `original.clone()` |
| alias | 别名；通过另一对象访问同一数据 | 修改 alias 可能改变 original |
| clone | 克隆，创建独立副本 | `clone()` 返回新的 Mat |
| copy | 复制；需要结合接口判断是否共享 | `copyTo(dst)` 写入目标；不要用同一词推断所有 API |
| allocation | 分配存储空间 | `allocation failure` 是分配失败 |
| dangling pointer | 悬空指针 | 缓冲区已释放，但指针仍被保留 |
| implicit sharing | 隐式共享 | Qt 的一些值类型通过共享减少复制成本 |
| copy on write / COW | 写时复制 | Qt 隐式共享对象在需要修改时可能分离数据 |
| detach | 分离共享数据 | Qt 文档中的 `detach` 关系到副本是否仍共享 |

Mat 的复制与 ROI 通常共享像素，写入不会自动按 Qt 的写时复制方式分离。QImage 的隐式共享也不能替你管理任意外部 Mat 缓冲区的生命周期。本工程转换接口返回拥有独立像素存储的 QImage。[Mat 源码说明](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)、[Qt 隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)

## Qt Widgets 常用词

| 英文 | 中文与实际含义 | API 或阅读提示 |
|---|---|---|
| widget | 控件、窗口部件 | `QWidget` |
| window / dialog | 窗口／对话框 | `QMainWindow`、`QDialog` |
| layout | 布局 | 管理控件的位置、大小和间距 |
| geometry | 几何位置和尺寸 | 布局管理中的 geometry 由布局计算 |
| margin / spacing | 边距／控件之间的间距 | 看布局相关参数 |
| size hint | 建议尺寸 | 不等于强制固定尺寸 |
| parent / child | 父对象／子对象 | QObject 对象树还承担生命周期管理 |
| signal | 信号；对象发出的通知 | 例如按钮的 `clicked` |
| slot | 槽；接收通知的函数 | 同时仍是可直接调用的 C++ 函数 |
| emit | 发射信号 | 不表示自动创建工作线程 |
| connect / disconnect | 建立／断开连接 | 函数指针语法便于编译期检查 |
| sender / receiver | 发送者／接收者 | 阅读连接关系时先找这两个对象 |
| event | 事件 | 键盘、鼠标、绘制等事件 |
| event loop | 事件循环 | 分发事件及队列调用；阻塞主线程会影响界面响应 |
| queued connection | 队列连接 | 槽稍后在接收对象所属线程执行，需要相应事件循环 |
| thread affinity | 线程归属 | QObject 属于哪个线程，不等于它的所有函数都自动在该线程执行 |
| property | 属性 | 与简单成员变量概念有关联，但不完全相同 |
| model / view | 模型／视图 | 数据与显示职责分离 |
| resource | 资源 | `:/` 路径访问编译进程序的资源 |
| stylesheet / QSS | 样式表 | 本工程保存在独立 `.qss` 文件中 |
| pixel format | 像素格式 | `QImage::Format_RGB888`；核对顺序、位深和透明度含义 |
| paint / render | 绘制／渲染 | 图像数据处理与控件绘制是两个环节 |

信号槽描述的是对象通信关系；连接类型决定槽何时、在哪个线程执行。不要把 signal 翻译成“异步任务”。[Qt 信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)

## 环境与构建词汇

| 英文 | 中文与实际含义 | 当前工程中的位置 |
|---|---|---|
| source / header | 源文件／头文件 | `.cpp`／`.h` |
| dependency | 依赖项 | Qt、OpenCV、spdlog |
| toolchain | 工具链 | 编译器、链接器及相关工具的组合 |
| configure / generate | 配置／生成构建系统 | CMake 检查依赖并生成 Ninja 构建规则 |
| build | 构建 | 通常包括编译、链接和必要的生成步骤 |
| compile | 编译 | 将源代码翻译成目标文件 |
| link | 链接 | 解析符号、组合目标文件与库 |
| target | 构建目标 | `mat_demo`、`vision_viewer` |
| preset | 预设 | `local-debug`、`local-release` |
| cache | 缓存 | `CMakeCache.txt` 保存配置结果 |
| Debug / Release | 调试配置／发布配置 | 影响优化、调试信息、运行库及日志策略 |
| runtime | 运行时；运行所需的环境或支持库 | 成功链接不代表运行时 DLL 一定齐全 |
| static / shared library | 静态库／共享库 | Windows `.lib` 也可能是 DLL 导入库，不能仅看扩展名判断 |
| deployment | 部署 | 将程序和所需依赖放到可运行目录 |
| symbol | 符号 | 函数或对象的链接名称；`unresolved external symbol` 表示未解析外部符号 |
| assertion | 断言 | 检查程序应满足的条件，失败需要回查输入和契约 |
| warning / error | 警告／错误 | 先看第一条关键诊断，再追后续信息 |

## 随后续课程逐步学习的视觉词汇

| 主题 | 英文词汇 | 中文与区分 |
|---|---|---|
| 统计 | histogram；normalization | 直方图；归一化 |
| 分割基础 | threshold；binary image；mask | 阈值；二值图；掩膜（指定参与处理的位置） |
| 滤波 | kernel；convolution；correlation | 核；卷积；相关。OpenCV `filter2D` 实际计算相关，核翻转关系需单独学习 |
| 平滑 | blur；Gaussian；median | 模糊；高斯；中值 |
| 形态学 | erosion；dilation；opening；closing | 腐蚀；膨胀；开运算；闭运算 |
| 边缘 | gradient；edge；contour | 梯度；边缘；轮廓，三者不能互换 |
| 区域 | connected component；bounding box | 连通域；包围框 |
| 几何 | resize；interpolation；warp | 调整尺寸；插值；几何变换映射 |
| 配准 | feature；descriptor；matching；homography | 特征；描述子；匹配；单应变换 |
| 相机 | calibration；intrinsic；extrinsic；distortion | 标定；内参；外参；畸变 |
| 三维 | pose；stereo；disparity；depth map | 位姿；双目；视差；深度图，这里的 depth 表示距离相关量 |
| 视频 | frame；frame rate；timestamp | 帧；帧率；时间戳 |
| 运动 | optical flow；tracking；motion | 光流；跟踪；运动 |
| 工业视觉 | inspection；defect；measurement；tolerance | 检测检验；缺陷；测量；公差 |
| 精度 | accuracy；precision；repeatability；uncertainty | 准确度；精密度；重复性；不确定度，需结合测量语境区分 |
| AI 数据 | tensor；batch；shape；layout | 张量；批次；各维大小；维度排列 |
| AI 流程 | training；inference；preprocessing；postprocessing | 训练；推理；预处理；后处理 |
| AI 任务 | classification；detection；segmentation | 分类；目标检测；分割 |
| AI 输出 | confidence；IoU；NMS | 置信度；交并比；非极大值抑制 |
| 性能 | latency；throughput；benchmark；profiling | 延迟；吞吐量；基准测试；性能剖析 |
| 并发 | asynchronous；synchronization；race condition | 异步；同步协调；竞态条件 |

这些词是后续路线的预习索引，不表示当前工程已经启用了视频、DNN 或 GPU 模块。

## 缩写与易混点

| 缩写或词组 | 全称或区分 |
|---|---|
| OpenCV | Open Source Computer Vision Library，开源计算机视觉库 |
| Qt | 框架名称，不按首字母缩写展开；通常读作英语单词 cute |
| GUI | Graphical User Interface，图形用户界面 |
| API | Application Programming Interface，应用程序编程接口 |
| ROI | Region of Interest，感兴趣区域 |
| FPS | Frames Per Second，每秒帧数 |
| DNN / CNN | Deep Neural Network／Convolutional Neural Network，深度神经网络／卷积神经网络 |
| ONNX | Open Neural Network Exchange，模型交换格式及相关规范 |
| SIMD | Single Instruction Multiple Data，单指令多数据 |
| CPU / GPU | Central Processing Unit／Graphics Processing Unit，中央处理器／图形处理器 |
| OpenGL / OpenCL | Open Graphics Library／Open Computing Language；分别面向图形与异构计算，和 OpenCV 是不同项目 |
| depth | Mat 的元素深度、相机测得的空间深度、神经网络层数是不同含义 |
| precision / recall | 分类检测指标中的精确率／召回率；precision 在测量领域又译为精密度 |
| size / step | 尺寸表示多少行列，步长表示内存地址间隔 |
| channel / dimension | 多通道二维图像仍可有 `dims == 2`，不能把 3 通道直接说成 3 维矩阵 |
| reference / pointer | 引用／指针；reference count 中 reference 指对共享资源的引用关系 |

## 从 API 名称学习英文

| API | 拆词 | 读代码时的理解 |
|---|---|---|
| `cvtColor` | convert + color | 转换颜色表示；具体转换由参数指定 |
| `imread` / `imwrite` | image + read / write | 读图／写图；还要检查失败结果 |
| `setTo` | set + to | 将选定元素设置为指定值 |
| `isContinuous` | is + continuous | 是否连续存储；布尔查询 |
| `bytesPerLine` | bytes + per + line | 每行占用的字节数 |
| `findContours` | find + contours | 查找轮廓 |
| `connectedComponents` | connected + components | 标记连通区域 |
| `warpPerspective` | warp + perspective | 透视映射 |
| `setObjectName` | set + object + name | 设置 QObject 名称；可供 QSS 选择器使用 |

## 常见文档句式和报错

以下为学习用自编句子，不是官方文档原文。

| 英文句子 | 中文理解 | 下一步检查 |
|---|---|---|
| The matrix shares its data with the original. | 该矩阵与原矩阵共享数据。 | 修改是否会互相可见？ |
| The function returns an empty matrix on failure. | 该函数失败时返回空矩阵。 | 调用后是否检查 `empty()`？ |
| The buffer must remain valid during the operation. | 操作期间缓冲区必须始终有效。 | 谁持有它？会不会提前释放？ |
| The destination has the same size and type as the source. | 目标与源的尺寸和类型相同。 | 不要把 same type 理解成相同像素值。 |
| This operation does not modify the source. | 此操作不会修改源数据。 | 看接口契约及共享数据条件。 |
| No matching function for call | 没有匹配该调用的函数。 | 参数数量、类型、重载、版本。 |
| Unresolved external symbol | 未解析的外部符号。 | 实现是否编译、库是否链接、配置是否匹配。 |
| Access violation | 非法内存访问。 | 越界、悬空指针、缓冲区生命周期。 |
| Assertion failed | 断言失败。 | 条件表达式、输入尺寸和类型。 |
| Could not find a package configuration file | 找不到依赖包的配置文件。 | 安装位置、预设、实际使用的 CMake。 |

## 独立自测

先自行回答，再展开答案。这里测试词义，不替代课程的代码练习。

1. 将“这个 ROI 共享原图像素，但它可能不是连续存储的”用英文说出来。
2. `row=2, col=5` 对应的 `(x,y)` 是什么？
3. `CV_8UC3` 的 depth、channels、每像素字节数分别是什么？能否仅由类型认定 BGR？
4. Why does a shallow copy not provide independent pixel storage?
5. signal、slot、queued connection 各描述什么？
6. 编译成功但启动时缺 DLL，属于 compile、link 还是 runtime 问题？
7. 解释 stride 与 width 的区别，再举出一种它们数值不相等的情况。
8. 为什么不能用 Qt 的 implicit sharing 推断 Mat 修改时会自动复制？

<details>
<summary>展开参考答案</summary>

1. This ROI shares the original image data, but it may not be continuous.
2. `(5,2)`。x 对应列，y 对应行。
3. 深度类别 `CV_8U`；3 通道；3 字节。不能，类型不记录颜色语义。
4. A shallow copy shares the pixel buffer with the original. 它没有为这些像素创建独立副本。
5. signal 是通知，slot 是响应函数，queued connection 表示排队后在接收对象所属线程执行；需要相应事件循环。
6. runtime。若链接阶段找不到符号才是 link 问题。
7. width 的单位是像素；stride 的单位通常是字节。宽 10 的 CV_8UC3 紧密排列图像，行步长为 30 字节；窄 ROI 还可能沿用更宽原图的步长。
8. 两者的共享机制不同，Mat 共享像素写入不会自动分离。要独立副本，应明确使用 `clone()` 等合适操作。

</details>

## 官方资料与后续扩充

- [OpenCV 5.0.0 Mat 头文件与说明](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)：当前固定版本的数据、共享、步长说明。
- [Qt 6.8 信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)：signal、slot、connect。
- [Qt 6.8 隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)：reference counting、copy on write。
- [Qt 6.8 QImage](https://doc.qt.io/qt-6.8/qimage.html)：像素格式、外部缓冲区与生命周期。
- [项目官方资料索引](../RESOURCES.md)：随课程查阅对应模块。

Qt 在线 6.8 文档随该系列补丁更新，不是专门的 6.8.3 冻结快照；涉及版本新增接口时仍需核对本机头文件。后续每课追加实际遇到的词、完整句子和自己的代码例子。

维护记录：2026-10-20，wbliu。
