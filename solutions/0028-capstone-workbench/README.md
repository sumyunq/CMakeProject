# 综合项目一：可复现图像处理工作台：完整参考解答

## 解题思路与实现

参考答案架构：presentation MainWindow只提交Parameters并显示Result；business ProcessingService输入不可变Mat快照，返回mask、对象列表及参数版本；infrastructure ImageRepository读写并检查错误；core负责日志。里程碑一先做纯业务及A-D测试；二用Designer布局双QLabel+QTableView+QSpinBox；三集中connect并显式队列传结果；四版本号过滤过期任务；五导出参数JSON与PNG，再做干净机部署。现有vision_viewer可作Widgets入口起点；不要把算法放进paintEvent。

## 数值推导

先以二值分割建立可解释业务契约：mask(x,y)=255当I>100，否则0；8邻域连接形成对象。三块12×20矩形总面积720，连通对象3。阈值250使计数0，是参数失败而非程序崩溃。阈值不是概率，强度变化会导致结果突然跳变，应显示原图和掩模。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 28`，或 `engineering_labs 28 artifacts/lesson28`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

28运行完整离线业务链：生成→threshold→connectedComponents→面积与计数断言→导出三幅PNG。28-input.png、28-mask.png、28-bad-threshold.png分别对应输入、正确结果、错误参数。该目标是可运行算法验收器；Qt交互扩展按下述里程碑独立完成，不把命令行输出冒充GUI验证。

## 独立扩展的实施顺序

规格：文件打开/保存、阈值0..255、最小面积、原图/结果视图、对象表格、重置、取消和状态栏。数据集A为当前3目标；B为同尺寸全黑；C为两块接触矩形；D为空文件或损坏PNG。A计数3面积720，B计数0，C按8邻域合为1，D报告错误且保持上一有效结果。

## 逐项验收

A-D正确性40分、参数版本与取消20分、界面布局和可见错误20分、导出重开及部署20分。命令行验收只覆盖业务40分；完整项目≥80且必须有真实GUI截图与交互录屏。

## 自测详解

1. 改参数时旧任务晚返回怎么处理？携带递增revision，丢弃过期结果。2. QImage直接包Mat内存能否跨线程延后显示？不能依赖短生命期，界面边界copy。3. 保存失败后能显示成功吗？不能，检查编码及写盘错误。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。

## 工作台数据流与交互验收脚本

业务Parameters包含threshold=100、minArea=1、revision；Result包含revision、mask、objects与error。每次阈值改变递增revision，复制参数后提交后台；结果返回时只接受与当前revision相等者。objects通过connectedComponentsWithStats获取，跳过背景标签0；每行记录left/top/width/height/area。输入三目标时行值依次是(10,30,12,20,240)、(46,30,12,20,240)、(82,30,12,20,240)。

1. 打开28-input.png，阈值100：界面必须显示3行、总面积720。
2. 快速切换阈值250→100：最后显示3，旧的0结果不得覆盖。
3. 阈值100、最小面积241：显示0；改回240显示3。
4. 保存后重开mask，countNonZero=720且PNG尺寸128×96。
5. 加载不存在文件：明确提示错误，不清空当前有效图。

最小业务参考算法：threshold后connectedComponentsWithStats；遍历标签1..N-1，读取CC_STAT_AREA并按minArea筛选，保留各标签外框。结果Mat作为独立持有值返回；UI QImage使用copy后转QPixmap，仅在主线程执行。应用层负责依赖注入，不能让业务类include QWidget。

交互项目构建入口沿用vision_viewer的CMake组织，在presentation目标显式追加窗口/ui，在business追加ProcessingService；QtCore可用于业务结果元类型，QtWidgets只在presentation。当前engineering_labs 28仅是此项目的离线业务参考答案，不是可视窗口。

## 可直接构建的Qt基础窗口

`reference/main.cpp`和`reference/CMakeLists.txt`是完整独立程序：默认生成三目标，阈值框控制结果，原图/掩模并排，显示数量/面积，支持Unicode文件打开和PNG导出；所有槽边界捕获异常。窗口和控件使用栈生命周期，父对象最后析构，避免裸new。

在仓库锁定的MSVC环境执行（把两个目录替换成CMakeUserPresets.json中的实际值）：

```powershell
./scripts/BuildWorkbenchReference.ps1
./scripts/BuildWorkbenchReference.ps1 -Run
```

运行时使用同一Qt/OpenCV的运行时PATH。此独立示例已在锁定MSVC 14.44、Qt6.8.3、OpenCV5.0.0环境以Release /W4 /WX编译链接通过；交互仍需验证，不能把engineering_labs测试结果冒充本窗口测试。它覆盖基础载入、参数、显示及导出，刻意仅同步处理小图；大图后台有界取消、对象表与版本丢弃按第25课和上述扩展验收实现。当前默认窗口的参数变化同步完成，不存在过期结果竞争；后台版本必须为每次提交与结果增加revision并在界面比较后接受。
