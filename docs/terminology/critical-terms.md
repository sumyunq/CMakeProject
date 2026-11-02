# Qt 与 OpenCV 关键术语辨析

本文件由 CSV 生成；请修改 CSV 后运行生成脚本。中文为项目推荐译法，例句为自编；来源和核对状态见各词条。

[速查手册](../qt-opencv-vocabulary.md) · [CSV 术语库](critical-terms.csv)

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
| TERM-032 | [histogram](#term-032) | 像素统计 | 直方图 |
| TERM-033 | [threshold](#term-033) | 图像分割 | 阈值 |
| TERM-034 | [mask](#term-034) | 图像处理 | 掩膜 |
| TERM-035 | [Gaussian blur](#term-035) | 滤波 | 高斯模糊 |
| TERM-036 | [erosion](#term-036) | 数学形态学 | 腐蚀 |
| TERM-037 | [dilation](#term-037) | 数学形态学 | 膨胀 |
| TERM-038 | [gradient](#term-038) | 图像导数 | 梯度 |
| TERM-039 | [contour](#term-039) | 形状分析 | 轮廓 |
| TERM-040 | [connected component](#term-040) | 二值图分析 | 连通分量 |
| TERM-041 | [interpolation](#term-041) | 几何变换 | 插值 |
| TERM-042 | [descriptor](#term-042) | 特征匹配 | 描述子 |
| TERM-043 | [homography](#term-043) | 图像几何 | 单应矩阵 |
| TERM-044 | [intrinsic parameters](#term-044) | 相机模型 | 相机内参 |
| TERM-045 | [distortion](#term-045) | 相机模型 | 畸变 |
| TERM-046 | [disparity](#term-046) | 双目视觉 | 视差 |
| TERM-047 | [optical flow](#term-047) | 运动估计 | 光流 |
| TERM-048 | [frame](#term-048) | 视频 | 帧 |
| TERM-049 | [tensor](#term-049) | 深度学习 | 张量 |
| TERM-050 | [non-maximum suppression](#term-050) | 目标检测 | 非极大值抑制 |
| TERM-051 | [latency](#term-051) | 性能测量 | 延迟 |
| TERM-052 | [pixel](#term-052) | 图像表示 | 像素 |
| TERM-053 | [region of interest](#term-053) | 图像处理 | 感兴趣区域 |
| TERM-054 | [matrix](#term-054) | 数组表示 | 矩阵 |
| TERM-055 | [element](#term-055) | 数组表示 | 元素 |
| TERM-056 | [buffer](#term-056) | 图像内存 | 缓冲区 |
| TERM-057 | [reference count](#term-057) | 内存管理 | 引用计数 |
| TERM-058 | [color space](#term-058) | 颜色表示 | 色彩空间 |
| TERM-059 | [binary image](#term-059) | 图像分割 | 二值图 |
| TERM-060 | [kernel](#term-060) | 图像滤波 | 核 |
| TERM-061 | [median filter](#term-061) | 图像滤波 | 中值滤波 |
| TERM-062 | [opening](#term-062) | 数学形态学 | 开运算 |
| TERM-063 | [closing](#term-063) | 数学形态学 | 闭运算 |
| TERM-064 | [bounding box](#term-064) | 形状分析 | 包围框 |
| TERM-065 | [keypoint](#term-065) | 局部特征 | 关键点 |
| TERM-066 | [matching](#term-066) | 局部特征 | 匹配 |
| TERM-067 | [tracking](#term-067) | 视频分析 | 跟踪 |
| TERM-068 | [frame rate](#term-068) | 视频 | 帧率 |
| TERM-069 | [batch](#term-069) | 深度学习 | 批次 |
| TERM-070 | [classification](#term-070) | 机器学习任务 | 分类 |
| TERM-071 | [segmentation](#term-071) | 计算机视觉任务 | 分割 |
| TERM-072 | [image](#term-072) | 图像与 Mat | 图像 |
| TERM-073 | [row](#term-073) | 图像与 Mat | 行 |
| TERM-074 | [column](#term-074) | 图像与 Mat | 列 |
| TERM-075 | [width](#term-075) | 图像与 Mat | 宽度 |
| TERM-076 | [height](#term-076) | 图像与 Mat | 高度 |
| TERM-077 | [coordinate](#term-077) | 图像与 Mat | 坐标 |
| TERM-078 | [index](#term-078) | 图像与 Mat | 索引 |
| TERM-079 | [type](#term-079) | 图像与 Mat | 类型 |
| TERM-080 | [unsigned](#term-080) | 图像与 Mat | 无符号的 |
| TERM-081 | [grayscale](#term-081) | 图像与 Mat | 灰度 |
| TERM-082 | [BGR](#term-082) | 图像与 Mat | 蓝绿红通道顺序 |
| TERM-083 | [RGB](#term-083) | 图像与 Mat | 红绿蓝通道顺序 |
| TERM-084 | [alpha](#term-084) | 图像与 Mat | 透明度分量 |
| TERM-085 | [range](#term-085) | 图像与 Mat | 区间 |
| TERM-086 | [rectangle](#term-086) | 图像与 Mat | 矩形 |
| TERM-087 | [dimensions](#term-087) | 图像与 Mat | 维数 |
| TERM-088 | [empty](#term-088) | 图像与 Mat | 空的 |
| TERM-089 | [header](#term-089) | 图像与 Mat | 矩阵头部 |
| TERM-090 | [ownership](#term-090) | 图像与 Mat | 所有权 |
| TERM-091 | [lifetime](#term-091) | 图像与 Mat | 生命周期 |
| TERM-092 | [alias](#term-092) | 图像与 Mat | 别名 |
| TERM-093 | [clone](#term-093) | 图像与 Mat | 克隆 |
| TERM-094 | [copy](#term-094) | 图像与 Mat | 复制 |
| TERM-095 | [allocation](#term-095) | 图像与 Mat | 分配 |
| TERM-096 | [dangling pointer](#term-096) | 图像与 Mat | 悬空指针 |
| TERM-097 | [detach](#term-097) | Qt Widgets | 分离 |
| TERM-098 | [widget](#term-098) | Qt Widgets | 控件 |
| TERM-099 | [window](#term-099) | Qt Widgets | 窗口 |
| TERM-100 | [dialog](#term-100) | Qt Widgets | 对话框 |
| TERM-101 | [layout](#term-101) | Qt Widgets | 布局 |
| TERM-102 | [geometry](#term-102) | Qt Widgets | 几何位置与尺寸 |
| TERM-103 | [margin](#term-103) | Qt Widgets | 边距 |
| TERM-104 | [spacing](#term-104) | Qt Widgets | 间距 |
| TERM-105 | [size hint](#term-105) | Qt Widgets | 建议尺寸 |
| TERM-106 | [parent](#term-106) | Qt Widgets | 父对象 |
| TERM-107 | [child](#term-107) | Qt Widgets | 子对象 |
| TERM-108 | [connect](#term-108) | Qt Widgets | 连接 |
| TERM-109 | [disconnect](#term-109) | Qt Widgets | 断开连接 |
| TERM-110 | [sender](#term-110) | Qt Widgets | 发送者 |
| TERM-111 | [receiver](#term-111) | Qt Widgets | 接收者 |
| TERM-112 | [event](#term-112) | Qt Widgets | 事件 |
| TERM-113 | [event loop](#term-113) | Qt Widgets | 事件循环 |
| TERM-114 | [property](#term-114) | Qt Widgets | 属性 |
| TERM-115 | [model](#term-115) | Qt Widgets | 模型 |
| TERM-116 | [view](#term-116) | Qt Widgets | 视图 |
| TERM-117 | [resource](#term-117) | Qt Widgets | 资源 |
| TERM-118 | [stylesheet](#term-118) | Qt Widgets | 样式表 |
| TERM-119 | [paint](#term-119) | Qt Widgets | 绘制 |
| TERM-120 | [render](#term-120) | Qt Widgets | 渲染 |
| TERM-121 | [source](#term-121) | 构建与部署 | 源文件 |
| TERM-122 | [header](#term-122) | 构建与部署 | 头文件 |
| TERM-123 | [dependency](#term-123) | 构建与部署 | 依赖项 |
| TERM-124 | [toolchain](#term-124) | 构建与部署 | 工具链 |
| TERM-125 | [configure](#term-125) | 构建与部署 | 配置 |
| TERM-126 | [generate](#term-126) | 构建与部署 | 生成 |
| TERM-127 | [build](#term-127) | 构建与部署 | 构建 |
| TERM-128 | [compile](#term-128) | 构建与部署 | 编译 |
| TERM-129 | [link](#term-129) | 构建与部署 | 链接 |
| TERM-130 | [target](#term-130) | 构建与部署 | 目标 |
| TERM-131 | [preset](#term-131) | 构建与部署 | 预设 |
| TERM-132 | [cache](#term-132) | 构建与部署 | 缓存 |
| TERM-133 | [Debug](#term-133) | 构建与部署 | 调试配置 |
| TERM-134 | [Release](#term-134) | 构建与部署 | 发布配置 |
| TERM-135 | [runtime](#term-135) | 构建与部署 | 运行时 |
| TERM-136 | [static](#term-136) | 构建与部署 | 静态库 |
| TERM-137 | [shared library](#term-137) | 构建与部署 | 共享库 |
| TERM-138 | [deployment](#term-138) | 构建与部署 | 部署 |
| TERM-139 | [symbol](#term-139) | 构建与部署 | 符号 |
| TERM-140 | [assertion](#term-140) | 构建与部署 | 断言 |
| TERM-141 | [warning](#term-141) | 构建与部署 | 警告 |
| TERM-142 | [error](#term-142) | 构建与部署 | 错误 |
| TERM-143 | [normalization](#term-143) | 视觉课程 | 归一化 |
| TERM-144 | [blur](#term-144) | 视觉课程 | 模糊 |
| TERM-145 | [edge](#term-145) | 视觉课程 | 边缘 |
| TERM-146 | [resize](#term-146) | 视觉课程 | 调整尺寸 |
| TERM-147 | [warp](#term-147) | 视觉课程 | 几何映射 |
| TERM-148 | [feature](#term-148) | 局部图像特征 | 特征 |
| TERM-149 | [extrinsic](#term-149) | 视觉课程 | 相机外参 |
| TERM-150 | [stereo](#term-150) | 视觉课程 | 双目视觉 |
| TERM-151 | [depth map](#term-151) | 视觉课程 | 深度图 |
| TERM-152 | [timestamp](#term-152) | 视觉课程 | 时间戳 |
| TERM-153 | [motion](#term-153) | 视频图像运动 | 运动 |
| TERM-154 | [shape](#term-154) | 视觉课程 | 形状 |
| TERM-155 | [preprocessing](#term-155) | 视觉课程 | 预处理 |
| TERM-156 | [postprocessing](#term-156) | 视觉课程 | 后处理 |
| TERM-157 | [detection](#term-157) | 视觉课程 | 目标检测 |
| TERM-158 | [inspection](#term-158) | 工业测量与性能 | 检验 |
| TERM-159 | [defect](#term-159) | 工业测量与性能 | 缺陷 |
| TERM-160 | [measurement](#term-160) | 工业测量与性能 | 测量 |
| TERM-161 | [tolerance](#term-161) | 尺寸计量 | 公差 |
| TERM-162 | [repeatability](#term-162) | 工业测量与性能 | 重复性 |
| TERM-163 | [uncertainty](#term-163) | 工业测量与性能 | 测量不确定度 |
| TERM-164 | [training](#term-164) | 工业测量与性能 | 训练 |
| TERM-165 | [confidence](#term-165) | 工业测量与性能 | 置信分数 |
| TERM-166 | [IoU](#term-166) | 工业测量与性能 | 交并比 |
| TERM-167 | [throughput](#term-167) | 工业测量与性能 | 吞吐量 |
| TERM-168 | [benchmark](#term-168) | 工业测量与性能 | 基准测试 |
| TERM-169 | [profiling](#term-169) | 工业测量与性能 | 性能剖析 |
| TERM-170 | [asynchronous](#term-170) | 工业测量与性能 | 异步的 |
| TERM-171 | [synchronization](#term-171) | 工业测量与性能 | 同步协调 |
| TERM-172 | [race condition](#term-172) | 工业测量与性能 | 竞态条件 |
| TERM-173 | [layout](#term-173) | 深度学习张量 | 维度布局 |
| TERM-174 | [watershed](#term-174) | 课程算法 | 分水岭 |
| TERM-175 | [Canny](#term-175) | 课程算法 | Canny 边缘检测 |
| TERM-176 | [Sobel](#term-176) | 课程算法 | Sobel 算子 |
| TERM-177 | [Hough transform](#term-177) | 课程算法 | 霍夫变换 |
| TERM-178 | [RANSAC](#term-178) | 课程算法 | 随机抽样一致性 |
| TERM-179 | [ORB](#term-179) | 课程算法 | ORB 特征 |
| TERM-180 | [SIFT](#term-180) | 课程算法 | 尺度不变特征变换 |
| TERM-181 | [subpixel](#term-181) | 课程算法 | 亚像素 |
| TERM-182 | [reprojection error](#term-182) | 课程算法 | 重投影误差 |
| TERM-183 | [PnP](#term-183) | 课程算法 | 透视 n 点位姿估计 |
| TERM-184 | [Otsu threshold](#term-184) | 课程算法与数据 | 大津阈值 |
| TERM-185 | [adaptive threshold](#term-185) | 课程算法与数据 | 自适应阈值 |
| TERM-186 | [structuring element](#term-186) | 课程算法与数据 | 结构元素 |
| TERM-187 | [distance transform](#term-187) | 课程算法与数据 | 距离变换 |
| TERM-188 | [inverse mapping](#term-188) | 课程算法与数据 | 逆映射 |
| TERM-189 | [Hamming distance](#term-189) | 课程算法与数据 | 汉明距离 |
| TERM-190 | [inlier](#term-190) | 课程算法与数据 | 内点 |
| TERM-191 | [outlier](#term-191) | 课程算法与数据 | 外点 |
| TERM-192 | [Kalman filter](#term-192) | 课程算法与数据 | 卡尔曼滤波器 |
| TERM-193 | [NCHW](#term-193) | 课程算法与数据 | 批通道高宽布局 |
| TERM-194 | [ReLU](#term-194) | 课程算法与数据 | 修正线性单元 |
| TERM-195 | [residual](#term-195) | 课程算法与数据 | 残差 |
| TERM-196 | [backpressure](#term-196) | 有界并发 | 背压 |
| TERM-197 | [saturation](#term-197) | 课程概念 | 饱和裁剪 |
| TERM-198 | [histogram bin](#term-198) | 课程概念 | 直方图分箱 |
| TERM-199 | [border extrapolation](#term-199) | 课程概念 | 边界外推 |
| TERM-200 | [contour hierarchy](#term-200) | 课程概念 | 轮廓层级 |
| TERM-201 | [watershed marker](#term-201) | 课程概念 | 分水岭标记 |
| TERM-202 | [affine transform](#term-202) | 课程概念 | 仿射变换 |
| TERM-203 | [non-maximum suppression](#term-203) | 边缘检测 | 非极大值抑制 |
| TERM-204 | [hysteresis thresholding](#term-204) | 边缘检测 | 滞后阈值连接 |
| TERM-205 | [false positive](#term-205) | 分类评价 | 假阳性 |
| TERM-206 | [false negative](#term-206) | 分类评价 | 假阴性 |
| TERM-207 | [cumulative distribution function](#term-207) | 图像统计 | 累积分布函数 |
| TERM-208 | [dynamic range](#term-208) | 图像表示 | 动态范围 |
| TERM-209 | [foreground](#term-209) | 图像分割 | 前景 |
| TERM-210 | [oversegmentation](#term-210) | 图像分割 | 过分割 |
| TERM-211 | [normalized cross-correlation](#term-211) | 模板匹配 | 归一化互相关 |
| TERM-212 | [ambiguity](#term-212) | 特征匹配 | 歧义 |
| TERM-213 | [seam](#term-213) | 图像拼接 | 接缝 |
| TERM-214 | [contract](#term-214) | API与模型接口 | 契约 |
| TERM-215 | [degeneracy](#term-215) | 几何估计 | 退化 |
| TERM-216 | [flat-field correction](#term-216) | 工业成像 | 平场校正 |
| TERM-217 | [exposure time](#term-217) | 工业成像 | 曝光时间 |
| TERM-218 | [validation set](#term-218) | 机器学习评价 | 验证集 |
| TERM-219 | [test set](#term-219) | 机器学习评价 | 测试集 |

<a id="term-001"></a>
## TERM-001 · depth

- **词性**：名词
- **领域**：Mat元素类型
- **推荐译法**：元素深度
- **定义**：每个通道分量的数据类型类别，如CV_8U、CV_32F；既涉及位数，也涉及数值表示。
- **语境**：第2课：检查 Mat 类型
- **易混义或误译**：不要直接译为每像素位数；CV_8UC3不是每通道24位。
- **英文例句**：The image depth is CV_8U, and the image has three channels.
- **中文例句**：图像的元素深度为8位无符号整数，共3个通道。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-002"></a>
## TERM-002 · depth

- **词性**：名词
- **领域**：相机与三维
- **推荐译法**：深度
- **定义**：依照坐标约定表示的空间深度，例如针孔模型中相机坐标Z；应注明单位。
- **语境**：第15课：解释相机坐标与深度
- **易混义或误译**：不要与Mat的depth()或相机到目标的欧氏距离自动等同。
- **英文例句**：Depth is expressed in millimetres in this experiment.
- **中文例句**：本实验以毫米表示深度。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0几何与位姿](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)

<a id="term-003"></a>
## TERM-003 · stride

- **词性**：名词
- **领域**：图像内存
- **推荐译法**：行步长
- **定义**：二维Mat相邻两行起始地址的字节间隔，对应step[0]。
- **语境**：第2课：检查 ROI 的 step
- **易混义或误译**：不是宽度，也不总等于width乘每像素字节数。
- **英文例句**：The ROI keeps the parent image's row stride.
- **中文例句**：该ROI保留父图的行步长。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-004"></a>
## TERM-004 · stride

- **词性**：名词
- **领域**：卷积算子
- **推荐译法**：步幅
- **定义**：卷积窗口沿空间轴移动的采样间隔，可按各轴分别指定。
- **语境**：第22课：读取卷积模型输入契约
- **易混义或误译**：单位是空间位置步数，不是Mat行地址间的字节数。
- **英文例句**：A stride of two reduces the number of sampled positions.
- **中文例句**：步幅为2会减少被采样的位置数量。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[ONNX Conv算子定义，仅作算子概念参考](https://onnx.ai/onnx/operators/onnx__Conv.html)

<a id="term-005"></a>
## TERM-005 · padding

- **词性**：名词
- **领域**：图像内存
- **推荐译法**：填充字节
- **定义**：行存储中可能为对齐等原因保留、不属于有效像素的字节。
- **语境**：第2课：检查图像行间存储
- **易混义或误译**：不要把它当作额外图像内容；并非每张图都含行末填充。
- **英文例句**：Padding bytes are not additional pixels.
- **中文例句**：填充字节不是额外的像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-006"></a>
## TERM-006 · padding

- **词性**：名词
- **领域**：卷积算子
- **推荐译法**：填充
- **定义**：按算子约定在输入边界外补充位置或数值，以影响边界计算和输出大小。
- **语境**：第22课：检查卷积输出尺寸
- **易混义或误译**：不是行末字节对齐；same padding还要核对步幅和取整规则。
- **英文例句**：The convolution uses zero padding at the border.
- **中文例句**：该卷积在边界使用零填充。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[ONNX Conv算子定义，仅作算子概念参考](https://onnx.ai/onnx/operators/onnx__Conv.html)

<a id="term-007"></a>
## TERM-007 · channel

- **词性**：名词
- **领域**：图像表示
- **推荐译法**：通道
- **定义**：一个元素的分量，例如BGR像素含3个通道。
- **语境**：第3课：检查 BGR 图像
- **易混义或误译**：通道数不等于Mat的dims；三通道二维图像仍可dims==2。
- **英文例句**：This matrix has three channels but only two dimensions.
- **中文例句**：该矩阵有3个通道，但只有2个维度。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-008"></a>
## TERM-008 · pixel format

- **词性**：名词短语
- **领域**：Qt图像存储
- **推荐译法**：像素格式
- **定义**：QImage等接口约定的像素数值表示、分量组织及透明度相关规则。
- **语境**：第2课：转换 QImage
- **易混义或误译**：不要与色彩空间或文件格式PNG混用。
- **英文例句**：Check the pixel format before interpreting the buffer.
- **中文例句**：解释缓冲区之前，先检查像素格式。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8 QImage像素格式](https://doc.qt.io/qt-6.8/qimage.html)

<a id="term-009"></a>
## TERM-009 · continuous

- **词性**：形容词
- **领域**：Mat存储布局
- **推荐译法**：连续存储的
- **定义**：在isContinuous()语境中，表示元素间无需跨越额外行间隙的布局特性。
- **语境**：第2课：检查 ROI 连续性
- **易混义或误译**：不是数学函数连续性；普通英文contiguous也常用来描述相邻存储。
- **英文例句**：This ROI is not continuous, so process it row by row.
- **中文例句**：该ROI不是连续存储的，应逐行处理。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-010"></a>
## TERM-010 · shallow copy

- **词性**：名词短语
- **领域**：Mat所有权
- **推荐译法**：浅拷贝
- **定义**：复制矩阵描述信息，并共享像素存储的复制行为。
- **语境**：第2课：复制 Mat 头部
- **易混义或误译**：不是自动产生独立像素快照；修改共享像素可互相可见。
- **英文例句**：A shallow copy shares the pixel buffer with the original.
- **中文例句**：浅拷贝与原对象共享像素缓冲区。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-011"></a>
## TERM-011 · deep copy

- **词性**：名词短语
- **领域**：Mat所有权
- **推荐译法**：深拷贝
- **定义**：把像素复制到独立存储；clone()是本项目常用方式。
- **语境**：第2课：使用 clone()
- **易混义或误译**：不能只凭copy这个单词判断深浅，必须看具体接口。
- **英文例句**：Use a deep copy when independent pixel storage is required.
- **中文例句**：需要独立像素存储时，使用深拷贝。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 Mat声明与内存约定](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp)

<a id="term-012"></a>
## TERM-012 · implicit sharing

- **词性**：名词短语
- **领域**：Qt值类型
- **推荐译法**：隐式共享
- **定义**：Qt中部分值类型共享内部数据，并在需要时进行数据分离的机制。
- **语境**：第2课：检查 QImage 副本
- **易混义或误译**：不能据此推断Mat写入会自动分离，也不能保证任意外部缓冲区有效。
- **英文例句**：Implicit sharing can postpone copying the data.
- **中文例句**：隐式共享可以推迟数据复制。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)

<a id="term-013"></a>
## TERM-013 · copy-on-write

- **词性**：名词短语；作定语时连字符连接
- **领域**：Qt共享数据
- **推荐译法**：写时复制
- **定义**：共享对象需要修改数据时，按具体类型的规则先分离数据。
- **语境**：第2课：修改共享 Qt 值
- **易混义或误译**：不要把它称为线程同步机制；绕过类型接口访问外部指针另有约束。
- **英文例句**：Copy-on-write does not replace synchronization.
- **中文例句**：写时复制不能代替同步。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8隐式共享](https://doc.qt.io/qt-6.8/implicit-sharing.html)

<a id="term-014"></a>
## TERM-014 · thread affinity

- **词性**：名词短语
- **领域**：QObject线程模型
- **推荐译法**：线程亲和性
- **定义**：QObject所属线程的关系；学习时可解释为线程归属。
- **语境**：第25课：安排 QObject 工作线程
- **易混义或误译**：不是CPU核心绑定；普通成员函数调用不会自动跳到对象所属线程。
- **英文例句**：The receiver has affinity to the GUI thread.
- **中文例句**：接收对象的线程亲和性指向GUI线程，即它属于GUI线程。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8 QObject线程亲和性](https://doc.qt.io/qt-6.8/qobject.html#thread-affinity)

<a id="term-015"></a>
## TERM-015 · signal

- **词性**：名词
- **领域**：Qt对象通信
- **推荐译法**：信号
- **定义**：用于通知状态变化或事件发生的对象通信接口。
- **语境**：第25课：传递任务完成通知
- **易混义或误译**：不是异步任务的同义词；执行方式取决于连接类型。
- **英文例句**：The object emits a signal when its state changes.
- **中文例句**：对象状态变化时会发射信号。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)

<a id="term-016"></a>
## TERM-016 · slot

- **词性**：名词
- **领域**：Qt对象通信
- **推荐译法**：槽；槽函数
- **定义**：可接收信号调用的函数，同时仍可按普通C++规则直接调用。
- **语境**：第25课：接收任务通知
- **易混义或误译**：在Qt语境不译为硬件插槽；也不意味着自动在子线程运行。
- **英文例句**：The slot updates the displayed value.
- **中文例句**：该槽函数更新显示值。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)

<a id="term-017"></a>
## TERM-017 · emit

- **词性**：动词；Qt中也是宏标记
- **领域**：Qt信号用语
- **推荐译法**：发射
- **定义**：常见搭配emit a signal，表示发出信号通知。
- **语境**：第25课：发出完成信号
- **易混义或误译**：不是创建线程、投递任意任务或保证异步执行。
- **英文例句**：Emit the signal after updating the model.
- **中文例句**：更新模型之后再发射信号。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)

<a id="term-018"></a>
## TERM-018 · queued connection

- **词性**：名词短语
- **领域**：Qt对象通信
- **推荐译法**：队列连接
- **定义**：通过事件循环安排槽在接收对象所属线程执行的连接方式。
- **语境**：第25课：跨线程传递结果
- **易混义或误译**：不要译成线程池；接收方需要能处理相应事件。
- **英文例句**：A queued connection delivers the call through the event loop.
- **中文例句**：队列连接通过事件循环递送调用。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8 QObject线程亲和性](https://doc.qt.io/qt-6.8/qobject.html#thread-affinity)

<a id="term-019"></a>
## TERM-019 · registration

- **词性**：名词
- **领域**：图像几何
- **推荐译法**：配准
- **定义**：估计并应用图像之间的空间对应关系或变换。
- **语境**：第13课：图像配准
- **易混义或误译**：此语境不是软件注册，也不是简单地把尺寸改成相同。
- **英文例句**：Registration aligns the sample image with the reference image.
- **中文例句**：配准使样本图像与参考图像在空间上对齐。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0仿射和几何配准](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)

<a id="term-020"></a>
## TERM-020 · registration

- **词性**：名词
- **领域**：Qt元类型系统
- **推荐译法**：注册
- **定义**：把类型等信息登记到相应系统，以供后续查询或调用。
- **语境**：第25课：跨线程传递自定义类型
- **易混义或误译**：此语境不译为图像配准；常见搭配type registration。
- **英文例句**：Type registration and image registration are different concepts.
- **中文例句**：类型注册与图像配准是不同概念。
- **版本**：Qt 6.8
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[Qt 6.8 QMetaType注册](https://doc.qt.io/qt-6.8/qmetatype.html)

<a id="term-021"></a>
## TERM-021 · pose

- **词性**：名词
- **领域**：相机与机器人几何
- **推荐译法**：位姿
- **定义**：通常包含位置和朝向；在PnP中必须说明坐标变换方向和尺度单位。
- **语境**：第15课：恢复相机位姿
- **易混义或误译**：不要只译为姿态而遗漏平移；orientation通常强调朝向。
- **英文例句**：The estimated pose contains rotation and translation.
- **中文例句**：估计的位姿包含旋转和平移。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0几何与位姿](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp)

<a id="term-022"></a>
## TERM-022 · calibration

- **词性**：名词
- **领域**：计算机视觉相机模型
- **推荐译法**：标定
- **定义**：根据观测和模型估计相机内参、畸变等参数的过程。
- **语境**：第14课：估计相机参数
- **易混义或误译**：不是把图像调好看；也不与每帧PnP位姿估计等同。
- **英文例句**：Camera calibration estimates the intrinsic parameters.
- **中文例句**：相机标定估计内参。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0相机标定](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp)

<a id="term-023"></a>
## TERM-023 · calibration

- **词性**：名词
- **领域**：计量学
- **推荐译法**：校准
- **定义**：在规定条件下建立标准量值、仪器示值与测量结果之间关系的操作。
- **语境**：第19课：测量系统校准
- **易混义或误译**：不要与adjustment调整或verification核查混同。
- **英文例句**：Calibration is not the same operation as adjustment.
- **中文例句**：校准与调整不是同一种操作。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[VIM3 2.39 calibration](https://jcgm.bipm.org/vim/en/2.39.html)

<a id="term-024"></a>
## TERM-024 · accuracy

- **词性**：名词
- **领域**：计量学
- **推荐译法**：测量准确度
- **定义**：测得量值与被测量真值之间接近程度的概念。
- **语境**：第19课：评价测量结果
- **易混义或误译**：VIM不把准确度本身定义为可直接给数值的量；应另报告误差或不确定度。
- **英文例句**：Accuracy and precision describe different aspects of measurement.
- **中文例句**：准确度与精密度描述测量的不同方面。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[VIM3 2.13 measurement accuracy](https://jcgm.bipm.org/vim/en/2.13.html)

<a id="term-025"></a>
## TERM-025 · precision

- **词性**：名词
- **领域**：计量学
- **推荐译法**：测量精密度
- **定义**：规定条件下重复测量所得示值或测得量值之间的一致程度。
- **语境**：第19课：评价重复测量
- **易混义或误译**：不要笼统写成精度，更不等于测值接近真值。
- **英文例句**：Repeated measurements may be precise but biased.
- **中文例句**：重复测量结果可能很集中，但仍有偏差。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[VIM3 2.15 measurement precision](https://jcgm.bipm.org/vim/en/2.15.html)

<a id="term-026"></a>
## TERM-026 · precision

- **词性**：名词
- **领域**：分类与检测指标
- **推荐译法**：精确率
- **定义**：预测为正的样本中真正为正的比例，TP/(TP+FP)。
- **语境**：第23课：评价目标检测
- **易混义或误译**：不是计量学精密度，也不是整体分类准确率；分母为零要声明处理策略。
- **英文例句**：Precision decreases when false positives increase, with true positives fixed.
- **中文例句**：真正例数量不变时，假正例增加会使精确率下降。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)

<a id="term-027"></a>
## TERM-027 · recall

- **词性**：名词
- **领域**：分类与检测指标
- **推荐译法**：召回率
- **定义**：实际正样本中被正确检出的比例，TP/(TP+FN)。
- **语境**：第23课：评价目标检测
- **易混义或误译**：不是日常英语的回忆；不要和precision交换分母。
- **英文例句**：Recall measures how many actual positives are retrieved.
- **中文例句**：召回率衡量实际正样本中有多少被检出。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)

<a id="term-028"></a>
## TERM-028 · accuracy

- **词性**：名词
- **领域**：分类指标
- **推荐译法**：准确率
- **定义**：分类正确的样本数占总样本数的比例；简单二分类可写(TP+TN)/总数。
- **语境**：第21课：评价分类结果
- **易混义或误译**：不是precision；类别不平衡时，单独报告它可能掩盖漏检。
- **英文例句**：High accuracy can hide poor recall on rare defects.
- **中文例句**：较高的准确率可能掩盖稀有缺陷召回率很差的问题。
- **版本**：见来源链接；版本未注明
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[scikit-learn分类指标定义，仅作通用指标参考](https://scikit-learn.org/stable/modules/model_evaluation.html#precision-recall-f-measure-metrics)

<a id="term-029"></a>
## TERM-029 · convolution

- **词性**：名词
- **领域**：离散信号处理
- **推荐译法**：卷积
- **定义**：按卷积定义对核作相应翻转后计算的滑动加权运算。
- **语境**：第6课：讨论滤波核翻转
- **易混义或误译**：OpenCV filter2D直接计算的是相关；对称核会掩盖区别。
- **英文例句**：For an asymmetric kernel, convolution and correlation can differ.
- **中文例句**：对于非对称核，卷积与相关的结果可能不同。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 filter2D声明](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp)

<a id="term-030"></a>
## TERM-030 · correlation

- **词性**：名词
- **领域**：图像滤波
- **推荐译法**：相关；相关运算
- **定义**：在filter2D语境中，按未翻转的核进行滑动加权。
- **语境**：第6课：理解 filter2D
- **易混义或误译**：不要机械译为相关系数；这里不是统计意义的Pearson系数。
- **英文例句**：The filter applies correlation without flipping the kernel.
- **中文例句**：该滤波器不翻转核，执行相关运算。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 filter2D声明](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp)

<a id="term-031"></a>
## TERM-031 · inference

- **词性**：名词
- **领域**：深度学习运行
- **推荐译法**：推理
- **定义**：使用已有网络和输入执行前向计算以得到输出的过程。
- **语境**：第22课：执行模型
- **易混义或误译**：不是训练或验证集评价；Net.forward是具体执行入口之一。
- **英文例句**：Inference uses the trained model to produce outputs.
- **中文例句**：推理使用已训练模型产生输出。
- **版本**：OpenCV 5.0.0
- **校核状态**：英文概念已核对；中文为项目推荐译法，非官方中译认证
- **核对日期**：2026-10-26
- **来源**：[OpenCV 5.0.0 DNN接口](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp)

<a id="term-032"></a>
## TERM-032 · histogram

- **词性**：名词
- **领域**：像素统计
- **推荐译法**：直方图
- **定义**：像素值落入各区间的计数分布。
- **语境**：第4课：比较灰度分布
- **易混义或误译**：直方图不是原图的空间排列。
- **英文例句**：The histogram counts pixels in each intensity bin.
- **中文例句**：直方图统计每个强度区间的像素数。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的calcHist 的分箱及范围；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：calcHist 的分箱及范围](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2973)

<a id="term-033"></a>
## TERM-033 · threshold

- **词性**：名词
- **领域**：图像分割
- **推荐译法**：阈值
- **定义**：用给定界限将像素映射为不同输出值的操作或界限。
- **语境**：第5课：threshold 生成二值图
- **易混义或误译**：阈值不保证分割结果正确。
- **英文例句**：Threshold the grayscale image before finding components.
- **中文例句**：查找连通域前先对灰度图做阈值处理。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的threshold 的固定阈值与输出；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：threshold 的固定阈值与输出](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2822)

<a id="term-034"></a>
## TERM-034 · mask

- **词性**：名词
- **领域**：图像处理
- **推荐译法**：掩膜
- **定义**：指定操作所涉及位置的数组。
- **语境**：第5课：选择有效像素
- **易混义或误译**：掩膜不必是原图的副本。
- **英文例句**：The mask selects pixels for the calculation.
- **中文例句**：掩膜选择参与计算的像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的calcHist 的非零掩膜；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：calcHist 的非零掩膜](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core.hpp#L2986)

<a id="term-035"></a>
## TERM-035 · Gaussian blur

- **词性**：名词
- **领域**：滤波
- **推荐译法**：高斯模糊
- **定义**：使用高斯核对图像进行平滑。
- **语境**：第6课：GaussianBlur 降噪
- **易混义或误译**：平滑不能恢复已丢失细节。
- **英文例句**：GaussianBlur smooths the image before edge detection.
- **中文例句**：边缘检测前用 GaussianBlur 平滑图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的GaussianBlur 的高斯核；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：GaussianBlur 的高斯核](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1255)

<a id="term-036"></a>
## TERM-036 · erosion

- **词性**：名词
- **领域**：数学形态学
- **推荐译法**：腐蚀
- **定义**：基于结构元素的局部最小值操作，常使前景区域收缩。
- **语境**：第7课：处理二值图
- **易混义或误译**：收缩效果取决于前景表示和结构元素。
- **英文例句**：Erosion can remove small foreground protrusions.
- **中文例句**：腐蚀可以去除小的前景凸起。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的erode 的局部最小值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：erode 的局部最小值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1968)

<a id="term-037"></a>
## TERM-037 · dilation

- **词性**：名词
- **领域**：数学形态学
- **推荐译法**：膨胀
- **定义**：基于结构元素的局部最大值操作，常使前景区域扩张。
- **语境**：第7课：连接前景区域
- **易混义或误译**：膨胀不等同于图像缩放。
- **英文例句**：Dilation expands foreground regions in the binary image.
- **中文例句**：膨胀使二值图中的前景区域扩张。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的dilate 的局部最大值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：dilate 的局部最大值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2001)

<a id="term-038"></a>
## TERM-038 · gradient

- **词性**：名词
- **领域**：图像导数
- **推荐译法**：梯度
- **定义**：描述图像强度空间变化方向和变化率的向量。
- **语境**：第8课：计算 Sobel 导数
- **易混义或误译**：梯度与轮廓不是同一对象。
- **英文例句**：The gradient magnitude highlights strong intensity changes.
- **中文例句**：梯度幅值突出强烈的强度变化。
- **版本**：OpenCV 4.13 官方概念资料；工程固定 OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 梯度方向及幅值公式；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 Canny 梯度方向与幅值](https://docs.opencv.org/4.13.0/da/d22/tutorial_py_canny.html)

<a id="term-039"></a>
## TERM-039 · contour

- **词性**：名词
- **领域**：形状分析
- **推荐译法**：轮廓
- **定义**：表示边界的一组点或曲线。
- **语境**：第9课：findContours 输出
- **易混义或误译**：轮廓不是填充的连通区域。
- **英文例句**：findContours returns boundary points for each contour.
- **中文例句**：findContours 返回每条轮廓的边界点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的findContours 的轮廓输出；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：findContours 的轮廓输出](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3747)

<a id="term-040"></a>
## TERM-040 · connected component

- **词性**：名词
- **领域**：二值图分析
- **推荐译法**：连通分量
- **定义**：按照指定邻接关系彼此连通的一组前景像素。
- **语境**：第9课：标记二值区域
- **易混义或误译**：四连通与八连通可能产生不同结果。
- **英文例句**：connectedComponents assigns labels to connected foreground pixels.
- **中文例句**：connectedComponents 为连通的前景像素分配标签。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的connectedComponents 的四或八连通；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：connectedComponents 的四或八连通](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3673)

<a id="term-041"></a>
## TERM-041 · interpolation

- **词性**：名词
- **领域**：几何变换
- **推荐译法**：插值
- **定义**：从邻近样本估计非整数位置数值的方法。
- **语境**：第11课：resize 与 warp
- **易混义或误译**：插值不会凭空还原真实细节。
- **英文例句**：Linear interpolation estimates values between source pixels.
- **中文例句**：线性插值估计源像素之间的数值。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的双线性插值选项；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：双线性插值选项](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L248)

<a id="term-042"></a>
## TERM-042 · descriptor

- **词性**：名词
- **领域**：特征匹配
- **推荐译法**：描述子
- **定义**：用于描述局部特征并比较其相似性的数值表示。
- **语境**：第12课：ORB 描述子匹配
- **易混义或误译**：描述子不是关键点坐标。
- **英文例句**：ORB computes descriptors for the detected keypoints.
- **中文例句**：ORB 为检测到的关键点计算描述子。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的ORB 的关键点和描述子；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：ORB 的关键点和描述子](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L437)

<a id="term-043"></a>
## TERM-043 · homography

- **词性**：名词
- **领域**：图像几何
- **推荐译法**：单应矩阵
- **定义**：以齐次坐标表示平面之间投影映射的矩阵。
- **语境**：第13课：估计透视映射
- **易混义或误译**：一般三维场景存在视差时单一单应矩阵不一定适用。
- **英文例句**：A homography maps points between the two image planes.
- **中文例句**：单应矩阵映射两个图像平面之间的点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的findHomography 的平面映射公式；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：findHomography 的平面映射公式](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L734)

<a id="term-044"></a>
## TERM-044 · intrinsic parameters

- **词性**：名词
- **领域**：相机模型
- **推荐译法**：相机内参
- **定义**：描述相机投影的内部参数，如焦距和主点。
- **语境**：第14课：查看相机矩阵
- **易混义或误译**：内参不表示相机在世界中的位姿。
- **英文例句**：The camera matrix stores focal lengths and the principal point.
- **中文例句**：相机矩阵存储焦距和主点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的calibrateCamera 的内参与畸变系数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：calibrateCamera 的内参与畸变系数](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp#L580)

<a id="term-045"></a>
## TERM-045 · distortion

- **词性**：名词
- **领域**：相机模型
- **推荐译法**：畸变
- **定义**：镜头成像相对于理想投影模型的偏离。
- **语境**：第14课：使用畸变系数
- **易混义或误译**：畸变系数不等于内参矩阵。
- **英文例句**：The distortion coefficients are used to undistort image points.
- **中文例句**：畸变系数用于校正图像点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的undistortPoints 的点与畸变系数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：undistortPoints 的点与畸变系数](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L2306)

<a id="term-046"></a>
## TERM-046 · disparity

- **词性**：名词
- **领域**：双目视觉
- **推荐译法**：视差
- **定义**：对应点在双目图像中的位置差，通常指校正后水平坐标差。
- **语境**：第15课：计算深度
- **易混义或误译**：视差本身不是深度；单位及符号依约定。
- **英文例句**：Disparity is the horizontal difference between rectified corresponding points.
- **中文例句**：视差是校正后对应点的水平位置差。
- **版本**：OpenCV 4.13 官方概念资料；工程固定 OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 4.13 双目视差定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 双目视差定义](https://docs.opencv.org/4.13.0/dd/d53/tutorial_py_depthmap.html)

<a id="term-047"></a>
## TERM-047 · optical flow

- **词性**：名词
- **领域**：运动估计
- **推荐译法**：光流
- **定义**：图像中亮度模式的表观运动场或其估计。
- **语境**：第17课：跟踪视频特征点
- **易混义或误译**：光流不一定等于物体的三维运动。
- **英文例句**：Lucas–Kanade estimates optical flow for selected points.
- **中文例句**：Lucas–Kanade 为选定点估计光流。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的calcOpticalFlowPyrLK 的点与状态；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：calcOpticalFlowPyrLK 的点与状态](https://github.com/opencv/opencv/blob/5.0.0/modules/video/include/opencv2/video/tracking.hpp#L139)

<a id="term-048"></a>
## TERM-048 · frame

- **词性**：名词
- **领域**：视频
- **推荐译法**：帧
- **定义**：视频序列中的一幅图像。
- **语境**：第16课：读取视频
- **易混义或误译**：帧与时间戳不等同。
- **英文例句**：VideoCapture reads the next frame into a Mat.
- **中文例句**：VideoCapture 将下一帧读入 Mat。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的VideoCapture::read 的帧与失败结果；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：VideoCapture::read 的帧与失败结果](https://github.com/opencv/opencv/blob/5.0.0/modules/videoio/include/opencv2/videoio.hpp#L978)

<a id="term-049"></a>
## TERM-049 · tensor

- **词性**：名词
- **领域**：深度学习
- **推荐译法**：张量
- **定义**：具有形状和元素类型的多维数据容器。
- **语境**：第22课：检查模型输入
- **易混义或误译**：张量维度顺序须按模型契约确认。
- **英文例句**：The input tensor has a batch dimension.
- **中文例句**：输入张量具有批次维度。
- **版本**：PyTorch 2.15 文档；工程依赖不变
- **校核状态**：已核对 PyTorch 2.15 Tensor 多维数组和数据类型；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[PyTorch 2.15 Tensor 多维数组和数据类型](https://docs.pytorch.org/docs/2.15/tensors.html)

<a id="term-050"></a>
## TERM-050 · non-maximum suppression

- **词性**：名词
- **领域**：目标检测
- **推荐译法**：非极大值抑制
- **定义**：按得分和重叠关系筛除冗余候选框的过程。
- **语境**：第23课：筛选检测结果
- **易混义或误译**：NMS 不会修正候选框坐标。
- **英文例句**：NMS removes overlapping lower-scoring detection boxes.
- **中文例句**：NMS 去除重叠且得分较低的检测框。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的NMSBoxes 的候选框和分数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：NMSBoxes 的候选框和分数](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1438)

<a id="term-051"></a>
## TERM-051 · latency

- **词性**：名词
- **领域**：性能测量
- **推荐译法**：延迟
- **定义**：单个任务从指定起点到终点经过的时间。
- **语境**：第24课：计时推理
- **易混义或误译**：延迟与每秒处理量不是同一指标。
- **英文例句**：Measure latency from frame capture to displayed result.
- **中文例句**：测量从采集帧到显示结果的延迟。
- **版本**：Intel 2017 文章
- **校核状态**：已核对 Intel 延迟与吞吐量；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Intel 延迟与吞吐量](https://www.intel.com/content/www/us/en/developer/articles/technical/optimizing-computer-applications-for-latency-part-1-configuring-the-hardware.html)

<a id="term-052"></a>
## TERM-052 · pixel

- **词性**：名词
- **领域**：图像表示
- **推荐译法**：像素
- **定义**：图像采样网格中的一个位置，其数值可以含多个通道。
- **语境**：第3课：读取像素
- **易混义或误译**：一个像素不一定只有一个数值。
- **英文例句**：A BGR pixel has three channel values.
- **中文例句**：BGR 像素有三个通道值。
- **版本**：NIST OOF 手册，在线版本
- **校核状态**：已核对 NIST 图像像素定义；中文为项目推荐译法，通道例句依本课 BGR 约定
- **核对日期**：2026-10-30
- **来源**：[NIST OOF 图像像素术语](https://www.ctcms.nist.gov/oof/oof1/MAN_PPM2OOF/ch01s06.html)

<a id="term-053"></a>
## TERM-053 · region of interest

- **词性**：名词
- **领域**：图像处理
- **推荐译法**：感兴趣区域
- **定义**：从图像中选出的待处理区域。
- **语境**：第2课：通过 Rect 构造 ROI
- **易混义或误译**：Mat ROI 通常共享原始像素而非自动复制。
- **英文例句**：A Mat ROI created from the original shares its pixels.
- **中文例句**：从原 Mat 创建的 ROI 与它共享像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 子矩阵共享和 clone；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 子矩阵共享和 clone](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L819)

<a id="term-054"></a>
## TERM-054 · matrix

- **词性**：名词
- **领域**：数组表示
- **推荐译法**：矩阵
- **定义**：按维度组织元素的数据结构；Mat 可表示图像或数值数组。
- **语境**：第2课：检查 Mat 的行列
- **易混义或误译**：图像的颜色语义不能从矩阵类型单独推断。
- **英文例句**：The matrix contains rows and columns.
- **中文例句**：矩阵包含行和列。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的多维数组及行列；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的多维数组及行列](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L772)

<a id="term-055"></a>
## TERM-055 · element

- **词性**：名词
- **领域**：数组表示
- **推荐译法**：元素
- **定义**：数组中的一个位置所存的值，可以包含多个通道。
- **语境**：第2课：查询 elemSize
- **易混义或误译**：完整元素字节数包括所有通道。
- **英文例句**：elemSize returns the size of one complete element in bytes.
- **中文例句**：elemSize 返回一个完整元素的字节数。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的 elemSize 和通道；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的 elemSize 和通道](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1096)

<a id="term-056"></a>
## TERM-056 · buffer

- **词性**：名词
- **领域**：图像内存
- **推荐译法**：缓冲区
- **定义**：用于保存数据的一段存储空间。
- **语境**：第2课：从外部数据构造 Mat
- **易混义或误译**：外部缓冲区的所有权不一定由 Mat 接管。
- **英文例句**：The external buffer must outlive the Mat view.
- **中文例句**：外部缓冲区的有效期必须覆盖 Mat 视图。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的外部数据的释放责任；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：外部数据的释放责任](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1096)

<a id="term-057"></a>
## TERM-057 · reference count

- **词性**：名词
- **领域**：内存管理
- **推荐译法**：引用计数
- **定义**：记录共享资源引用情况的计数机制。
- **语境**：第2课：复制 Mat 头部
- **易混义或误译**：外部数据的生命周期不一定受 Mat 引用计数管理。
- **英文例句**：Mat headers can share reference-counted pixel storage.
- **中文例句**：多个 Mat 头部可以共享引用计数管理的像素存储。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 复制头与引用计数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 复制头与引用计数](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L819)

<a id="term-058"></a>
## TERM-058 · color space

- **词性**：名词
- **领域**：颜色表示
- **推荐译法**：色彩空间
- **定义**：按颜色模型及相关规则组织和表示颜色的空间，例如 sRGB；颜色模型和具体色彩空间需区分。
- **语境**：第3课：cvtColor 转换
- **易混义或误译**：RGB 是颜色模型；sRGB 是具体色彩空间。通道顺序和数据存储类型也不等同于色彩空间。
- **英文例句**：cvtColor converts BGR data to HSV.
- **中文例句**：cvtColor 将 BGR 数据转换为 HSV。
- **版本**：MDN 在线术语；工程依赖版本不变
- **校核状态**：已核对 MDN 色彩空间与模型区分；中文为项目推荐译法，例句为 OpenCV 颜色表示转换
- **核对日期**：2026-10-30
- **来源**：[MDN 色彩空间与颜色模型](https://developer.mozilla.org/en-US/docs/Glossary/Color_space)

<a id="term-059"></a>
## TERM-059 · binary image

- **词性**：名词
- **领域**：图像分割
- **推荐译法**：二值图
- **定义**：像素只使用两种取值表示两类状态的图像。
- **语境**：第5课：阈值化输出
- **易混义或误译**：二值图不必以单个位存储每个像素。
- **英文例句**：Thresholding can produce a binary image.
- **中文例句**：阈值处理可以产生二值图。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的阈值输出二值图；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：阈值输出二值图](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2822)

<a id="term-060"></a>
## TERM-060 · kernel

- **词性**：名词
- **领域**：图像滤波
- **推荐译法**：核
- **定义**：局部算子使用的系数或结构元素；具体含义由操作决定。
- **语境**：第6课：设置 filter2D 核
- **易混义或误译**：滤波核与操作系统内核不同。
- **英文例句**：filter2D applies the supplied kernel to the image.
- **中文例句**：filter2D 将指定核应用到图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的filter2D 的核及相关运算；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：filter2D 的核及相关运算](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1400)

<a id="term-061"></a>
## TERM-061 · median filter

- **词性**：名词
- **领域**：图像滤波
- **推荐译法**：中值滤波
- **定义**：用邻域样本的中值替换像素的非线性滤波。
- **语境**：第6课：去除脉冲噪声
- **易混义或误译**：中值滤波不是均值滤波。
- **英文例句**：medianBlur replaces each pixel with a neighborhood median.
- **中文例句**：medianBlur 用邻域中值替换像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的medianBlur 的中值滤波；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：medianBlur 的中值滤波](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1239)

<a id="term-062"></a>
## TERM-062 · opening

- **词性**：名词
- **领域**：数学形态学
- **推荐译法**：开运算
- **定义**：先腐蚀后膨胀的形态学组合操作。
- **语境**：第7课：去除小前景
- **易混义或误译**：开运算与闭运算次序相反。
- **英文例句**：MORPH_OPEN applies erosion before dilation.
- **中文例句**：MORPH_OPEN 先腐蚀再膨胀。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的MORPH_OPEN 的操作顺序；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：MORPH_OPEN 的操作顺序](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L216)

<a id="term-063"></a>
## TERM-063 · closing

- **词性**：名词
- **领域**：数学形态学
- **推荐译法**：闭运算
- **定义**：先膨胀后腐蚀的形态学组合操作。
- **语境**：第7课：填补小间隙
- **易混义或误译**：闭运算与开运算次序相反。
- **英文例句**：MORPH_CLOSE applies dilation before erosion.
- **中文例句**：MORPH_CLOSE 先膨胀再腐蚀。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的MORPH_CLOSE 的操作顺序；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：MORPH_CLOSE 的操作顺序](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L216)

<a id="term-064"></a>
## TERM-064 · bounding box

- **词性**：名词
- **领域**：形状分析
- **推荐译法**：包围框
- **定义**：包围目标区域的矩形，可按轴对齐或旋转。
- **语境**：第9课：计算轮廓外接矩形
- **易混义或误译**：轴对齐包围框不一定紧贴旋转目标。
- **英文例句**：boundingRect returns an upright rectangle around the points.
- **中文例句**：boundingRect 返回包围这些点的轴对齐矩形。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的boundingRect 的最小轴对齐矩形；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：boundingRect 的最小轴对齐矩形](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/2d.hpp#L822)

<a id="term-065"></a>
## TERM-065 · keypoint

- **词性**：名词
- **领域**：局部特征
- **推荐译法**：关键点
- **定义**：检测器选出的图像位置及其尺度等属性。
- **语境**：第12课：ORB 检测
- **易混义或误译**：关键点与其描述子是不同数据。
- **英文例句**：ORB detects keypoints before computing descriptors.
- **中文例句**：ORB 在计算描述子前检测关键点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Feature2D 与 ORB 描述；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Feature2D 与 ORB 描述](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L225)

<a id="term-066"></a>
## TERM-066 · matching

- **词性**：名词
- **领域**：局部特征
- **推荐译法**：匹配
- **定义**：依据描述子距离等准则建立特征对应关系。
- **语境**：第12课：比较描述子
- **易混义或误译**：描述子最近邻不保证是正确几何对应。
- **英文例句**：The matcher compares descriptors from two images.
- **中文例句**：匹配器比较两幅图像的描述子。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的DescriptorMatcher 的两图描述子匹配；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：DescriptorMatcher 的两图描述子匹配](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L952)

<a id="term-067"></a>
## TERM-067 · tracking

- **词性**：名词
- **领域**：视频分析
- **推荐译法**：跟踪
- **定义**：在连续帧中估计目标或特征的位置变化。
- **语境**：第17课：跟踪点
- **易混义或误译**：跟踪不等于每帧独立检测。
- **英文例句**：The tracker updates point positions in the next frame.
- **中文例句**：跟踪器更新点在下一帧中的位置。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Lucas–Kanade 输出新点位置；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Lucas–Kanade 输出新点位置](https://github.com/opencv/opencv/blob/5.0.0/modules/video/include/opencv2/video/tracking.hpp#L139)

<a id="term-068"></a>
## TERM-068 · frame rate

- **词性**：名词
- **领域**：视频
- **推荐译法**：帧率
- **定义**：单位时间内采集、处理或播放的帧数；应注明测量位置。
- **语境**：第16课：检查视频帧率
- **易混义或误译**：文件标称帧率不一定等于实时处理吞吐量。
- **英文例句**：The video reports its nominal frame rate in frames per second.
- **中文例句**：视频以每秒帧数报告其标称帧率。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的CAP_PROP_FPS 及帧率；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：CAP_PROP_FPS 及帧率](https://github.com/opencv/opencv/blob/5.0.0/modules/videoio/include/opencv2/videoio.hpp#L135)

<a id="term-069"></a>
## TERM-069 · batch

- **词性**：名词
- **领域**：深度学习
- **推荐译法**：批次
- **定义**：一起输入模型处理的一组样本。
- **语境**：第22课：检查输入形状
- **易混义或误译**：批次维与通道维不能互换。
- **英文例句**：The model input uses a batch size of one.
- **中文例句**：模型输入使用批次大小一。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的imagesFromBlob 的批次维；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：imagesFromBlob 的批次维](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1420)

<a id="term-070"></a>
## TERM-070 · classification

- **词性**：名词
- **领域**：机器学习任务
- **推荐译法**：分类
- **定义**：为输入样本预测类别或类别得分的任务。
- **语境**：第21课：评价分类输出
- **易混义或误译**：分类通常不提供目标位置。
- **英文例句**：Classification assigns a class score to the input image.
- **中文例句**：分类为输入图像给出类别得分。
- **版本**：Google 在线术语资料；不改变工程依赖
- **校核状态**：已核对 Google 机器学习词汇：分类模型；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Google 机器学习词汇：分类模型](https://developers.google.com/machine-learning/glossary#classification_model)

<a id="term-071"></a>
## TERM-071 · segmentation

- **词性**：名词
- **领域**：计算机视觉任务
- **推荐译法**：分割
- **定义**：按像素或区域划分图像并赋予类别或实例等含义。
- **语境**：第23课：解释分割输出
- **易混义或误译**：语义分割和实例分割输出含义不同。
- **英文例句**：Segmentation assigns labels to image pixels.
- **中文例句**：分割为图像像素分配标签。
- **版本**：Torchvision 在线文档；不改变工程依赖
- **校核状态**：已核对 Torchvision 像素级语义分割与实例分割；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Torchvision 像素级语义分割与实例分割](https://docs.pytorch.org/vision/stable/models.html#semantic-segmentation)

<a id="term-072"></a>
## TERM-072 · image

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：图像
- **定义**：由像素组成的二维视觉数据
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：Mat 也可表示非图像矩阵
- **英文例句**：An image can be stored in a Mat.
- **中文例句**：图像可存储在 Mat 中。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 表示灰度或彩色图像；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 表示灰度或彩色图像](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L772)

<a id="term-073"></a>
## TERM-073 · row

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：行
- **定义**：二维数组中横向排列的一组元素，行号沿竖直方向递增。
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：行索引不是 x 坐标
- **英文例句**：The image has 480 rows.
- **中文例句**：图像有 480 行。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的二维行列索引；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的二维行列索引](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L778)

<a id="term-074"></a>
## TERM-074 · column

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：列
- **定义**：二维数组中竖向排列的一组元素，列号沿水平方向递增。
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：列索引不是 y 坐标
- **英文例句**：The image has 640 columns.
- **中文例句**：图像有 640 列。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的二维行列索引；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的二维行列索引](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L778)

<a id="term-075"></a>
## TERM-075 · width

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：宽度
- **定义**：图像水平方向的像素数量
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：宽度不等于以字节计的行步长
- **英文例句**：The image width is 640 pixels.
- **中文例句**：图像宽度是 640 像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的 Size 宽高构造示例；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的 Size 宽高构造示例](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L824)

<a id="term-076"></a>
## TERM-076 · height

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：高度
- **定义**：图像竖直方向的像素数量
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：高度不等于通道数量
- **英文例句**：The image height is 480 pixels.
- **中文例句**：图像高度是 480 像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 的 Size 宽高构造示例；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 的 Size 宽高构造示例](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L824)

<a id="term-077"></a>
## TERM-077 · coordinate

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：坐标
- **定义**：依据坐标系表示位置的数值
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：Point 的 x 对应列而不是行
- **英文例句**：Point uses x for the column coordinate.
- **中文例句**：Point 的 x 表示列坐标。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 Rect 坐标及图像遍历；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 Rect 坐标及图像遍历](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/types.hpp#L402)

<a id="term-078"></a>
## TERM-078 · index

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：索引
- **定义**：用于选择数组位置的下标
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：数组索引通常从零开始
- **英文例句**：The row index starts at zero.
- **中文例句**：行索引从零开始。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 零起始索引范围；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 零起始索引范围](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L778)

<a id="term-079"></a>
## TERM-079 · type

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：类型
- **定义**：Mat 元素深度与通道数的组合
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：类型不记录颜色语义
- **英文例句**：CV_8UC3 specifies an unsigned depth and three channels.
- **中文例句**：CV_8UC3 指定无符号深度和三个通道。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 类型和通道定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 类型和通道定义](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1012)

<a id="term-080"></a>
## TERM-080 · unsigned

- **词性**：形容词
- **领域**：图像与 Mat
- **推荐译法**：无符号的
- **定义**：整数类型不表示负数的属性
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：无符号运算仍须考虑范围
- **英文例句**：CV_8U stores unsigned 8-bit values.
- **中文例句**：CV_8U 存储无符号 8 位数值。
- **版本**：C++20 工作草案 N4861
- **校核状态**：已核对 C++ N4861 基本整数类型；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[C++ N4861 基本整数类型](https://timsong-cpp.github.io/cppwp/n4861/basic.fundamental)

<a id="term-081"></a>
## TERM-081 · grayscale

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：灰度
- **定义**：以单个强度相关数值表示像素的图像表示；QImage 提供 8 位和 16 位灰度格式。
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：灰度图仍有数据类型和数值范围
- **英文例句**：The grayscale Mat has one channel.
- **中文例句**：灰度 Mat 有一个通道。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）
- **校核状态**：已核对 Qt 6.8 QImage 灰度格式；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 QImage 灰度格式](https://doc.qt.io/qt-6.8/qimage.html#Format-enum)

<a id="term-082"></a>
## TERM-082 · BGR

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：蓝绿红通道顺序
- **定义**：依蓝、绿、红排列分量的颜色数据约定
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：Mat 类型本身不证明采用 BGR 顺序
- **英文例句**：Convert the BGR image before displaying RGB pixels.
- **中文例句**：显示 RGB 像素前先转换 BGR 图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的BGR 通道顺序；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：BGR 通道顺序](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3481)

<a id="term-083"></a>
## TERM-083 · RGB

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：红绿蓝通道顺序
- **定义**：依红、绿、蓝排列分量的颜色数据约定
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：RGB 顺序不等同于 BGR 顺序
- **英文例句**：The RGB image stores red before green and blue.
- **中文例句**：RGB 图像按红、绿、蓝顺序存储。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的RGB 与 BGR 顺序；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：RGB 与 BGR 顺序](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3481)

<a id="term-084"></a>
## TERM-084 · alpha

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：透明度分量
- **定义**：用于表示像素覆盖或透明程度的分量
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：预乘 alpha 与直通 alpha 的颜色分量不同
- **英文例句**：The alpha channel controls pixel transparency.
- **中文例句**：alpha 通道控制像素透明度。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt QImage 像素格式和透明度；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt QImage 像素格式和透明度](https://doc.qt.io/qt-6.8/qimage.html#Format-enum)

<a id="term-085"></a>
## TERM-085 · range

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：区间
- **定义**：由边界限定的一组数值或索引
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：应明确边界是否包含
- **英文例句**：The row range excludes its end index.
- **中文例句**：行区间不包含结束索引。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 子矩阵 Range 的左闭右开；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 子矩阵 Range 的左闭右开](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L845)

<a id="term-086"></a>
## TERM-086 · rectangle

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：矩形
- **定义**：由位置与宽高确定的矩形区域
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：确保 ROI 没有越过图像边界
- **英文例句**：Rect describes the image region to crop.
- **中文例句**：Rect 描述要裁剪的图像区域。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Rect 的位置宽高与边界；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Rect 的位置宽高与边界](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/types.hpp#L402)

<a id="term-087"></a>
## TERM-087 · dimensions

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：维数
- **定义**：数组坐标轴的数量
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：二维多通道 Mat 的通道数不等于维数
- **英文例句**：A three-channel image can still have two dimensions.
- **中文例句**：三通道图像仍可以是二维的。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 维数及多通道；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 维数及多通道](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1012)

<a id="term-088"></a>
## TERM-088 · empty

- **词性**：形容词
- **领域**：图像与 Mat
- **推荐译法**：空的
- **定义**：Mat 的 data 为空或元素总数为零的状态。
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：读取失败应先检查而不是直接处理
- **英文例句**：Check empty before using the loaded image.
- **中文例句**：使用读入的图像前检查是否为空。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat::empty 的数据和元素总数条件；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV 5.0.0：Mat::empty 的数据和元素总数条件](https://github.com/opencv/opencv/blob/5.0.0/modules/core/src/matrix.cpp#L1028)

<a id="term-089"></a>
## TERM-089 · header

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：矩阵头部
- **定义**：保存数组形状类型步长与数据位置等描述信息
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：复制头部不必复制像素
- **英文例句**：A copied Mat header can share pixel data.
- **中文例句**：复制的 Mat 头部可以共享像素数据。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 头部复制与引用计数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 头部复制与引用计数](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L819)

<a id="term-090"></a>
## TERM-090 · ownership

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：所有权
- **定义**：负责管理和释放资源的责任
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：Mat 外部数据的释放责任需由调用者核实
- **英文例句**：The external buffer retains its own ownership.
- **中文例句**：外部缓冲区仍由原所有者管理。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 外部存储不自动释放；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 外部存储不自动释放](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1096)

<a id="term-091"></a>
## TERM-091 · lifetime

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：生命周期
- **定义**：对象或资源从创建到失效的时间范围
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：指针存在不代表指向的数据仍有效
- **英文例句**：The buffer lifetime must cover every Mat access.
- **中文例句**：缓冲区生命周期必须覆盖每次 Mat 访问。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 外部存储不自动释放；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 外部存储不自动释放](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1096)

<a id="term-092"></a>
## TERM-092 · alias

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：别名
- **定义**：指向或共享同一数据的另一访问途径
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：别名修改可能影响原对象
- **英文例句**：The alias and original share pixels.
- **中文例句**：别名与原对象共享像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 子矩阵共享像素；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 子矩阵共享像素](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1082)

<a id="term-093"></a>
## TERM-093 · clone

- **词性**：动词（亦可作工程名词）
- **领域**：图像与 Mat
- **推荐译法**：克隆
- **定义**：创建包含独立像素数据的 Mat 副本
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：复制 Mat 头部不等于调用 clone
- **英文例句**：Use clone to obtain independent pixels.
- **中文例句**：使用 clone 获取独立像素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat::clone 的深拷贝；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV 5.0.0：Mat::clone 的深拷贝](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L819)

<a id="term-094"></a>
## TERM-094 · copy

- **词性**：动词（亦可作工程名词）
- **领域**：图像与 Mat
- **推荐译法**：复制
- **定义**：将数据或对象状态复制到另一位置的操作
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：不同接口可能复制头部或像素
- **英文例句**：copyTo copies pixels into the destination.
- **中文例句**：copyTo 将像素复制到目标。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat::copyTo 的像素复制；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV 5.0.0：Mat::copyTo 的像素复制](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L824)

<a id="term-095"></a>
## TERM-095 · allocation

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：分配
- **定义**：为数据请求存储空间的过程
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：分配与初始化不是同一个步骤
- **英文例句**：A Mat constructor can allocate storage for its elements.
- **中文例句**：Mat 构造函数可以为元素分配存储空间。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码中的Mat 存储构造与外部数据；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0：Mat 存储构造与外部数据](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L1096)

<a id="term-096"></a>
## TERM-096 · dangling pointer

- **词性**：名词
- **领域**：图像与 Mat
- **推荐译法**：悬空指针
- **定义**：所指对象生命周期已结束而不能再按原对象正常访问的指针。
- **语境**：第2–3课：阅读图像与 Mat 接口
- **易混义或误译**：对象生命周期结束后指针的使用受标准约束；非空并不证明对象仍然存活。
- **英文例句**：A pointer becomes dangling after its buffer is destroyed.
- **中文例句**：缓冲区销毁后指针会悬空。
- **版本**：C++20 工作草案 N4861
- **校核状态**：已核对 C++ N4861 对象生命周期；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[C++ N4861 对象生命周期](https://timsong-cpp.github.io/cppwp/n4861/basic.life)

<a id="term-097"></a>
## TERM-097 · detach

- **词性**：动词
- **领域**：Qt Widgets
- **推荐译法**：分离
- **定义**：共享值对象在需要独立数据时分离其共享存储
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：与销毁对象不同
- **英文例句**：A shared Qt value detaches when a modification requires a private copy.
- **中文例句**：修改需要独立副本时，共享的 Qt 值会分离。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的Qt implicit sharing 的 detach 机制；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Qt 6.8：Qt implicit sharing 的 detach 机制](https://doc.qt.io/qt-6.8/implicit-sharing.html)

<a id="term-098"></a>
## TERM-098 · widget

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：控件
- **定义**：可接收事件和绘制自身的用户界面对象
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：控件不一定是顶层窗口
- **英文例句**：QWidget is the base class of user interface objects.
- **中文例句**：QWidget 是界面对象的基类。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QWidget 类说明；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QWidget 类说明](https://doc.qt.io/qt-6.8/qwidget.html)

<a id="term-099"></a>
## TERM-099 · window

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：窗口
- **定义**：通常作为顶层显示的用户界面区域
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：不是所有控件都是顶层窗口
- **英文例句**：A top-level widget can be shown as a window.
- **中文例句**：顶层控件可以作为窗口显示。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QWidget 顶层窗口说明；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QWidget 顶层窗口说明](https://doc.qt.io/qt-6.8/qwidget.html)

<a id="term-100"></a>
## TERM-100 · dialog

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：对话框
- **定义**：用于短暂任务或交互的窗口
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：模态和非模态对话框的交互方式不同
- **英文例句**：A QDialog can be shown non-modally.
- **中文例句**：QDialog 可以非模态显示。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QDialog 模态与非模态；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QDialog 模态与非模态](https://doc.qt.io/qt-6.8/qdialog.html)

<a id="term-101"></a>
## TERM-101 · layout

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：布局
- **定义**：负责管理控件大小和位置的机制
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：布局不等同于样式表
- **英文例句**：A layout adjusts the size and position of its widgets.
- **中文例句**：布局调整其控件的位置和大小。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的Layout Management 的自动布局；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：Layout Management 的自动布局](https://doc.qt.io/qt-6.8/layout.html)

<a id="term-102"></a>
## TERM-102 · geometry

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：几何位置与尺寸
- **定义**：控件相对父控件的位置和大小，不包含窗口边框；顶层窗口坐标需结合窗口系统约定。
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：布局管理的 geometry 可能被布局重新计算
- **英文例句**：The layout updates the widget geometry.
- **中文例句**：布局更新控件的几何位置和尺寸。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QWidget geometry 属性；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QWidget geometry 属性](https://doc.qt.io/qt-6.8/qwidget.html#geometry-prop)

<a id="term-103"></a>
## TERM-103 · margin

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：边距
- **定义**：布局内容与其容器边界之间预留的空间
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：边距不同于控件之间的间距
- **英文例句**：Set the layout margins around its contents.
- **中文例句**：设置布局内容周围的边距。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QLayout contentsMargins；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QLayout contentsMargins](https://doc.qt.io/qt-6.8/qlayout.html#contentsMargins)

<a id="term-104"></a>
## TERM-104 · spacing

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：间距
- **定义**：布局中相邻项目之间的距离
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：间距不同于容器边距
- **英文例句**：The layout spacing separates adjacent widgets.
- **中文例句**：布局间距分隔相邻控件。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QLayout spacing 属性；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QLayout spacing 属性](https://doc.qt.io/qt-6.8/qlayout.html#spacing-prop)

<a id="term-105"></a>
## TERM-105 · size hint

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：建议尺寸
- **定义**：控件向布局管理器提供的合适尺寸建议
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：建议尺寸不是强制固定尺寸
- **英文例句**：The layout considers the widget size hint.
- **中文例句**：布局考虑控件的建议尺寸。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QWidget sizeHint 属性；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QWidget sizeHint 属性](https://doc.qt.io/qt-6.8/qwidget.html#sizeHint-prop)

<a id="term-106"></a>
## TERM-106 · parent

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：父对象
- **定义**：对象树中拥有子对象的对象
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：QObject 父子关系涉及生命周期
- **英文例句**：The parent destroys its child objects.
- **中文例句**：父对象销毁其子对象。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QObject 对象树所有权；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QObject 对象树所有权](https://doc.qt.io/qt-6.8/objecttrees.html)

<a id="term-107"></a>
## TERM-107 · child

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：子对象
- **定义**：对象树中归属于父对象的对象
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：对象树关系不等于 C++ 继承关系
- **英文例句**：The child is removed from its parent's object tree when destroyed.
- **中文例句**：子对象销毁时从父对象树移除。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QObject 子对象析构；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QObject 子对象析构](https://doc.qt.io/qt-6.8/objecttrees.html)

<a id="term-108"></a>
## TERM-108 · connect

- **词性**：动词
- **领域**：Qt Widgets
- **推荐译法**：连接
- **定义**：建立信号到槽或其他可调用对象的关系
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：连接不必然是跨线程调用
- **英文例句**：Connect the button signal to the handler.
- **中文例句**：将按钮信号连接到处理函数。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QObject::connect 信号与槽；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Qt 6.8：QObject::connect 信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)

<a id="term-109"></a>
## TERM-109 · disconnect

- **词性**：动词
- **领域**：Qt Widgets
- **推荐译法**：断开连接
- **定义**：移除已有的信号槽连接关系
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：断开连接不销毁发送者
- **英文例句**：Disconnect the signal when the connection is no longer needed.
- **中文例句**：不再需要连接时断开信号。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 QObject::disconnect 连接断开语义；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Qt 6.8 官方文档：disconnect](https://doc.qt.io/qt-6.8/qobject.html#disconnect)

<a id="term-110"></a>
## TERM-110 · sender

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：发送者
- **定义**：发出信号的 QObject
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：发送者不是接收槽的对象
- **英文例句**：The button is the sender of clicked.
- **中文例句**：按钮是 clicked 信号的发送者。
- **版本**：Qt 6.8
- **校核状态**：已核对 Qt 6.8 在线文档中的QObject 信号发送者；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8：QObject 信号发送者](https://doc.qt.io/qt-6.8/signalsandslots.html)

<a id="term-111"></a>
## TERM-111 · receiver

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：接收者
- **定义**：连接中承载槽或上下文的对象
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：接收者线程影响队列连接的执行位置
- **英文例句**：A queued slot runs in the receiver's thread.
- **中文例句**：队列连接的槽在接收者线程运行。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 队列连接在接收者线程执行；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 队列连接在接收者线程执行](https://doc.qt.io/qt-6.8/threads-qobject.html)

<a id="term-112"></a>
## TERM-112 · event

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：事件
- **定义**：用于向对象传递输入或状态变化的信息
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：事件不等同于信号
- **英文例句**：A widget receives a mouse press event.
- **中文例句**：控件收到鼠标按下事件。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 QEvent 与事件分派；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 QEvent 与事件分派](https://doc.qt.io/qt-6.8/eventsandfilters.html)

<a id="term-113"></a>
## TERM-113 · event loop

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：事件循环
- **定义**：持续取出并分派待处理事件的机制
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：长时间阻塞会影响事件分派
- **英文例句**：The event loop dispatches queued events.
- **中文例句**：事件循环分派排队的事件。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 QCoreApplication 事件循环；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 QCoreApplication 事件循环](https://doc.qt.io/qt-6.8/qcoreapplication.html#exec)

<a id="term-114"></a>
## TERM-114 · property

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：属性
- **定义**：可通过 Qt 元对象系统描述和访问的对象特性
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：并非每个普通成员都是 Qt 属性
- **英文例句**：A Qt property can be read through the meta-object system.
- **中文例句**：Qt 属性可通过元对象系统读取。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 属性和元对象系统；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 属性和元对象系统](https://doc.qt.io/qt-6.8/properties.html)

<a id="term-115"></a>
## TERM-115 · model

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：模型
- **定义**：模型视图架构中提供数据和访问接口的对象
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：模型与视图职责不同
- **英文例句**：The model exposes data to a view.
- **中文例句**：模型向视图提供数据。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 model/view 数据接口；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 model/view 数据接口](https://doc.qt.io/qt-6.8/model-view-programming.html)

<a id="term-116"></a>
## TERM-116 · view

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：视图
- **定义**：根据模型数据呈现内容的界面组件
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：视图不是数据唯一来源
- **英文例句**：The view displays items from the model.
- **中文例句**：视图显示模型中的项目。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 model/view 数据显示；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 model/view 数据显示](https://doc.qt.io/qt-6.8/model-view-programming.html)

<a id="term-117"></a>
## TERM-117 · resource

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：资源
- **定义**：通过 Qt 资源系统嵌入或注册并以资源路径访问的数据
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：资源路径不是普通绝对文件路径
- **英文例句**：Load the icon from a qrc resource path.
- **中文例句**：从 qrc 资源路径加载图标。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 资源系统与资源路径；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 资源系统与资源路径](https://doc.qt.io/qt-6.8/resources.html)

<a id="term-118"></a>
## TERM-118 · stylesheet

- **词性**：名词
- **领域**：Qt Widgets
- **推荐译法**：样式表
- **定义**：用于指定 Qt 控件外观规则的文本
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：样式表不替代布局管理
- **英文例句**：The stylesheet sets the button color.
- **中文例句**：样式表设置按钮颜色。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 样式表控件外观；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 样式表控件外观](https://doc.qt.io/qt-6.8/stylesheet.html)

<a id="term-119"></a>
## TERM-119 · paint

- **词性**：动词
- **领域**：Qt Widgets
- **推荐译法**：绘制
- **定义**：使用绘图接口将图形内容画到绘制设备的过程
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：绘制事件处理应遵循 Qt 约定
- **英文例句**：The widget paints its contents in paintEvent.
- **中文例句**：控件在 paintEvent 中绘制内容。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 QWidget paintEvent；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Qt 6.8 QWidget paintEvent](https://doc.qt.io/qt-6.8/qwidget.html#paintEvent)

<a id="term-120"></a>
## TERM-120 · render

- **词性**：动词
- **领域**：Qt Widgets
- **推荐译法**：渲染
- **定义**：将控件或图形内容生成到目标绘制设备的过程
- **语境**：工作台：Qt Widgets 与对象通信
- **易混义或误译**：不等同于修改原图像数据
- **英文例句**：QWidget render draws the widget into a paint device.
- **中文例句**：QWidget render 将控件绘制到绘图设备。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 QWidget render；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Qt 6.8 QWidget render](https://doc.qt.io/qt-6.8/qwidget.html#render)

<a id="term-121"></a>
## TERM-121 · source

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：源文件
- **定义**：包含程序源代码的文件
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：源文件与头文件职责不同
- **英文例句**：The compiler processes each source file.
- **中文例句**：编译器处理每个源文件。
- **版本**：C++20 工作草案 N4861
- **校核状态**：已核对 C++20 N4861 源文件与翻译单元；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[C++20 N4861 源文件与翻译单元](https://timsong-cpp.github.io/cppwp/n4861/lex.separate)

<a id="term-122"></a>
## TERM-122 · header

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：头文件
- **定义**：工程中用于提供声明或定义、通常通过 include 引入的文件；C++ 标准中的实现提供的 header 不必然是普通文件。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：这里不是 Mat 的数据头部
- **英文例句**：The source file includes its own header.
- **中文例句**：源文件包含自己的头文件。
- **版本**：C++20 工作草案 N4861
- **校核状态**：已核对 C++20 N4861 include 指令与头文件；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[C++20 N4861 include 指令与头文件](https://timsong-cpp.github.io/cppwp/n4861/cpp.include)

<a id="term-123"></a>
## TERM-123 · dependency

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：依赖项
- **定义**：本工程中应用构建或运行所需的组件，如 Qt 库；具体阶段所需依赖应分别核对。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：构建依赖与运行依赖不完全相同
- **英文例句**：The application depends on Qt and OpenCV.
- **中文例句**：应用依赖 Qt 和 OpenCV。
- **版本**：Qt 6.8 在线文档；项目固定 6.8.3
- **校核状态**：已核对 Qt Windows 部署中的运行库及依赖；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 Windows 部署的应用依赖与运行库](https://doc.qt.io/qt-6.8/windows-deployment.html)

<a id="term-124"></a>
## TERM-124 · toolchain

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：工具链
- **定义**：用于构建目标平台程序的一组编译及相关工具
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：工具链不是单个库
- **英文例句**：The toolchain selects the compiler for the build.
- **中文例句**：工具链选择构建使用的编译器。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 工具链与编译器；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 3.30 工具链与编译器](https://cmake.org/cmake/help/v3.30/manual/cmake-toolchains.7.html)

<a id="term-125"></a>
## TERM-125 · configure

- **词性**：动词
- **领域**：构建与部署
- **推荐译法**：配置
- **定义**：CMake 读取项目和缓存并确定构建设置的阶段
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：配置不等同于编译
- **英文例句**：Configure the project with the selected preset.
- **中文例句**：使用选定预设配置项目。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 配置并生成构建系统；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[CMake 3.30 配置并生成构建系统](https://cmake.org/cmake/help/v3.30/manual/cmake.1.html)

<a id="term-126"></a>
## TERM-126 · generate

- **词性**：动词
- **领域**：构建与部署
- **推荐译法**：生成
- **定义**：CMake 输出所选原生构建系统文件的阶段
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：生成构建文件不是编译源文件
- **英文例句**：CMake generates Ninja build files.
- **中文例句**：CMake 生成 Ninja 构建文件。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 生成构建系统；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[CMake 3.30 生成构建系统](https://cmake.org/cmake/help/v3.30/manual/cmake.1.html)

<a id="term-127"></a>
## TERM-127 · build

- **词性**：动词（亦可作工程名词）
- **领域**：构建与部署
- **推荐译法**：构建
- **定义**：执行构建系统规则以生成目标产物
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：构建可能包含编译和链接等步骤
- **英文例句**：Build the application after configuration.
- **中文例句**：配置后构建应用程序。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 构建项目；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[CMake 3.30 构建项目](https://cmake.org/cmake/help/v3.30/manual/cmake.1.html#build-a-project)

<a id="term-128"></a>
## TERM-128 · compile

- **词性**：动词
- **领域**：构建与部署
- **推荐译法**：编译
- **定义**：将源代码翻译为目标代码的过程
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：编译成功不保证链接成功
- **英文例句**：Compile the source files into object files.
- **中文例句**：将源文件编译为目标文件。
- **版本**：官方在线文档；不改变工程工具链
- **校核状态**：已核对 Microsoft MSVC 翻译单元及目标文件；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Microsoft MSVC 翻译单元及目标文件](https://learn.microsoft.com/en-us/cpp/build/projects-and-build-systems-cpp?view=msvc-170)

<a id="term-129"></a>
## TERM-129 · link

- **词性**：动词（亦可作工程名词）
- **领域**：构建与部署
- **推荐译法**：链接
- **定义**：将分别翻译的单元和所需库组合成程序的过程；具体符号解析由工具链实现。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：链接成功不保证运行时动态库可用
- **英文例句**：Link the executable to the required libraries.
- **中文例句**：将可执行文件链接到所需库。
- **版本**：C++20 工作草案 N4861
- **校核状态**：已核对 C++20 N4861 分别翻译并链接；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[C++20 N4861 分别翻译并链接](https://timsong-cpp.github.io/cppwp/n4861/lex.separate)

<a id="term-130"></a>
## TERM-130 · target

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：目标
- **定义**：构建系统中可执行文件库或自定义构建项的逻辑实体
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：目标名不一定等于源文件名
- **英文例句**：The executable target links to Qt Widgets.
- **中文例句**：可执行目标链接 Qt Widgets。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 构建目标；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 3.30 构建目标](https://cmake.org/cmake/help/v3.30/manual/cmake-buildsystem.7.html)

<a id="term-131"></a>
## TERM-131 · preset

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：预设
- **定义**：可复用的 CMake 配置或构建参数集合
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：预设不包含所有机器上的工具安装
- **英文例句**：Choose the local debug preset.
- **中文例句**：选择本机调试预设。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 配置与构建预设；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 3.30 配置与构建预设](https://cmake.org/cmake/help/v3.30/manual/cmake-presets.7.html)

<a id="term-132"></a>
## TERM-132 · cache

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：缓存
- **定义**：CMake 持久保存配置变量值的机制
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：旧缓存可能保留旧工具链路径
- **英文例句**：The cache stores configuration variables.
- **中文例句**：缓存保存配置变量。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 缓存条目；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 3.30 缓存条目](https://cmake.org/cmake/help/v3.30/manual/cmake.1.html#generate-a-project-buildsystem)

<a id="term-133"></a>
## TERM-133 · Debug

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：调试配置
- **定义**：CMake 常见构建配置名；实际编译选项由工具链和项目设置决定。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：具体优化和调试信息取决于工具链设置
- **英文例句**：Build the Debug configuration for debugging.
- **中文例句**：构建 Debug 配置以进行调试。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 常用 Debug 配置名；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 常用 Debug 配置名](https://cmake.org/cmake/help/v3.30/variable/CMAKE_BUILD_TYPE.html)

<a id="term-134"></a>
## TERM-134 · Release

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：发布配置
- **定义**：CMake 常见构建配置名；实际编译选项由工具链和项目设置决定。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：Release 不必然没有调试符号
- **英文例句**：Build the Release configuration for deployment.
- **中文例句**：构建 Release 配置以便部署。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 常用 Release 配置名；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 常用 Release 配置名](https://cmake.org/cmake/help/v3.30/variable/CMAKE_BUILD_TYPE.html)

<a id="term-135"></a>
## TERM-135 · runtime

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：运行时
- **定义**：程序运行阶段；也用于指该阶段需要的支持库，例如 Windows 上的 Qt 运行库。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：运行时缺失 DLL 不属于编译错误
- **英文例句**：The application needs its DLLs at runtime.
- **中文例句**：应用在运行时需要其 DLL。
- **版本**：Qt 6.8 在线文档；项目固定 6.8.3
- **校核状态**：已核对 Qt Windows 部署中的运行库及依赖；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 Windows 部署的应用依赖与运行库](https://doc.qt.io/qt-6.8/windows-deployment.html)

<a id="term-136"></a>
## TERM-136 · static

- **词性**：形容词
- **领域**：构建与部署
- **推荐译法**：静态库
- **定义**：链接时将所需目标代码纳入最终目标的库类型
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：Windows 的 lib 扩展名也可能表示导入库
- **英文例句**：A static library is linked into the executable.
- **中文例句**：静态库链接到可执行文件中。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 STATIC 库；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[CMake 3.30 STATIC 库](https://cmake.org/cmake/help/v3.30/command/add_library.html)

<a id="term-137"></a>
## TERM-137 · shared library

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：共享库
- **定义**：运行时可由多个程序加载使用的库
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：共享库的部署需求与静态库不同
- **英文例句**：The executable loads the shared library at runtime.
- **中文例句**：可执行文件在运行时加载共享库。
- **版本**：CMake 3.30 在线文档（当前 3.30.9）；项目固定 3.30.5
- **校核状态**：已核对 CMake 3.30 SHARED 库；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[CMake 3.30 SHARED 库](https://cmake.org/cmake/help/v3.30/command/add_library.html)

<a id="term-138"></a>
## TERM-138 · deployment

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：部署
- **定义**：将可执行程序及运行所需文件安排到目标环境的过程
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：构建成功不代表部署完整
- **英文例句**：Deploy the application with its required runtime libraries.
- **中文例句**：部署应用及其所需运行库。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；工程固定 6.8.3
- **校核状态**：已核对 Qt 6.8 Windows 部署及运行依赖；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 Windows 部署及运行依赖](https://doc.qt.io/qt-6.8/windows-deployment.html)

<a id="term-139"></a>
## TERM-139 · symbol

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：符号
- **定义**：链接器用于标识函数或对象等实体的名称
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：缺失符号与缺失运行时 DLL 是不同阶段的问题
- **英文例句**：The linker reports an unresolved symbol.
- **中文例句**：链接器报告未解析符号。
- **版本**：MSVC 170 官方文档
- **校核状态**：已核对 Microsoft MSVC LNK2001 符号定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Microsoft MSVC LNK2001 符号定义](https://learn.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk2001?view=msvc-170)

<a id="term-140"></a>
## TERM-140 · assertion

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：断言
- **定义**：检查程序预期条件并在失败时触发诊断的机制
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：断言不能替代所有生产错误处理
- **英文例句**：An assertion checks that the pointer is valid.
- **中文例句**：断言检查指针是否有效。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt Q_ASSERT 检查条件；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt Q_ASSERT 检查条件](https://doc.qt.io/qt-6.8/qtassert.html#Q_ASSERT)

<a id="term-141"></a>
## TERM-141 · warning

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：警告
- **定义**：编译器等工具对潜在问题发出的诊断；通常不阻止构建，但可配置为错误。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：项目设置可能将警告视为错误
- **英文例句**：Read the first compiler warning.
- **中文例句**：阅读第一条编译器警告。
- **版本**：官方在线文档；不改变工程工具链
- **校核状态**：已核对 MSVC 编译器警告与 /WX；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[MSVC 编译器警告与 /WX](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170)

<a id="term-142"></a>
## TERM-142 · error

- **词性**：名词
- **领域**：构建与部署
- **推荐译法**：错误
- **定义**：编译或构建工具报告的错误诊断；错误可能阻止生成目标产物。
- **语境**：第1课：配置、构建与运行工程
- **易混义或误译**：后续报错可能源于最早的错误
- **英文例句**：Inspect the first configuration error.
- **中文例句**：检查第一条配置错误。
- **版本**：官方在线文档；不改变工程工具链
- **校核状态**：已核对 MSVC 构建错误与诊断；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[MSVC 构建错误与诊断](https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/c-cpp-build-errors?view=msvc-170)

<a id="term-143"></a>
## TERM-143 · normalization

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：归一化
- **定义**：按指定规则调整数值范围或范数
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：归一化不总是映射到零到一
- **英文例句**：normalize rescales the selected pixel values.
- **中文例句**：normalize 调整选定像素值的尺度。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 normalize 的范围与范数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 normalize 的范围与范数](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core.hpp#L728)

<a id="term-144"></a>
## TERM-144 · blur

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：模糊
- **定义**：降低图像局部细节变化的平滑操作
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：不同模糊方法的核和统计量不同
- **英文例句**：blur computes a local average.
- **中文例句**：blur 计算局部平均值。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 blur 的局部平均；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 blur 的局部平均](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1360)

<a id="term-145"></a>
## TERM-145 · edge

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：边缘
- **定义**：图像中强度等属性变化明显的位置；Canny 使用梯度响应识别边缘。
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：边缘图与对象轮廓不完全相同
- **英文例句**：Canny detects edges in the image.
- **中文例句**：Canny 检测图像中的边缘。
- **版本**：OpenCV 4.13 在线概念资料；项目固定 5.0.0
- **校核状态**：已核对 OpenCV Canny 图像强度变化和边缘；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV Canny 图像强度变化和边缘](https://docs.opencv.org/4.13.0/da/d22/tutorial_py_canny.html)

<a id="term-146"></a>
## TERM-146 · resize

- **词性**：动词
- **领域**：视觉课程
- **推荐译法**：调整尺寸
- **定义**：重新采样图像到指定输出尺寸
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：缩放结果依插值方法而异
- **英文例句**：resize produces an image with the requested size.
- **中文例句**：resize 生成指定尺寸的图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 resize 尺寸与插值；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV 5 resize 尺寸与插值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2058)

<a id="term-147"></a>
## TERM-147 · warp

- **词性**：动词（亦可作工程名词）
- **领域**：视觉课程
- **推荐译法**：几何映射
- **定义**：按空间变换对图像进行重采样
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：映射方向与逆映射选项需核对
- **英文例句**：warpPerspective maps the image using a homography.
- **中文例句**：warpPerspective 使用单应矩阵映射图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 warpPerspective 的映射；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV 5 warpPerspective 的映射](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2130)

<a id="term-148"></a>
## TERM-148 · feature

- **词性**：名词
- **领域**：局部图像特征
- **推荐译法**：特征
- **定义**：图像中可用于描述或匹配的局部信息；ORB 检测关键点并计算描述子。
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：特征点与描述子不是同一个数据
- **英文例句**：ORB extracts local image features.
- **中文例句**：ORB 提取局部图像特征。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 ORB 局部特征与描述子；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 ORB 局部特征与描述子](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L437)

<a id="term-149"></a>
## TERM-149 · extrinsic

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：相机外参
- **定义**：描述坐标系之间旋转和平移的参数
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：外参与相机内部成像参数不同
- **英文例句**：The extrinsic parameters transform world points into camera coordinates.
- **中文例句**：外参将世界坐标点变换到相机坐标。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 标定参数的坐标变换；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 标定参数的坐标变换](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp#L581)

<a id="term-150"></a>
## TERM-150 · stereo

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：双目视觉
- **定义**：利用两个视角之间的对应关系恢复空间信息的方法
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：两个图像之间仍需要标定和匹配
- **英文例句**：Stereo matching estimates disparity from two views.
- **中文例句**：双目匹配从两个视图估计视差。
- **版本**：OpenCV 4.13 官方概念资料；工程固定 OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 4.13 双目与视差；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 双目与视差](https://docs.opencv.org/4.13.0/dd/d53/tutorial_py_depthmap.html)

<a id="term-151"></a>
## TERM-151 · depth map

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：深度图
- **定义**：各像素或区域对应空间深度的图像表示
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：深度单位及无效值须由数据契约说明
- **英文例句**：The depth map stores a depth value at each valid pixel.
- **中文例句**：深度图在每个有效像素存储深度值。
- **版本**：OpenCV 4.13 官方概念资料；工程固定 OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 4.13 双目深度与视差；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 双目深度与视差](https://docs.opencv.org/4.13.0/dd/d53/tutorial_py_depthmap.html)

<a id="term-152"></a>
## TERM-152 · timestamp

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：时间戳
- **定义**：标识帧或事件发生时间的数值
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：时间基准和单位不能自行假定
- **英文例句**：Record a timestamp for each captured frame.
- **中文例句**：为每个采集帧记录时间戳。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 视频时间戳属性；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 视频时间戳属性](https://github.com/opencv/opencv/blob/5.0.0/modules/videoio/include/opencv2/videoio.hpp#L230)

<a id="term-153"></a>
## TERM-153 · motion

- **词性**：名词
- **领域**：视频图像运动
- **推荐译法**：运动
- **定义**：本课语境中，图像特征位置随相邻帧时间变化的表观运动。
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：图像运动不自动等同于三维物体运动
- **英文例句**：Estimate motion between consecutive frames.
- **中文例句**：估计连续帧之间的运动。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 光流与特征点位置变化；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 光流与特征点位置变化](https://github.com/opencv/opencv/blob/5.0.0/modules/video/include/opencv2/video/tracking.hpp#L139)

<a id="term-154"></a>
## TERM-154 · shape

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：形状
- **定义**：张量各维长度构成的序列
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：形状不单独指定维度的语义顺序
- **英文例句**：Check the input tensor shape before inference.
- **中文例句**：推理前检查输入张量形状。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 MatShape 维数与长度；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 MatShape 维数与长度](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/mat.hpp#L101)

<a id="term-155"></a>
## TERM-155 · preprocessing

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：预处理
- **定义**：推理前将输入转换为模型约定形式的步骤
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：均值、尺度与通道顺序必须遵循模型契约
- **英文例句**：Preprocessing may resize and normalize the input image as required by the model.
- **中文例句**：预处理可按模型要求调整输入尺寸并归一化。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 blobFromImage 缩放和尺寸；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 blobFromImage 缩放和尺寸](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1272)

<a id="term-156"></a>
## TERM-156 · postprocessing

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：后处理
- **定义**：本课检测语境中，将模型输出转换为任务结果的步骤，例如按分数和重叠筛选候选框。
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：后处理参数不能凭空假定
- **英文例句**：Postprocessing filters overlapping detections.
- **中文例句**：后处理筛除重叠检测结果。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 NMS 候选框筛选；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 NMS 候选框筛选](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1438)

<a id="term-157"></a>
## TERM-157 · detection

- **词性**：名词
- **领域**：视觉课程
- **推荐译法**：目标检测
- **定义**：预测图像中目标类别及其位置的任务
- **语境**：视觉课程：图像、视频和模型处理
- **易混义或误译**：检测与整图分类输出不同
- **英文例句**：Detection returns a class and a box for each candidate.
- **中文例句**：目标检测为每个候选输出类别和边界框。
- **版本**：官方在线文档；不改变工程工具链
- **校核状态**：已核对 Google 目标检测类别位置输出；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Google 目标检测类别位置输出](https://developers.google.com/edge/mediapipe/solutions/vision/object_detector)

<a id="term-158"></a>
## TERM-158 · inspection

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：检验
- **定义**：将对象或结果与规定要求比较以判断符合性的活动
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：检验不同于单次数值测量
- **英文例句**：Inspection compares the measured dimension with its tolerance.
- **中文例句**：检验将测得尺寸与公差比较。
- **版本**：ISO/IEC 17020:2026
- **校核状态**：已核对 ISO 检验说明；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[ISO/IEC 17020:2026 检验](https://committee.iso.org/cms/live/live/en/sites/isoorg/contents/data/standard/08/62/86292.html)

<a id="term-159"></a>
## TERM-159 · defect

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：缺陷
- **定义**：与预期或规定用途相关的不符合。
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：缺陷与不符合不完全同义；划痕是否构成缺陷依使用要求而定。
- **英文例句**：The inspection flags a scratch as a defect.
- **中文例句**：检验将划痕标记为缺陷。
- **版本**：ISO 9000 在线术语
- **校核状态**：已核对 ISO 9000 defect 定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[ISO 9000 术语 3.5.14 defect](https://www.iso.org/obp/ui?_escaped_fragment_=iso%3Astd%3Aiso%3A9000%3Aed-5%3Av1%3Aen)

<a id="term-160"></a>
## TERM-160 · measurement

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：测量
- **定义**：通过实验获取可合理赋予量的一个或多个值的过程
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：测量结果需要单位和不确定度语境
- **英文例句**：Measurement estimates the width in millimetres.
- **中文例句**：测量以毫米估计宽度。
- **版本**：见来源版本；项目依赖不变
- **校核状态**：已核对 VIM 3 第 2.1 条 measurement；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[VIM 3 第 2.1 条 measurement](https://jcgm.bipm.org/vim/en/2.1.html)

<a id="term-161"></a>
## TERM-161 · tolerance

- **词性**：名词
- **领域**：尺寸计量
- **推荐译法**：公差
- **定义**：尺寸公差上下极限之差；允许区间由上下极限共同确定。
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：公差是极限之差，不等同于允许区间或测量不确定度；合格判定需约定决策规则。
- **英文例句**：Compare the measured dimension with the tolerance using the agreed decision rule.
- **中文例句**：按约定的判定规则将测量尺寸与公差比较。
- **版本**：ISO 10209:2022
- **校核状态**：已核对 ISO 10209:2022 3.5.2 尺寸公差；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[ISO 10209:2022 3.5.2 尺寸公差](https://www.iso.org/obp/ui?_escaped_fragment_=iso%3Astd%3Aiso%3A10209%3Aed-2%3Av1%3Aen)

<a id="term-162"></a>
## TERM-162 · repeatability

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：重复性
- **定义**：在一组重复性条件下的测量精密度。
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：重复性不是准确度
- **英文例句**：Repeatability is evaluated using repeated measurements under the same conditions.
- **中文例句**：重复性通过相同条件下的重复测量评价。
- **版本**：见来源版本；项目依赖不变
- **校核状态**：已核对 VIM 3 第 2.21 条 repeatability；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[VIM 3 第 2.21 条 repeatability](https://jcgm.bipm.org/vim/en/2.21.html)

<a id="term-163"></a>
## TERM-163 · uncertainty

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：测量不确定度
- **定义**：表征依据所用信息赋予被测量值的分散程度的非负参数
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：不确定度不等于误差
- **英文例句**：Report the measurement result with its uncertainty.
- **中文例句**：报告测量结果及其不确定度。
- **版本**：见来源版本；项目依赖不变
- **校核状态**：已核对 VIM 3 第 2.26 条 uncertainty；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[VIM 3 第 2.26 条 uncertainty](https://jcgm.bipm.org/vim/en/2.26.html)

<a id="term-164"></a>
## TERM-164 · training

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：训练
- **定义**：利用训练数据调整模型参数的过程
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：训练与固定参数执行推理不同
- **英文例句**：Training updates model parameters from examples.
- **中文例句**：训练利用样本更新模型参数。
- **版本**：见来源版本；项目依赖不变
- **校核状态**：已核对 PyTorch 教程训练循环和参数更新；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[PyTorch 教程训练循环和参数更新](https://pytorch.org/tutorials/beginner/basics/optimization_tutorial.html)

<a id="term-165"></a>
## TERM-165 · confidence

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：置信分数
- **定义**：模型给出的对某个预测的分数
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：分数不一定是经过校准的概率
- **英文例句**：Filter predictions below the confidence threshold.
- **中文例句**：过滤低于置信分数阈值的预测。
- **版本**：官方在线文档；不改变工程工具链
- **校核状态**：已核对 Google 目标检测置信分数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Google 目标检测置信分数](https://developers.google.com/edge/mediapipe/solutions/vision/object_detector/web_js)

<a id="term-166"></a>
## TERM-166 · IoU

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：交并比
- **定义**：两个区域交集面积与并集面积的比值
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：交并比不是分类精确率
- **英文例句**：Compute IoU between the predicted and reference boxes.
- **中文例句**：计算预测框与参考框的交并比。
- **版本**：见来源版本；项目依赖不变
- **校核状态**：已核对 Torchvision box_iou 交并比；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Torchvision box_iou 交并比](https://docs.pytorch.org/vision/stable/generated/torchvision.ops.box_iou.html)

<a id="term-167"></a>
## TERM-167 · throughput

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：吞吐量
- **定义**：单位时间完成的任务或处理的数据量
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：吞吐量与单次延迟不同
- **英文例句**：Measure throughput in frames per second.
- **中文例句**：以每秒帧数测量吞吐量。
- **版本**：Intel 2017 文章
- **校核状态**：已核对 Intel 延迟与吞吐量；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Intel 延迟与吞吐量](https://www.intel.com/content/www/us/en/developer/articles/technical/optimizing-computer-applications-for-latency-part-1-configuring-the-hardware.html)

<a id="term-168"></a>
## TERM-168 · benchmark

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：基准测试
- **定义**：在指定条件下测量并比较性能的实验
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：不同硬件和输入的结果不能直接等同
- **英文例句**：Run the benchmark with a fixed input and configuration.
- **中文例句**：使用固定输入和配置运行基准测试。
- **版本**：Google Benchmark 在线指南，版本未固定
- **校核状态**：已核对 Google Benchmark 基准运行与结果；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Google Benchmark 基准运行与结果](https://github.com/google/benchmark/blob/main/docs/user_guide.md)

<a id="term-169"></a>
## TERM-169 · profiling

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：性能剖析
- **定义**：采集运行数据以定位程序资源消耗和热点
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：剖析结果受采样和测量开销影响
- **英文例句**：Profiling identifies the functions consuming the most CPU time.
- **中文例句**：性能剖析识别消耗 CPU 时间最多的函数。
- **版本**：VTune 2025.4 文档；仅概念参考
- **校核状态**：已核对 Intel VTune 热点采样；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Intel VTune 热点采样](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2025-4/basic-hotspots-analysis.html)

<a id="term-170"></a>
## TERM-170 · asynchronous

- **词性**：形容词
- **领域**：工业测量与性能
- **推荐译法**：异步的
- **定义**：发起操作后不必等待其完成即可继续其他工作的执行方式
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：异步不必然意味着并行
- **英文例句**：The asynchronous task reports completion later.
- **中文例句**：异步任务稍后报告完成。
- **版本**：MDN 在线术语资料
- **校核状态**：已核对 MDN 异步术语；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[MDN 异步术语](https://developer.mozilla.org/en-US/docs/Glossary/Asynchronous)

<a id="term-171"></a>
## TERM-171 · synchronization

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：同步协调
- **定义**：协调并发执行或共享数据访问的机制
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：同步不总是意味着阻塞整个程序
- **英文例句**：A mutex provides synchronization for shared state.
- **中文例句**：互斥锁为共享状态提供同步协调。
- **版本**：Qt 6.8 在线文档（当前 6.8.9）；项目固定 6.8.3
- **校核状态**：已核对 Qt 6.8 QMutex 互斥访问；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Qt 6.8 QMutex 互斥访问](https://doc.qt.io/qt-6.8/threads-synchronizing.html)

<a id="term-172"></a>
## TERM-172 · race condition

- **词性**：名词
- **领域**：工业测量与性能
- **推荐译法**：竞态条件
- **定义**：程序结果依赖并发操作执行时序的情况
- **语境**：第19–25课：测量、模型与性能评价
- **易混义或误译**：广义竞态与 C++ 标准规定的数据竞争并非同义；无数据竞争也可能存在逻辑竞态。
- **英文例句**：A race condition can occur when updates depend on timing.
- **中文例句**：更新结果取决于时序时可能发生竞态。
- **版本**：MDN 在线技术文档
- **校核状态**：已核对 MDN 条件请求中的竞态实例；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[MDN 条件请求中的竞态实例](https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Conditional_requests)

<a id="term-173"></a>
## TERM-173 · layout

- **词性**：名词
- **领域**：深度学习张量
- **推荐译法**：维度布局
- **定义**：张量各维所代表含义及其排列约定，例如 NCHW。
- **语境**：第22课：检查模型输入维度顺序
- **易混义或误译**：张量布局不是 Qt 控件布局；形状相同也不能推断维度语义。
- **英文例句**：The model expects the input in NCHW layout.
- **中文例句**：模型要求输入使用 NCHW 维度布局。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 blob 输入维度布局；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 blob 输入维度布局](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1420)

<a id="term-174"></a>
## TERM-174 · watershed

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：分水岭
- **定义**：将图像视作地形并从标记出发划分区域的分割方法
- **语境**：第10课：标记分水岭
- **易混义或误译**：分水岭边界和原始边缘图不同
- **英文例句**：watershed uses marker labels to separate image regions.
- **中文例句**：watershed 使用标记标签分离图像区域。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 watershed 标记与分割；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 watershed 标记与分割](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3170)

<a id="term-175"></a>
## TERM-175 · Canny

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：Canny 边缘检测
- **定义**：通过梯度和滞后阈值等步骤检测边缘的算法
- **语境**：第8课：Canny 双阈值
- **易混义或误译**：双阈值不能理解为两次独立二值化
- **英文例句**：Canny uses two thresholds to link edge candidates.
- **中文例句**：Canny 使用两个阈值连接边缘候选。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 Canny 双阈值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 Canny 双阈值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1600)

<a id="term-176"></a>
## TERM-176 · Sobel

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：Sobel 算子
- **定义**：通过离散导数核估计图像方向导数的算子
- **语境**：第8课：计算梯度
- **易混义或误译**：方向导数本身不是完整梯度幅值
- **英文例句**：Sobel estimates the horizontal image derivative.
- **中文例句**：Sobel 估计图像水平方向导数。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 Sobel 方向导数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 Sobel 方向导数](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1481)

<a id="term-177"></a>
## TERM-177 · Hough transform

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：霍夫变换
- **定义**：通过参数空间投票检测直线或其他参数化形状的方法
- **语境**：第9课：直线检测
- **易混义或误译**：检测结果依阈值和参数范围变化
- **英文例句**：HoughLines detects line candidates in the edge image.
- **中文例句**：HoughLines 在边缘图中检测直线候选。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 HoughLines；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 HoughLines](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1810)

<a id="term-178"></a>
## TERM-178 · RANSAC

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：随机抽样一致性
- **定义**：通过重复抽取样本与评估内点估计模型的稳健方法
- **语境**：第13课：剔除错误匹配
- **易混义或误译**：估计结果依阈值及随机采样变化
- **英文例句**：RANSAC estimates a homography while rejecting outliers.
- **中文例句**：RANSAC 在排除异常对应点的同时估计单应矩阵。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 findHomography RANSAC；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 findHomography RANSAC](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L734)

<a id="term-179"></a>
## TERM-179 · ORB

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：ORB 特征
- **定义**：结合关键点检测和二进制描述子的局部特征方法
- **语境**：第12课：特征匹配
- **易混义或误译**：ORB 描述子与 SIFT 浮点描述子距离准则不同
- **英文例句**：ORB computes binary descriptors for keypoints.
- **中文例句**：ORB 为关键点计算二进制描述子。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 ORB 特征和描述子；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 ORB 特征和描述子](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L437)

<a id="term-180"></a>
## TERM-180 · SIFT

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：尺度不变特征变换
- **定义**：检测并描述图像局部特征的方法
- **语境**：第12课：对比特征
- **易混义或误译**：不同描述子需要适当的匹配距离
- **英文例句**：SIFT computes descriptors for detected keypoints.
- **中文例句**：SIFT 为检测到的关键点计算描述子。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 SIFT 特征提取；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 SIFT 特征提取](https://github.com/opencv/opencv/blob/5.0.0/modules/features/include/opencv2/features.hpp#L347)

<a id="term-181"></a>
## TERM-181 · subpixel

- **词性**：形容词
- **领域**：课程算法
- **推荐译法**：亚像素
- **定义**：以小于一个像素间隔的尺度估计连续位置或位移；不自动保证相应准确度。
- **语境**：第19课：角点精细化
- **易混义或误译**：亚像素坐标不等于提高光学分辨率；误差须独立评价。
- **英文例句**：Refine the detected corner to subpixel coordinates.
- **中文例句**：将检测到的角点精细化至亚像素坐标。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 cornerSubPix；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 cornerSubPix](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1770)

<a id="term-182"></a>
## TERM-182 · reprojection error

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：重投影误差
- **定义**：三维点经相机模型投影后的预测位置与观测图像点的差异
- **语境**：第14课：评估标定
- **易混义或误译**：小重投影误差不自动保证所有测量场景精度
- **英文例句**：Compute reprojection error after camera calibration.
- **中文例句**：相机标定后计算重投影误差。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 calibrateCamera 重投影误差；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 calibrateCamera 重投影误差](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp#L663)

<a id="term-183"></a>
## TERM-183 · PnP

- **词性**：名词
- **领域**：课程算法
- **推荐译法**：透视 n 点位姿估计
- **定义**：根据三维点与其二维投影对应关系估计相机位姿的问题
- **语境**：第15课：位姿估计
- **易混义或误译**：对应点质量及坐标系约定影响结果
- **英文例句**：solvePnP estimates pose from 3D and 2D correspondences.
- **中文例句**：solvePnP 根据三维和二维对应点估计位姿。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 solvePnP 三维二维对应；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 solvePnP 三维二维对应](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L980)

<a id="term-184"></a>
## TERM-184 · Otsu threshold

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：大津阈值
- **定义**：依据图像直方图自动选取阈值的方法
- **语境**：第5课：自动阈值
- **易混义或误译**：阈值自动选择不保证分割成功
- **英文例句**：Otsu selects a threshold from the image histogram.
- **中文例句**：大津法依据图像直方图选取阈值。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的Otsu 源码直方图和阈值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 Otsu 源码直方图和阈值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/src/thresh.cpp#L1183)

<a id="term-185"></a>
## TERM-185 · adaptive threshold

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：自适应阈值
- **定义**：根据像素邻域计算阈值的分割方法
- **语境**：第5课：不均匀照明
- **易混义或误译**：局部阈值结果受邻域大小影响
- **英文例句**：adaptiveThreshold computes a local threshold for each pixel.
- **中文例句**：adaptiveThreshold 为每个像素计算局部阈值。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的adaptiveThreshold 的逐像素局部阈值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 adaptiveThreshold 的逐像素局部阈值](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2870)

<a id="term-186"></a>
## TERM-186 · structuring element

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：结构元素
- **定义**：形态学操作中定义参与邻域形状的元素
- **语境**：第7课：形态学
- **易混义或误译**：结构元素不是卷积核的同义词
- **英文例句**：getStructuringElement creates a kernel for erosion.
- **中文例句**：getStructuringElement 为腐蚀创建结构元素。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的形态学结构元素；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 形态学结构元素](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1218)

<a id="term-187"></a>
## TERM-187 · distance transform

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：距离变换
- **定义**：为像素计算到最近指定目标像素的距离的变换；OpenCV distanceTransform 以零像素为目标。
- **语境**：第10课：分水岭标记
- **易混义或误译**：目标与距离类型以 API 参数为准
- **英文例句**：distanceTransform measures distance to the nearest zero pixel.
- **中文例句**：distanceTransform 测量到最近零像素的距离。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的distanceTransform 的最近零像素；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 distanceTransform 的最近零像素](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3311)

<a id="term-188"></a>
## TERM-188 · inverse mapping

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：逆映射
- **定义**：从目标位置回查源位置的几何映射方式
- **语境**：第11课：warpPerspective
- **易混义或误译**：矩阵方向须结合 WARP_INVERSE_MAP 选项判断
- **英文例句**：WARP_INVERSE_MAP treats the matrix as a destination-to-source mapping.
- **中文例句**：WARP_INVERSE_MAP 将矩阵作为目标到源的映射。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的warpPerspective 的逆映射；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 warpPerspective 的逆映射](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2137)

<a id="term-189"></a>
## TERM-189 · Hamming distance

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：汉明距离
- **定义**：两个等长二进制串中不同位的数量
- **语境**：第12课：ORB 匹配
- **易混义或误译**：ORB 的 WTA_K 取值可能要求 Hamming2
- **英文例句**：Use Hamming distance for binary descriptors when appropriate.
- **中文例句**：适用时使用汉明距离比较二进制描述子。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的汉明距离的不同位计数；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 汉明距离的不同位计数](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/base.hpp#L283)

<a id="term-190"></a>
## TERM-190 · inlier

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：内点
- **定义**：按模型与阈值判定符合模型的观测数据
- **语境**：第13课：单应估计
- **易混义或误译**：内点不意味着观测一定真实正确
- **英文例句**：RANSAC marks point pairs within the error threshold as inliers.
- **中文例句**：RANSAC 将误差阈值内的对应点标为内点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的findHomography 的内点阈值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 findHomography 的内点阈值](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L734)

<a id="term-191"></a>
## TERM-191 · outlier

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：外点
- **定义**：按模型与阈值判定不符合模型的观测数据
- **语境**：第13课：单应估计
- **易混义或误译**：外点标签取决于模型和阈值
- **英文例句**：The homography mask marks rejected point pairs as outliers.
- **中文例句**：单应矩阵掩膜将被拒绝的对应点标为外点。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的findHomography 的外点阈值；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 findHomography 的外点阈值](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L734)

<a id="term-192"></a>
## TERM-192 · Kalman filter

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：卡尔曼滤波器
- **定义**：利用状态预测和测量更新估计动态系统状态的滤波器
- **语境**：第17课：目标跟踪
- **易混义或误译**：模型和噪声参数选择影响结果
- **英文例句**：KalmanFilter predicts the state and corrects it using a measurement.
- **中文例句**：KalmanFilter 预测状态并用测量进行校正。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的KalmanFilter 预测及测量校正；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 KalmanFilter 预测及测量校正](https://github.com/opencv/opencv/blob/5.0.0/modules/video/include/opencv2/video/tracking.hpp#L522)

<a id="term-193"></a>
## TERM-193 · NCHW

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：批通道高宽布局
- **定义**：按批次、通道、高、宽排列四维数据的约定
- **语境**：第22课：模型输入
- **易混义或误译**：不能仅凭维度大小确定语义
- **英文例句**：blobFromImage returns a blob in NCHW order.
- **中文例句**：blobFromImage 返回 NCHW 顺序的 blob。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的blobFromImage 的 NCHW 输出；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 blobFromImage 的 NCHW 输出](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp#L1272)

<a id="term-194"></a>
## TERM-194 · ReLU

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：修正线性单元
- **定义**：将负输入映射为零并保留正输入的逐元素激活函数
- **语境**：第22课：ONNX 推理
- **易混义或误译**：ReLU 不等于 softmax
- **英文例句**：ReLU maps a negative input to zero.
- **中文例句**：ReLU 将负输入映射为零。
- **版本**：ONNX 算子规范 since_version 13；不改变工程依赖
- **校核状态**：已核对 ONNX Relu 逐元素公式；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[ONNX Relu 算子规范](https://onnx.ai/onnx/operators/onnx__Relu.html)

<a id="term-195"></a>
## TERM-195 · residual

- **词性**：名词
- **领域**：课程算法与数据
- **推荐译法**：残差
- **定义**：观测值与模型预测值之间的差；具体差值方向应明确约定。
- **语境**：第14课：重投影
- **易混义或误译**：残差大小依所选单位和模型定义
- **英文例句**：The residual compares an observed point with its projection.
- **中文例句**：残差比较观测点与其投影。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对固定源码的标定重投影预测与观测差；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 标定重投影预测与观测差](https://github.com/opencv/opencv/blob/5.0.0/modules/calib/include/opencv2/calib.hpp#L663)

<a id="term-196"></a>
## TERM-196 · backpressure

- **词性**：名词
- **领域**：有界并发
- **推荐译法**：背压
- **定义**：下游处理能力不足时向上游反馈需求或限制以约束数据流速和缓冲。
- **语境**：第25课：有界帧队列
- **易混义或误译**：不能降速的来源可能需要丢帧或其他明确策略；背压不等同于无限缓存。
- **英文例句**：A bounded queue can apply backpressure when the consumer is slower.
- **中文例句**：消费者较慢时，有界队列可以施加背压。
- **版本**：Reactive Streams 规范，JVM 语境作为概念参考
- **校核状态**：已核对 Reactive Streams 有界缓存与需求控制说明；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[Reactive Streams 有界缓存与背压](https://github.com/reactive-streams/reactive-streams-jvm#specification)

<a id="term-197"></a>
## TERM-197 · saturation

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：饱和裁剪
- **定义**：超出目标类型可表示范围时限制到边界；OpenCV saturate_cast 对 8 位或 16 位整数目标执行裁剪。
- **语境**：第4课：图像算术
- **易混义或误译**：32 位整数目标不执行该裁剪；其他运算需按各接口契约判断。
- **英文例句**：saturate_cast converts a negative integer to zero for uchar.
- **中文例句**：saturate_cast 将负整数转为 uchar 时限制为零。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 固定源码的saturate_cast 的 8、16 位目标裁剪；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 saturation](https://github.com/opencv/opencv/blob/5.0.0/modules/core/include/opencv2/core/saturate.hpp#L60)

<a id="term-198"></a>
## TERM-198 · histogram bin

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：直方图分箱
- **定义**：直方图中对应指定数值区间的计数单元
- **语境**：第4课：统计灰度
- **易混义或误译**：箱的范围与边界包含方式需明确
- **英文例句**：The histogram bin counts values within its interval.
- **中文例句**：直方图分箱统计其区间内的数值。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 固定源码的calcHist 的分箱与区间；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 histogram bin](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2973)

<a id="term-199"></a>
## TERM-199 · border extrapolation

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：边界外推
- **定义**：为局部图像运算需要的图像外位置确定取值的方法
- **语境**：第6课：滤波边界
- **易混义或误译**：不同边界模式会产生不同结果
- **英文例句**：The filter uses a border mode for pixels outside the image.
- **中文例句**：滤波器对图像外像素使用边界模式。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 固定源码的sepFilter2D 的边界外推；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 border extrapolation](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L1453)

<a id="term-200"></a>
## TERM-200 · contour hierarchy

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：轮廓层级
- **定义**：轮廓间父子和相邻关系的表示
- **语境**：第9课：轮廓检索
- **易混义或误译**：层级关系依轮廓检索模式而定
- **英文例句**：findContours can return the hierarchy of contours.
- **中文例句**：findContours 可以返回轮廓层级。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 固定源码的findContours 的层级输出；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 contour hierarchy](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3747)

<a id="term-201"></a>
## TERM-201 · watershed marker

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：分水岭标记
- **定义**：标识不同初始区域并供分水岭算法使用的标签图
- **语境**：第10课：标记分水岭
- **易混义或误译**：未知区域和边界标签需遵循 API 约定
- **英文例句**：Watershed uses the marker labels as initial regions.
- **中文例句**：分水岭使用标记标签作为初始区域。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 固定源码的watershed 的输入标记和边界；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5.0.0 watershed marker](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3191)

<a id="term-202"></a>
## TERM-202 · affine transform

- **词性**：名词
- **领域**：课程概念
- **推荐译法**：仿射变换
- **定义**：由线性映射与平移构成的二维变换；warpAffine 使用 2×3 矩阵表示。
- **语境**：第11课：几何变换
- **易混义或误译**：一般非退化仿射变换保持共线性与同一直线上的比例，不能表达一般透视变换。
- **英文例句**：warpAffine transforms an image with a 2 by 3 matrix.
- **中文例句**：warpAffine 使用 2×3 矩阵变换图像。
- **版本**：OpenCV 5.0.0
- **校核状态**：已核对 OpenCV 5 warpAffine 变换公式；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 5 warpAffine 变换公式](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L2100)

<a id="term-203"></a>
## TERM-203 · non-maximum suppression

- **词性**：名词
- **领域**：边缘检测
- **推荐译法**：非极大值抑制
- **定义**：沿梯度方向保留局部极大响应以细化边缘。
- **语境**：第8课：Canny 边缘
- **易混义或误译**：与检测框按 IoU 筛除重叠候选的 NMS 分义不同。
- **英文例句**：Non-maximum suppression thins gradient responses along the edge.
- **中文例句**：非极大值抑制细化边缘附近的梯度响应。
- **版本**：OpenCV4.13 Canny概念资料；工程仍锁定5.0.0
- **校核状态**：已核对官方文档相关定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 Canny 教程](https://docs.opencv.org/4.13.0/da/d22/tutorial_py_canny.html)

<a id="term-204"></a>
## TERM-204 · hysteresis thresholding

- **词性**：名词
- **领域**：边缘检测
- **推荐译法**：滞后阈值连接
- **定义**：依据双阈值及与强边缘的连通性保留弱边缘。
- **语境**：第8课：Canny 双阈值
- **易混义或误译**：阈值间像素不会全部保留。
- **英文例句**：Hysteresis keeps weak edges connected to strong edges.
- **中文例句**：滞后阈值保留与强边缘连通的弱边缘。
- **版本**：OpenCV4.13 Canny概念资料；工程仍锁定5.0.0
- **校核状态**：已核对官方文档相关定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[OpenCV 4.13 Canny 教程](https://docs.opencv.org/4.13.0/da/d22/tutorial_py_canny.html)

<a id="term-205"></a>
## TERM-205 · false positive

- **词性**：名词
- **领域**：分类评价
- **推荐译法**：假阳性
- **定义**：实际为负而被预测为正的样本。
- **语境**：第20课：缺陷误报
- **易混义或误译**：正类含义由任务指定。
- **英文例句**：A normal part flagged as defective is a false positive.
- **中文例句**：正常零件被判为缺陷属于假阳性。
- **版本**：scikit-learn官方评价概念文档，访问2026-10-30
- **校核状态**：已核对官方文档相关定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[scikit-learn 混淆矩阵](https://scikit-learn.org/stable/modules/model_evaluation.html#confusion-matrix)

<a id="term-206"></a>
## TERM-206 · false negative

- **词性**：名词
- **领域**：分类评价
- **推荐译法**：假阴性
- **定义**：实际为正而被预测为负的样本。
- **语境**：第20课：缺陷漏报
- **易混义或误译**：不能把漏报与误报混淆。
- **英文例句**：A defective part classified as normal is a false negative.
- **中文例句**：缺陷零件被判为正常属于假阴性。
- **版本**：scikit-learn官方评价概念文档，访问2026-10-30
- **校核状态**：已核对官方文档相关定义；中文为项目推荐译法，例句自编
- **核对日期**：2026-10-30
- **来源**：[scikit-learn 混淆矩阵](https://scikit-learn.org/stable/modules/model_evaluation.html#confusion-matrix)

<a id="term-207"></a>
## TERM-207 · cumulative distribution function

- **词性**：名词
- **领域**：图像统计
- **推荐译法**：累积分布函数
- **定义**：离散强度不大于给定值的概率，由归一化直方图累加得到。
- **语境**：第4课：F(k)=sum_{i<=k}h(i)/N。
- **易混义或误译**：未归一化累计计数终值为N，概率CDF终值为1。
- **英文例句**：The normalized CDF ends at one.
- **中文例句**：归一化CDF的终值为1。
- **版本**：scikit-image0.25.2概念资料
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[scikit-image cumulative_distribution](https://scikit-image.org/docs/0.25.x/api/skimage.exposure.html#skimage.exposure.cumulative_distribution)

<a id="term-208"></a>
## TERM-208 · dynamic range

- **词性**：名词
- **领域**：图像表示
- **推荐译法**：动态范围
- **定义**：指定成像或表示约定下可分辨的信号范围，需区分采集系统能力和当前图像占用区间。
- **语境**：第4课：强度拉伸不能恢复采集时丢失的信号。
- **易混义或误译**：8位存储有256级不等于传感器有效动态范围。
- **英文例句**：Rescaling intensities does not recover saturated detail.
- **中文例句**：强度重映射不能恢复饱和细节。
- **版本**：EMVA1288 Release4.0
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[EMVA1288信号范围](https://www.emva.org/wp-content/uploads/EMVA1288General_4.0Release.pdf)

<a id="term-209"></a>
## TERM-209 · foreground

- **词性**：名词
- **领域**：图像分割
- **推荐译法**：前景
- **定义**：由分割任务指定的目标区域，二值掩模通常以非零表示。
- **语境**：第5课：暗目标可使用反向阈值。
- **易混义或误译**：前景不总是原图亮区域，分水岭未知区另用0标记。
- **英文例句**：The foreground mask selects the dark objects.
- **中文例句**：前景掩模选出暗色目标。
- **版本**：OpenCV4.13概念对照
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV标记分水岭](https://docs.opencv.org/4.13.0/d3/db4/tutorial_py_watershed.html)

<a id="term-210"></a>
## TERM-210 · oversegmentation

- **词性**：名词
- **领域**：图像分割
- **推荐译法**：过分割
- **定义**：一个期望对象被拆成过多区域的分割现象。
- **语境**：第10课：噪声局部极值生成多余分水岭区域。
- **易混义或误译**：与多个对象合并成一个区域不同。
- **英文例句**：Noisy minima can cause oversegmentation.
- **中文例句**：含噪局部极小值可能导致过分割。
- **版本**：OpenCV4.13概念对照
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV分水岭教程](https://docs.opencv.org/4.13.0/d3/db4/tutorial_py_watershed.html)

<a id="term-211"></a>
## TERM-211 · normalized cross-correlation

- **词性**：名词
- **领域**：模板匹配
- **推荐译法**：归一化互相关
- **定义**：用能量归一化的块间相关度；本课程NCC特指先去均值的版本。
- **语境**：第12课：TM_CCOEFF_NORMED先去均值再归一化。
- **易混义或误译**：TM_CCORR_NORMED不去均值；零方差应先拒绝。
- **英文例句**：Mean-centered NCC is unchanged by an additive offset without clipping.
- **中文例句**：未截断时，去均值NCC不受加性偏移影响。
- **版本**：OpenCV5.0.0
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV5模板匹配公式](https://github.com/opencv/opencv/blob/5.0.0/modules/imgproc/include/opencv2/imgproc.hpp#L3621)

<a id="term-212"></a>
## TERM-212 · ambiguity

- **词性**：名词
- **领域**：特征匹配
- **推荐译法**：歧义
- **定义**：多个候选具有相近证据，无法可靠确定唯一对应的情形。
- **语境**：第12课：重复纹理的最佳与次佳距离相近。
- **易混义或误译**：比率测试仅过滤候选，不证明几何对应唯一。
- **英文例句**：Repeated patterns make the match ambiguous.
- **中文例句**：重复图案使匹配存在歧义。
- **版本**：OpenCV4.13概念对照
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV匹配比率测试](https://docs.opencv.org/4.13.0/d1/de0/tutorial_py_feature_homography.html)

<a id="term-213"></a>
## TERM-213 · seam

- **词性**：名词
- **领域**：图像拼接
- **推荐译法**：接缝
- **定义**：重叠图像组合时选择的区域分界，接缝估计与后续融合是不同步骤。
- **语境**：第13课：配准之后处理接缝。
- **易混义或误译**：单应估计成功不等于曝光一致或接缝不可见。
- **英文例句**：Seam estimation follows geometric alignment.
- **中文例句**：接缝估计在几何对齐之后进行。
- **版本**：OpenCV4.13概念资料，未加入stitching构建
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV stitching pipeline](https://docs.opencv.org/4.13.0/d1/d46/group__stitching.html)

<a id="term-214"></a>
## TERM-214 · contract

- **词性**：名词
- **领域**：API与模型接口
- **推荐译法**：契约
- **定义**：调用双方约定的输入前提、输出含义和有效条件。
- **语境**：第14至30课：维度、类型、单位、名字、所有权和错误策略。
- **易混义或误译**：不指商业合同；不意味着C++20有语言级contract语法。
- **英文例句**：The model contract specifies RGB float32 input.
- **中文例句**：模型契约规定输入为RGB float32。
- **版本**：OpenCV5.0.0；课程工程用语
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV5 Net::setInput输入约定](https://github.com/opencv/opencv/blob/5.0.0/modules/dnn/include/opencv2/dnn/dnn.hpp)

<a id="term-215"></a>
## TERM-215 · degeneracy

- **词性**：名词
- **领域**：几何估计
- **推荐译法**：退化
- **定义**：观测配置无法充分约束所求模型或令解不稳定的情形。
- **语境**：第14课：标定视图不足；单应共线对应点需拒绝。
- **易混义或误译**：数值函数返回矩阵不证明物理可观测性。
- **英文例句**：Collinear correspondences are degenerate for homography estimation.
- **中文例句**：共线对应点对单应估计构成退化配置。
- **版本**：OpenCV5.0.0
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[OpenCV5共线子集处理](https://github.com/opencv/opencv/blob/5.0.0/modules/geometry/include/opencv2/geometry/3d.hpp#L770)

<a id="term-216"></a>
## TERM-216 · flat-field correction

- **词性**：名词
- **领域**：工业成像
- **推荐译法**：平场校正
- **定义**：以均匀亮场和暗场观测补偿空间响应与偏置不均匀的方法。
- **语境**：第18课：在线性未饱和模型中估计增益和偏置。
- **易混义或误译**：需受控均匀照明；近零增益与饱和应拒绝。
- **英文例句**：Flat-field correction uses bright and dark reference frames.
- **中文例句**：平场校正使用亮场与暗场参考帧。
- **版本**：Basler在线文档，访问2026-11-02
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[Basler Flat-Field Correction](https://docs.baslerweb.com/flat-field-correction)

<a id="term-217"></a>
## TERM-217 · exposure time

- **词性**：名词
- **领域**：工业成像
- **推荐译法**：曝光时间
- **定义**：像素在一次成像中积累光生信号的时间间隔。
- **语境**：第18课：影响信号量、运动模糊和帧周期约束。
- **易混义或误译**：不等于帧间隔，也不总是全图同时曝光。
- **英文例句**：A longer exposure can increase motion blur.
- **中文例句**：更长曝光可能增加运动模糊。
- **版本**：EMVA1288 Release4.0
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[EMVA1288曝光与暗信号](https://www.emva.org/wp-content/uploads/EMVA1288General_4.0Release.pdf)

<a id="term-218"></a>
## TERM-218 · validation set

- **词性**：名词
- **领域**：机器学习评价
- **推荐译法**：验证集
- **定义**：用于选择模型或超参数的数据子集，需与最终测试集区分。
- **语境**：第21课：同一工件的连拍按组切分。
- **易混义或误译**：反复按测试集调阈值会污染独立评价。
- **英文例句**：Use the validation set to select the threshold.
- **中文例句**：使用验证集选择阈值。
- **版本**：scikit-learn在线文档，访问2026-11-02
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[scikit-learn交叉验证](https://scikit-learn.org/stable/modules/cross_validation.html)

<a id="term-219"></a>
## TERM-219 · test set

- **词性**：名词
- **领域**：机器学习评价
- **推荐译法**：测试集
- **定义**：模型及选择规则固定后用于最终评价的留出数据。
- **语境**：第21课：独立工件或批次，归一化参数来自训练集。
- **易混义或误译**：测试样本不参与训练、标准化估计或阈值选择。
- **英文例句**：Evaluate the frozen model on the held-out test set.
- **中文例句**：在留出的测试集上评价固定模型。
- **版本**：scikit-learn在线文档，访问2026-11-02
- **校核状态**：已核对所列一手资料及课程语境；中文为项目推荐译法，例句自编
- **核对日期**：2026-11-02
- **来源**：[scikit-learn交叉验证](https://scikit-learn.org/stable/modules/cross_validation.html)
