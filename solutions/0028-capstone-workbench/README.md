# 综合项目一：可复现图像处理工作台：完整参考解答

## 解题思路与实现

参考答案架构：presentation MainWindow只提交Parameters并显示Result；business ProcessingService输入不可变Mat快照，返回mask、对象列表及参数版本；infrastructure ImageRepository读写并检查错误；core负责日志。里程碑一先做纯业务及A-D测试；二用Designer布局双QLabel+QTableView+QSpinBox；三集中connect并显式队列传结果；四版本号过滤过期任务；五导出参数JSON与PNG，再做干净机部署。现有vision_viewer可作Widgets入口起点；不要把算法放进paintEvent。

## 数值推导

先以二值分割建立可解释业务契约：mask(x,y)=255当I>100，否则0；8邻域连接形成对象。三块12×20矩形总面积720，连通对象3。阈值250使计数0，是参数失败而非程序崩溃。阈值不是概率，强度变化会导致结果突然跳变，应显示原图和掩模。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 28`，或 `engineering_labs 28 artifacts/lesson28`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

28运行完整离线业务链：生成→threshold→connectedComponents→面积与计数断言→导出三幅PNG。28-input.png、28-mask.png、28-bad-threshold.png分别对应输入、正确结果、错误参数。该目标是可运行算法验收器；另有独立 Qt 工作台参考，分别验证，不把命令行输出冒充GUI验证。

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

运行时使用同一Qt/OpenCV的运行时PATH。此独立示例使用 .ui/QSS 和 qrc；参数保存在模型中，输入图像、阈值、最小面积组成快照交给单个后台线程，待处理槽最多保留一个最新请求。结果带 revision，通过 Qt::QueuedConnection 回到主线程；过期结果丢弃，掩模与 QImage 拥有独立像素。支持连通域表格、参数 JSON 原子保存和重开、PNG 原子导出以及可见错误。Debug/Release 可分别通过脚本构建并运行离屏自检，真实桌面交互需另行检查。

## 进阶验算详解：工作台验收

题目：图中有三块 12×20 矩形，阈值分割后互不接触。最小面积分别取 240、241 时对象数是多少？失败载入及旧任务返回如何处理？

参考计算：每块面积 240，总面积 720；按 area≥minArea 筛选时分别为 3 和 0。失败载入保留上一有效图并提示；每次提交递增 revision，仅接收当前 revision 的结果。导出后重新读取验证尺寸及 countNonZero。

## 面向参考窗口的分步实现和验证

从 `reference/main.cpp` 的可编译基础窗口开始，保持参考程序不进入默认构建。先确认打开图像失败时保留上一幅有效图、阈值 100 时三对象面积 720；把最小面积与阈值整理为独立参数，并将连通区域对象表与总面积从同一个结果计算。将处理拆成纯业务函数，输入持有独立图像快照，输出 mask、对象列表、错误和 revision；边界分别覆盖空图、阈值 0/255、面积 240/241。只在 UI 线程创建 QPixmap，跨线程使用拥有独立内存的图像副本。

参考程序已经使用单工作线程、最新请求槽及版本过滤；取消使未开始的请求和过期结果失效，但不会强行中断正在执行的 OpenCV 调用，关闭时等待该调用结束。PNG 写入通过 QSaveFile 原子提交；导出期间参数、打开及取消控件被禁用并显示“提交期间不可取消”，避免取消与提交的竞态。关闭时会等待正在执行的不可中断 I/O 或 OpenCV 调用。按文件打开、调整参数、导出、重新打开、异常输入和连续改参数的顺序自行录屏；对大图操作再实测界面响应。

参考程序分别保存处理参数 JSON 与掩模 PNG，并检查写入结果；数据来源与版本应在自己的实验记录中补充；文件失败时显示错误而非成功提示。人工验收至少记录 Windows 与锁定依赖版本、图像哈希、交互步骤、对象表、输出文件及失败日志。当前环境未完成真实 GUI 交互或第二台干净机器部署验收。

### 后台结果过滤的代码参考

以下为集成到现有窗口后的伪代码结构，类型注册与 Worker 线程生命周期必须按实际类完成；它不是可单独编译的替代窗口。`result` 应持有独立内存，不引用工作线程即将销毁的临时缓冲区。

```text
// GUI 线程：每次提交都产生新的版本，参数和图像均为快照。
++cl_revision;
Parameters request = currentParameters();
request.revision = cl_revision;
submitProcessing(request, imageSnapshot);

// 初始化连接时指定接收者 context 和排队方式。
connect(worker, &Worker::resultReady, this,
        [this](const Result& result) {
            if (result.revision != cl_revision) {
                return;
            }
            if (!result.error.isEmpty()) {
                showError(result.error);
                return;
            }
            displayResult(result); // 仅在 GUI 线程更新控件
        }, Qt::QueuedConnection);
```

取消当前请求时同样递增版本使在途结果失效，再向 worker 发出协作取消；避免主线程销毁仍被 worker 使用的数据。参考程序采用单消费者和版本判断；进一步扩展时须用可控延迟测试：令版本 1 比版本 2 晚返回，最终界面仍显示版本 2；另测关闭窗口时在途任务和队列资源释放。


工作台最多接受1000个保留连通域；超过上限时保留上一结果并显示错误，用户可提高最小面积或调整阈值。输入文件最大64 MiB，解码后最多1600万像素。过滤采用一次标签查表遍历；取消使用每次请求独立的 stop_token，在可协作的阶段检查。
