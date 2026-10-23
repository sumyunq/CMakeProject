# 综合项目三：视频、AI接口和跟踪验收：完整参考解答

## 解题思路与实现

参考架构与步骤：FrameSource生成含sequenceId和timestamp的Frame；Detector返回frameId、box、score、classId；Tracker用时间差预测，门控距离≤10px，单次缺失保留ID并标predicted，两次以上终止。先完成合成帧断言，再实现有界队列与取消，再加Qt展示，最后单独接DNN。接模型时把预处理尺寸、RGB/BGR、scale、mean、letterbox反变换和NMS阈值作为模型契约存档；dnn/videoio并非当前默认组件，必须显式扩建可选目标。模型许可证、文件哈希和真实标注集随报告交付。

## 数值推导

检测给每帧位置，跟踪在时间上关联。匀速预测x(t+Δt)=x(t)+vΔt；本例x=10+3t，矩形宽10，中心为x+4.5。第4帧x22、第5帧故意缺失、第6帧x28；跨两帧位移6，所以速度仍为3px/frame，不能把6当速度。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 30`，或 `engineering_labs 30 artifacts/lesson30`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

30生成10帧及对应二值掩模，共20张PNG；只有第5帧没有目标，因此检测9次、缺失1次、最终左上x37。fixture-detector使用阈值和外框，不是神经网络推理；目标刻意只依赖core/imgproc/imgcodecs，不需要模型或摄像头。实验验证帧编号和速度恢复契约，不验证DNN精度。

## 独立扩展的实施顺序

项目规格：帧源可切换合成序列/本地视频，Detector接口可切换fixture/真实模型，Tracker输出trackId和状态，界面有暂停、逐帧、取消、置信度与轨迹。最小验收集是本课10帧；扩展集100帧含20次漏检、两个目标交叉和不同尺寸输入。基本集位置误差0、漏检计数1、ID切换0；交叉集必须单独报告ID切换数而不能沿用单目标成绩。

## 逐项验收

基本集9/1/37契约30分、时间戳和ID状态20分、有界取消20分、UI逐帧及报告15分、可选AI真实验证15分。离线基础满分85；AI部分必须提供模型和标注证据，fixture不得冒充已完成AI推理。

## 自测详解

1. 模型300×300坐标能直接画原图吗？不能，要反变换缩放和补边。2. 缺检帧可以隐瞒吗？不能，预测与检测状态必须不同。3. 帧率变化还能用px/frame吗？跨源比较应换成px/s并用时间戳。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。

## 跟踪器的完整状态转换答案

状态只有Absent、Detected、Predicted。初始Absent；首个检测分配ID=1。Detected下一帧有框且与预测中心距离≤10则沿用ID并更新速度；缺框进入Predicted并累加misses；Predicted且misses≤1遇到门内框恢复Detected；连续缺失2次变Absent，再出现分配新ID。边界门值10是合成实验参数，跨帧必须按dt放大或统一用速度门。无效dt≤0应拒绝该帧。

本课帧0..4为Detected，帧5为Predicted（预测左上25、不可标真实检测），帧6恢复Detected左上28，帧9为37；ID始终1。帧0无法估计速度，帧1由(13-10)/(1-0)=3得到速度，帧6跨帧估计(28-22)/(6-4)=3。报告检测计数9、预测1、实际漏检1，不能把预测补框记为模型命中。

每帧报告frame_id,timestamp,backend,detected,track_id,state,left,top,width,height,score。fixture没有真实置信度校准，score固定1只能表示确定性合成规则命中。真实DNN适配器应先在单张图上与官方预处理结果对照，之后接同一Tracker。使用letterbox时(x_net-pad_x)/scale映射回原图，再裁到有效边界；漏掉pad会造成系统偏移。当前构建没有dnn/videoio，因此视频解码和真实推理必须在可选目标中验证，不能在默认实验结果中填“已通过”。
