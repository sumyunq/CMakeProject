# 有界并发、取消和Qt线程边界：完整参考解答

## 解题思路与实现

参考架构：生产者在condition_variable_any.wait(lock,stop,predicate)等待容量；消费者等待done或非空。done由生产者完成后持锁设置并notify_all。Qt界面层通过Worker QObject moveToThread，connect(worker,&Worker::result,window,&Window::showResult,Qt::QueuedConnection)；FrameResult用Q_DECLARE_METATYPE及qRegisterMetaType。工作线程只读参数快照，界面不共享可变Mat；finished连接thread quit与worker deleteLater，等待线程退出后释放线程对象。取消长循环必须每块检查原子/stop_token，排队的取消槽无法打断正在阻塞的槽。

## 数值推导

队列长度Q(t)=Q(0)+(生产率-消费率)t。背压让生产者在Q达到容量时等待，把内存上界固定为capacity×frameBytes；代价是入口吞吐受慢阶段约束。实时预览也可丢旧帧但必须公开丢帧计数；检测存档不能静默丢。条件变量等待谓词必须在持锁状态检查，以应对虚假唤醒。

## 可运行基准答案

`pwsh -File scripts/RunLab.ps1 -Lesson 25`，或 `engineering_labs 25 artifacts/lesson25`。共享完整实现位于 `src/app/EngineeringLabs.cpp` 中对应函数；程序检查每张PNG写读并校验数值，任何契约失败非零退出。

25运行容量2的std::jthread队列，生产整数0..19，消费总和190，最大长度≤2，结束后队列空。第二个线程专门等待永假谓词，request_stop必须唤醒它并join成功。25-input.png验证输出通道可用。这里处理整数是为了把线程调度与图像算法隔离；工程传Mat时需独占clone或不可变共享协议。

## 独立扩展的实施顺序

把队列元素改成含frameId、时间戳、独占Mat的结构。对20帧做threshold，消费者求白像素总和14400。增加取消按钮的业务入口：停止接收新任务、请求停止、唤醒所有等待者、等待结束，再销毁状态。必须分别测试队列空和队列满时取消。

## 逐项验收

容量断言、顺序与sum、空/满取消、重复启动停止、GUI显式QueuedConnection各20分。合格≥80；死锁、后台操作GUI或无界队列直接不合格。

## 自测详解

1. notify能替代谓词吗？不能，通知可能先于等待。2. jthread析构请求停止是否保证业务退出？不保证，业务必须响应token。3. 为什么不能直接把Mat裸data排队？发送方可能已释放或重写；必须建立稳定所有权。

## 验证边界

默认实现仅验证离线算法和工程契约。第28—30课的Qt界面、真实工件与真实DNN部分为独立项目要求，必须按上述完整架构和里程碑实施并提供交互及数据证据；当前命令行实验不声称替代这些验证。

## 官方依据

- https://docs.opencv.org/4.13.0/d4/d86/group__imgproc__filter.html
- https://doc.qt.io/qt-6.8/qthread.html
- https://github.com/opencv/opencv/tree/5.0.0

上述在线概念文档不是锁定二进制版本说明；以5.0.0源码与本机6.8.3实际编译运行结果为准。
