# Qt 与 OpenCV 关键术语辨析

核对日期：2026-10-26。范围：本工程中容易因同词异义而误学、误译的关键概念。

每个词条对应一个概念；同一英文在不同领域分别立项。英文概念对照下列一手资料；中文是本项目推荐译法，并非声称原作者发布了官方中文译文。例句均为自编，不是原文引述。Qt在线6.8文档会随补丁更新，API行为仍以本机6.8.3和固定OpenCV5源码为准。ONNX与scikit-learn资料仅说明概念，不改变工程依赖版本。

[全部词汇速查](../qt-opencv-vocabulary.md) · [CSV术语库](critical-terms.csv)

学习时先确认领域，再看译名；不要按英文字符串做全局替换。例如registration在图像中是配准，在Qt元类型系统中是注册。

## 词条索引

| 编号 | 英文 | 领域 | 推荐译法 |
|---|---|---|---|
| TERM-001 | [depth](#term-001) | Mat元素类型 | 元素深度 |
| TERM-002 | [depth](#term-002) | 相机与三维 | 深度 |
| TERM-003 | [stride](#term-003) | 图像内存 | 行步长 |
| TERM-004 | [stride](#term-004) | 卷积算子 | 步幅 |
| TERM-005 | [padding](#term-005) | 图像内存 | 填充字节 |
| TERM-006 | [padding](#term-006) | 卷积算子 | 填充 |
| TERM-007 | [channel](#term-007) | 图像表示 | 通道 |
| TERM-008 | [pixel format](#term-008) | Qt图像存储 | 像素格式 |
| TERM-009 | [continuous](#term-009) | Mat存储布局 | 连续存储的 |
| TERM-010 | [shallow copy](#term-010) | Mat所有权 | 浅拷贝 |
| TERM-011 | [deep copy](#term-011) | Mat所有权 | 深拷贝 |
| TERM-012 | [implicit sharing](#term-012) | Qt值类型 | 隐式共享 |
| TERM-013 | [copy-on-write](#term-013) | Qt共享数据 | 写时复制 |
| TERM-014 | [thread affinity](#term-014) | QObject线程模型 | 线程亲和性 |
| TERM-015 | [signal](#term-015) | Qt对象通信 | 信号 |
| TERM-016 | [slot](#term-016) | Qt对象通信 | 槽；槽函数 |
| TERM-017 | [emit](#term-017) | Qt信号用语 | 发射 |
| TERM-018 | [queued connection](#term-018) | Qt对象通信 | 队列连接 |
| TERM-019 | [registration](#term-019) | 图像几何 | 配准 |
| TERM-020 | [registration](#term-020) | Qt元类型系统 | 注册 |
| TERM-021 | [pose](#term-021) | 相机与机器人几何 | 位姿 |
| TERM-022 | [calibration](#term-022) | 计算机视觉相机模型 | 标定 |
| TERM-023 | [calibration](#term-023) | 计量学 | 校准 |
| TERM-024 | [accuracy](#term-024) | 计量学 | 测量准确度 |
| TERM-025 | [precision](#term-025) | 计量学 | 测量精密度 |
| TERM-026 | [precision](#term-026) | 分类与检测指标 | 精确率 |
| TERM-027 | [recall](#term-027) | 分类与检测指标 | 召回率 |
| TERM-028 | [accuracy](#term-028) | 分类指标 | 准确率 |
| TERM-029 | [convolution](#term-029) | 离散信号处理 | 卷积 |
| TERM-030 | [correlation](#term-030) | 图像滤波 | 相关；相关运算 |
| TERM-031 | [inference](#term-031) | 深度学习运行 | 推理 |

<a id="term-001"></a>
### TERM-001 · depth

**词性：** 名词。**领域：** Mat元素类型。**推荐译法：** 元素深度。

**概念：** 每个通道分量的数据类型类别，如CV_8U、CV_32F；既涉及位数，也涉及数值表示。

**易误译与边界：** 不要直接译为每像素位数；CV_8UC3不是每通道24位。

**自编例句：** The image depth is CV_8U, and the image has three channels.

**对应译文：** 图像的元素深度为8位无符号整数，共3个通道。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-002"></a>
### TERM-002 · depth

**词性：** 名词。**领域：** 相机与三维。**推荐译法：** 深度。

**概念：** 依照坐标约定表示的空间深度，例如针孔模型中相机坐标Z；应注明单位。

**易误译与边界：** 不要与Mat的depth()或相机到目标的欧氏距离自动等同。

**自编例句：** Depth is expressed in millimetres in this experiment.

**对应译文：** 本实验以毫米表示深度。

**概念来源：** [OpenCV 5.0.0几何与位姿](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-003"></a>
### TERM-003 · stride

**词性：** 名词。**领域：** 图像内存。**推荐译法：** 行步长。

**概念：** 二维Mat相邻两行起始地址的字节间隔，对应step[0]。

**易误译与边界：** 不是宽度，也不总等于width乘每像素字节数。

**自编例句：** The ROI keeps the parent image's row stride.

**对应译文：** 该ROI保留父图的行步长。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-004"></a>
### TERM-004 · stride

**词性：** 名词。**领域：** 卷积算子。**推荐译法：** 步幅。

**概念：** 卷积窗口沿空间轴移动的采样间隔，可按各轴分别指定。

**易误译与边界：** 单位是空间位置步数，不是Mat行地址间的字节数。

**自编例句：** A stride of two reduces the number of sampled positions.

**对应译文：** 步幅为2会减少被采样的位置数量。

**概念来源：** [ONNX Conv算子定义，仅作算子概念参考](https://onnx.ai/onnx/operators/onnx__Conv.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-005"></a>
### TERM-005 · padding

**词性：** 名词。**领域：** 图像内存。**推荐译法：** 填充字节。

**概念：** 行存储中可能为对齐等原因保留、不属于有效像素的字节。

**易误译与边界：** 不要把它当作额外图像内容；并非每张图都含行末填充。

**自编例句：** Padding bytes are not additional pixels.

**对应译文：** 填充字节不是额外的像素。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-006"></a>
### TERM-006 · padding

**词性：** 名词。**领域：** 卷积算子。**推荐译法：** 填充。

**概念：** 按算子约定在输入边界外补充位置或数值，以影响边界计算和输出大小。

**易误译与边界：** 不是行末字节对齐；same padding还要核对步幅和取整规则。

**自编例句：** The convolution uses zero padding at the border.

**对应译文：** 该卷积在边界使用零填充。

**概念来源：** [ONNX Conv算子定义，仅作算子概念参考](https://onnx.ai/onnx/operators/onnx__Conv.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-007"></a>
### TERM-007 · channel

**词性：** 名词。**领域：** 图像表示。**推荐译法：** 通道。

**概念：** 一个元素的分量，例如BGR像素含3个通道。

**易误译与边界：** 通道数不等于Mat的dims；三通道二维图像仍可dims==2。

**自编例句：** This matrix has three channels but only two dimensions.

**对应译文：** 该矩阵有3个通道，但只有2个维度。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-008"></a>
### TERM-008 · pixel format

**词性：** 名词短语。**领域：** Qt图像存储。**推荐译法：** 像素格式。

**概念：** QImage等接口约定的像素数值表示、分量组织及透明度相关规则。

**易误译与边界：** 不要与色彩空间或文件格式PNG混用。

**自编例句：** Check the pixel format before interpreting the buffer.

**对应译文：** 解释缓冲区之前，先检查像素格式。

**概念来源：** [Qt 6.8 QImage像素格式](https://doc.qt.io/qt-6.8/qimage.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-009"></a>
### TERM-009 · continuous

**词性：** 形容词。**领域：** Mat存储布局。**推荐译法：** 连续存储的。

**概念：** 在isContinuous()语境中，表示元素间无需跨越额外行间隙的布局特性。

**易误译与边界：** 不是数学函数连续性；普通英文contiguous也常用来描述相邻存储。

**自编例句：** This ROI is not continuous, so process it row by row.

**对应译文：** 该ROI不是连续存储的，应逐行处理。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-010"></a>
### TERM-010 · shallow copy

**词性：** 名词短语。**领域：** Mat所有权。**推荐译法：** 浅拷贝。

**概念：** 复制矩阵描述信息，并共享像素存储的复制行为。

**易误译与边界：** 不是自动产生独立像素快照；修改共享像素可互相可见。

**自编例句：** A shallow copy shares the pixel buffer with the original.

**对应译文：** 浅拷贝与原对象共享像素缓冲区。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-011"></a>
### TERM-011 · deep copy

**词性：** 名词短语。**领域：** Mat所有权。**推荐译法：** 深拷贝。

**概念：** 把像素复制到独立存储；clone()是本项目常用方式。

**易误译与边界：** 不能只凭copy这个单词判断深浅，必须看具体接口。

**自编例句：** Use a deep copy when independent pixel storage is required.

**对应译文：** 需要独立像素存储时，使用深拷贝。

**概念来源：** [OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-012"></a>
### TERM-012 · implicit sharing

**词性：** 名词短语。**领域：** Qt值类型。**推荐译法：** 隐式共享。

**概念：** Qt中部分值类型共享内部数据，并在需要时进行数据分离的机制。

**易误译与边界：** 不能据此推断Mat写入会自动分离，也不能保证任意外部缓冲区有效。

**自编例句：** Implicit sharing can postpone copying the data.

**对应译文：** 隐式共享可以推迟数据复制。

**概念来源：** [Qt 6.8隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-013"></a>
### TERM-013 · copy-on-write

**词性：** 名词短语；作定语时连字符连接。**领域：** Qt共享数据。**推荐译法：** 写时复制。

**概念：** 共享对象需要修改数据时，按具体类型的规则先分离数据。

**易误译与边界：** 不要把它称为线程同步机制；绕过类型接口访问外部指针另有约束。

**自编例句：** Copy-on-write does not replace synchronization.

**对应译文：** 写时复制不能代替同步。

**概念来源：** [Qt 6.8隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-014"></a>
### TERM-014 · thread affinity

**词性：** 名词短语。**领域：** QObject线程模型。**推荐译法：** 线程亲和性。

**概念：** QObject所属线程的关系；学习时可解释为线程归属。

**易误译与边界：** 不是CPU核心绑定；普通成员函数调用不会自动跳到对象所属线程。

**自编例句：** The receiver has affinity to the GUI thread.

**对应译文：** 接收对象的线程亲和性指向GUI线程，即它属于GUI线程。

**概念来源：** [Qt 6.8 QObject线程亲和性](https://doc.qt.io/qt-6.8/qobject.html#thread-affinity)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-015"></a>
### TERM-015 · signal

**词性：** 名词。**领域：** Qt对象通信。**推荐译法：** 信号。

**概念：** 用于通知状态变化或事件发生的对象通信接口。

**易误译与边界：** 不是异步任务的同义词；执行方式取决于连接类型。

**自编例句：** The object emits a signal when its state changes.

**对应译文：** 对象状态变化时会发射信号。

**概念来源：** [Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-016"></a>
### TERM-016 · slot

**词性：** 名词。**领域：** Qt对象通信。**推荐译法：** 槽；槽函数。

**概念：** 可接收信号调用的函数，同时仍可按普通C++规则直接调用。

**易误译与边界：** 在Qt语境不译为硬件插槽；也不意味着自动在子线程运行。

**自编例句：** The slot updates the displayed value.

**对应译文：** 该槽函数更新显示值。

**概念来源：** [Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-017"></a>
### TERM-017 · emit

**词性：** 动词；Qt中也是宏标记。**领域：** Qt信号用语。**推荐译法：** 发射。

**概念：** 常见搭配emit a signal，表示发出信号通知。

**易误译与边界：** 不是创建线程、投递任意任务或保证异步执行。

**自编例句：** Emit the signal after updating the model.

**对应译文：** 更新模型之后再发射信号。

**概念来源：** [Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-018"></a>
### TERM-018 · queued connection

**词性：** 名词短语。**领域：** Qt对象通信。**推荐译法：** 队列连接。

**概念：** 通过事件循环安排槽在接收对象所属线程执行的连接方式。

**易误译与边界：** 不要译成线程池；接收方需要能处理相应事件。

**自编例句：** A queued connection delivers the call through the event loop.

**对应译文：** 队列连接通过事件循环递送调用。

**概念来源：** [Qt 6.8 QObject线程亲和性](https://doc.qt.io/qt-6.8/qobject.html#thread-affinity)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-019"></a>
### TERM-019 · registration

**词性：** 名词。**领域：** 图像几何。**推荐译法：** 配准。

**概念：** 估计并应用图像之间的空间对应关系或变换。

**易误译与边界：** 此语境不是软件注册，也不是简单地把尺寸改成相同。

**自编例句：** Registration aligns the sample image with the reference image.

**对应译文：** 配准使样本图像与参考图像在空间上对齐。

**概念来源：** [OpenCV 5.0.0仿射和几何配准](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-020"></a>
### TERM-020 · registration

**词性：** 名词。**领域：** Qt元类型系统。**推荐译法：** 注册。

**概念：** 把类型等信息登记到相应系统，以供后续查询或调用。

**易误译与边界：** 此语境不译为图像配准；常见搭配type registration。

**自编例句：** Type registration and image registration are different concepts.

**对应译文：** 类型注册与图像配准是不同概念。

**概念来源：** [Qt 6.8 QMetaType注册](https://doc.qt.io/qt-6.8/qmetatype.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-021"></a>
### TERM-021 · pose

**词性：** 名词。**领域：** 相机与机器人几何。**推荐译法：** 位姿。

**概念：** 通常包含位置和朝向；在PnP中必须说明坐标变换方向和尺度单位。

**易误译与边界：** 不要只译为姿态而遗漏平移；orientation通常强调朝向。

**自编例句：** The estimated pose contains rotation and translation.

**对应译文：** 估计的位姿包含旋转和平移。

**概念来源：** [OpenCV 5.0.0几何与位姿](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-022"></a>
### TERM-022 · calibration

**词性：** 名词。**领域：** 计算机视觉相机模型。**推荐译法：** 标定。

**概念：** 根据观测和模型估计相机内参、畸变等参数的过程。

**易误译与边界：** 不是把图像调好看；也不与每帧PnP位姿估计等同。

**自编例句：** Camera calibration estimates the intrinsic parameters.

**对应译文：** 相机标定估计内参。

**概念来源：** [OpenCV 5.0.0相机标定](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-023"></a>
### TERM-023 · calibration

**词性：** 名词。**领域：** 计量学。**推荐译法：** 校准。

**概念：** 在规定条件下建立标准量值、仪器示值与测量结果之间关系的操作。

**易误译与边界：** 不要与adjustment调整或verification核查混同。

**自编例句：** Calibration is not the same operation as adjustment.

**对应译文：** 校准与调整不是同一种操作。

**概念来源：** [VIM3 2.39 calibration](https://jcgm.bipm.org/vim/en/2.39.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-024"></a>
### TERM-024 · accuracy

**词性：** 名词。**领域：** 计量学。**推荐译法：** 测量准确度。

**概念：** 测得量值与被测量真值之间接近程度的概念。

**易误译与边界：** VIM不把准确度本身定义为可直接给数值的量；应另报告误差或不确定度。

**自编例句：** Accuracy and precision describe different aspects of measurement.

**对应译文：** 准确度与精密度描述测量的不同方面。

**概念来源：** [VIM3 2.13 measurement accuracy](https://jcgm.bipm.org/vim/en/2.13.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-025"></a>
### TERM-025 · precision

**词性：** 名词。**领域：** 计量学。**推荐译法：** 测量精密度。

**概念：** 规定条件下重复测量所得示值或测得量值之间的一致程度。

**易误译与边界：** 不要笼统写成精度，更不等于测值接近真值。

**自编例句：** Repeated measurements may be precise but biased.

**对应译文：** 重复测量结果可能很集中，但仍有偏差。

**概念来源：** [VIM3 2.15 measurement precision](https://jcgm.bipm.org/vim/en/2.15.html)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-026"></a>
### TERM-026 · precision

**词性：** 名词。**领域：** 分类与检测指标。**推荐译法：** 精确率。

**概念：** 预测为正的样本中真正为正的比例，TP/(TP+FP)。

**易误译与边界：** 不是计量学精密度，也不是整体分类准确率；分母为零要声明处理策略。

**自编例句：** Precision decreases when false positives increase, with true positives fixed.

**对应译文：** 真正例数量不变时，假正例增加会使精确率下降。

**概念来源：** [scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-027"></a>
### TERM-027 · recall

**词性：** 名词。**领域：** 分类与检测指标。**推荐译法：** 召回率。

**概念：** 实际正样本中被正确检出的比例，TP/(TP+FN)。

**易误译与边界：** 不是日常英语的回忆；不要和precision交换分母。

**自编例句：** Recall measures how many actual positives are retrieved.

**对应译文：** 召回率衡量实际正样本中有多少被检出。

**概念来源：** [scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-028"></a>
### TERM-028 · accuracy

**词性：** 名词。**领域：** 分类指标。**推荐译法：** 准确率。

**概念：** 分类正确的样本数占总样本数的比例；简单二分类可写(TP+TN)/总数。

**易误译与边界：** 不是precision；类别不平衡时，单独报告它可能掩盖漏检。

**自编例句：** High accuracy can hide poor recall on rare defects.

**对应译文：** 较高的准确率可能掩盖稀有缺陷召回率很差的问题。

**概念来源：** [scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-029"></a>
### TERM-029 · convolution

**词性：** 名词。**领域：** 离散信号处理。**推荐译法：** 卷积。

**概念：** 按卷积定义对核作相应翻转后计算的滑动加权运算。

**易误译与边界：** OpenCV filter2D直接计算的是相关；对称核会掩盖区别。

**自编例句：** For an asymmetric kernel, convolution and correlation can differ.

**对应译文：** 对于非对称核，卷积与相关的结果可能不同。

**概念来源：** [OpenCV 5.0.0 filter2D声明](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-030"></a>
### TERM-030 · correlation

**词性：** 名词。**领域：** 图像滤波。**推荐译法：** 相关；相关运算。

**概念：** 在filter2D语境中，按未翻转的核进行滑动加权。

**易误译与边界：** 不要机械译为相关系数；这里不是统计意义的Pearson系数。

**自编例句：** The filter applies correlation without flipping the kernel.

**对应译文：** 该滤波器不翻转核，执行相关运算。

**概念来源：** [OpenCV 5.0.0 filter2D声明](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。

<a id="term-031"></a>
### TERM-031 · inference

**词性：** 名词。**领域：** 深度学习运行。**推荐译法：** 推理。

**概念：** 使用已有网络和输入执行前向计算以得到输出的过程。

**易误译与边界：** 不是训练或验证集评价；Net.forward是具体执行入口之一。

**自编例句：** Inference uses the trained model to produce outputs.

**对应译文：** 推理使用已训练模型产生输出。

**概念来源：** [OpenCV 5.0.0 DNN接口](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp)。**校核状态：** 英文概念已核对；中文为项目推荐译法。
