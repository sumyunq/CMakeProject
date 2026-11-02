# 真实图像、衍生视频与 ONNX 推理实验

本页给30课提供实际照片、显微图、视频编解码和真实分类模型的可运行参考。输入图片是实拍数据；视频运动与圆形缺陷由程序人为构造，不是实拍运动或真实工业缺陷。所有文件来源、许可证、SHA-256、标注及尺寸见 [样本清单](../tests/fixtures/real/manifest.json) 与 [标注](../tests/fixtures/real/annotations.json)。

## 准备与离线运行

需要本机已完成 OpenCV 5.0.0 Full Release 构建及 Qt 6.8.3 预设配置；完整准备步骤见 [干净Windows指南](clean-windows.md)。仓库自带 512×512 RGB astronaut、550×660 灰度 cell、12 帧 MJPEG 视频和标注；检查文件未损坏：

```powershell
python scripts/VerifyAssets.py
python tests/scripts/TestVerifyAssets.py
./scripts/BuildNativeLabs.ps1 -Configuration Release
```

基础运行不下载模型，图片/视频可离线执行。真实模型首次需要网络：

```powershell
./scripts/PrepareRealModel.ps1
./scripts/BuildNativeLabs.ps1 -Configuration Release -WithModel
```

脚本只写入忽略提交的 `build/models`，校验 SHA-256；下载或校验失败立即报错。模型按 ONNX Model Zoo 固定提交 `b1eeaa1ac722dcc1cd1a8284bde34393dab61c3d` 获取，大小 4,956,208 字节，SHA-256 `1eeff551a67ae8d565ca33b572fc4b66e3ef357b0eb2863bb9ff47a918cc4088`，来源的 SqueezeNet 页面注明 Apache 2.0。模型是分类器，不产生检测框或分割掩模。[模型来源与输入定义](https://github.com/onnx/models/blob/b1eeaa1ac722dcc1cd1a8284bde34393dab61c3d/validated/vision/classification/squeezenet/README.md)

运行后 `build/verification/native-tests.log` 是 CTest 结果，`build/native-release/real-output` 保存 PNG 与 `video-ai.csv`。执行以下命令可查看真实实验的中间数值；CTest 自带正确 DLL 搜索路径：

```powershell
$tools = (Get-Content CMakeUserPresets.json -Raw | ConvertFrom-Json).vendor.cvlab
$ctest = Join-Path (Split-Path $tools.cmake) ctest.exe
& $ctest --test-dir build/native-release -V -R '^(real_|native_calibration)'
```

命令从本机预设读取 ctest 路径，不修改全局 PATH。

## 图片读写与失败案例

`real_image` 检查真实 RGB 图以 BGR 三通道读取、显微图以单通道读取，写 PNG 后读回并逐像素比较。期望原图 512×512、灰度图 550×660，PNG 最大像素差为 0；缺失文件必须明确拒绝。输入和输出文件不一样大时，比较前先查通道、尺寸和编码是否匹配。

## 真实纹理配准

`real_registration` 将 astronaut 的灰度图人为平移 (7,5) 像素，提取 ORB，Hamming 两近邻比率筛选，再用 RANSAC 估计单应。人工已知变换用于验收：至少20个内点；四个验证点的最大误差小于2像素。示例本机测得 797 个候选匹配、756 个内点、最大误差 0.492 像素；特征数量可因CPU实现变化，不把它当硬编码真值。输出 `astronaut-shifted.png`。

练习：替换成带旋转的仿射变换，分别报告内点率与留出点重投影误差；再限制特征只集中在一个小区域，观察外推误差。重复纹理、强遮挡或近共线点不保证求解成功；空描述子与内点不足必须拒绝。

## 多视图相机标定接口

`native_calibration` 使用12种已知倾角和距离、每视图54个平面点，以 `projectPoints` 合成640×480观测，然后执行 `calibrateCamera` 恢复内参。固定 K=(fx=800,fy=820,cx=320,cy=240)，无畸变，约束切向和径向畸变为0；要求重投影 RMS<0.001 px、K 最大元素误差<0.1，空观测必须拒绝。它验证完整标定接口而不依赖摄像头，不是实机标定证据。独立练习加入噪声、不同视图覆盖并留出视图检查误差；真实采集操作见第14课。

## 视频与运动

`astronaut-motion.avi` 由公开照片缩放到320×240后，按第 `n` 帧左上角 `(32+2n,24)` 裁剪256×192得到；12 帧、10 FPS，画面纹理预期位移约(-2,0)像素/帧。`real_video` 使用 OpenCV 内置 MJPEG 实际解码并运行 LK 光流，要求12帧、FPS误差小于0.01、至少20有效特征点且平均位移误差小于0.6像素。本机首对帧为 dx=-2.00047、dy=-0.00787、有效点149。

复现生成时使用 FFmpeg 5.1：

```powershell
ffmpeg -loop 1 -framerate 10 -i tests/fixtures/real/astronaut.png -vf 'scale=320:240,crop=256:192:x=32+2*n:y=24' -frames:v 12 -an -c:v mjpeg -q:v 3 -pix_fmt yuvj420p astronaut-motion.avi
```

不同编解码器版本生成的字节可能不同；仓库内固定文件以清单哈希为准。该序列的纹理取自真实照片，但没有实拍运动、遮挡或时间戳抖动；需要自己拍摄或获取许可明确的视频进一步验证。

## 工业检测参考：真实纹理上的人工缺陷

`real_inspection` 读取真实 cell 图，在标注的圆形区域逐像素改变80级灰度，绝对差阈值为40，再与人工掩模按像素比较。固定输入期望 TP=317、FP=0、FN=0，精确率和召回率均为1；同图对比无前景。输出分别为 `inspection-sample.png`、`inspection-mask.png`、`inspection-truth.png`。

这验证差分管线和指标计算，不验证工业材料、光照、漏检率或量具精度。独立练习：加入照明变化和错位，记录误报，再做配准及局部阈值对比；用独立工件、盲测集和标定尺度验证真实应用。

## SqueezeNet 输入输出与视频 AI

`real_model` 使用随仓库提供的 `imagenet-synset.txt` 输出类别名称（按需通过 `--labels` 指定），将图片拉伸到224×224，BGR→RGB，除255，再按通道减 `[0.485,0.456,0.406]`、除 `[0.229,0.224,0.225]`，输入为 float32 NCHW `[1,3,224,224]`。固定模型在本机 OpenCV 5.0.0 新图引擎输出 `[1,1000]` 分数；astronaut 的前五个类别索引和分数为：823:19.1695、543:19.0708、422:18.8869、503:18.2567、773:17.7489，数值容差0.01。这里直接拉伸，与训练评价时可能使用的裁剪步骤不同；分数未经 softmax，不能当概率或工业精度。

固定样本断言只在 `--verify-fixture` 模式启用；普通模式接受其他有效图片。缺图片、缺模型、模型哈希不符和错误固定样本均必须失败。当前构建已关闭 CUDA/OpenCL，实际在 CPU 环境执行；OpenCV 新图引擎会提示尚不支持 `setPreferableTarget`，不应把该调用当成已验证的 GPU 选择。

`real_video_ai` 对12帧逐帧执行 SqueezeNet 分类，并对相邻帧用 LK 估计整体纹理运动，输出 `video-ai.csv` 中的帧号、最高分索引、原始分数、dx/dy 与有效点数。首帧没有上一帧，运动为0；其余11对平均位移要求约(-2,0)±0.8像素。它是视频分类加纹理光流参考，不具备目标检测框、目标身份跟踪或语义分割。独立练习应自行增加合适的检测模型与标注、数据关联、类别/坐标后处理及误检漏检评价。

## 原始数据与版权

- `astronaut.png`：scikit-image v0.25.2 收录的 NASA Eileen Collins 照片，官方资料说明无已知版权限制、公开领域；用于课堂演示。[资料](https://scikit-image.org/docs/0.25.x/api/skimage.data.html#skimage.data.astronaut)
- `cell.png`：scikit-image v0.25.2 的盐水细胞显微图，官方资料标注 CC0；仅作为灰度真实纹理，非工业缺陷样本。[资料](https://scikit-image.org/docs/0.25.x/api/skimage.data.html#skimage.data.cell)
- 视频是上述公开照片的裁剪衍生；标注为本项目制作。原始源文件、衍生过程和精确哈希都记录在清单内。
- `imagenet-synset.txt` 与 Apache-2.0 许可证文件来自固定提交的 ONNX Model Zoo；类别名称只用于解释输出，不能证明模型判断正确。

完整记录以代码和 `build/verification` 实际日志为准；不同机器、数据、硬件和模型需重新验收。真实相机、GPU和第二台干净 Windows 在本批没有实测。

2026-11-02复验：上述原生测试Debug/Release各20/20；工作台窗口已验证三对象、阈值和重置，完整交互录屏未做。
