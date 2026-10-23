# OpenCV 学习工程协作约定

## 学习与环境
- 完整路线与30课内容一次提供，自主学习不等待会话解锁；面向已掌握 Qt Widgets 的学习者。覆盖图像、工业视觉、视频、AI、性能与源码。
- Qt **6.8.3** msvc2022_64；OpenCV **5.0.0** 官方源码；C++20；MSVC v143 **14.44.35207** x64；CMake **3.30.5**；Ninja **1.12.1**。不得静默替换版本。
- 示例与独立练习分开；不能代做所有练习，不能把自动测试通过记为学习者掌握。
- 每次修改批次前更新 `docs/seession_context.md`；压缩上下文后完整重读。保留此拼写。
- 重大范围、版本或方案冲突向用户报告；已确认的常规实现不反复索取授权。

## 代码
- 保持 src/app、presentation、business、infrastructure、core 分层；仅 app/presentation 依赖 QtGui/Widgets。跨层具体实现通过接口注入，首批不制造空抽象。
- 普通类 .h/.cpp 分离；头文件自包含、`#pragma once`；指针/引用优先前向声明；对应头文件作为 .cpp 第一个 include。
- 自有代码禁止裸 new/delete。QObject 所有权不与智能指针混管；uic 生成的有 parent 控件由 Qt 对象树管理。普通类无需 Q_OBJECT。
- 类 PascalCase，函数 camelCase，成员 cl_，指针成员 clp_，常量 kPascalCase，枚举项 kValue。禁止头文件 using namespace。
- 标准库、Qt、第三方、项目 include 分组；字段赋值表达含义，避免位置相关聚合初始化。所有控制分支加大括号。
- 所有 I/O 和外部调用检查失败；信号槽边界处理异常。资源错误可见；路径用跨平台 API。
- 跨线程连接显式 QueuedConnection，后台不操作 GUI；自定义跨线程数据声明并注册元类型。
- .ui 只保留布局；QSS 放独立资源文件，C++ 初始化加载；以 objectName 限定选择器。connect 集中在初始化函数，禁用自动 on_object_signal 槽。
- 状态唯一来源是业务模型，UI 仅编辑/展示。用户可见文本使用 tr()，内部常量使用 QStringLiteral。
- Doxygen 以 docs/conventions/doxygen.md 为准：头文件函数写完整文档块并覆盖具名参数/非 void 返回值，字段和枚举项用行尾 ///<；CPP 定义前用 /// \brief，函数体内用 //。只按实际实现补所有权/线程约束；纯注释提交不得夹带代码、签名或格式变更。修改记录用实际北京时间及 wbliu 标识。
- Debug 第一方函数用作用域进出追踪；Release 无函数追踪，保留关键事件和脱敏错误。日志异步输出到可执行文件旁 logs，保留 7 天，退出排空。

## 构建、测试与交付
- 新增源文件立即显式加入 CMake，禁止 GLOB。目标级设置、C++20、MSVC /W4 /permissive- /Zc:__cplusplus；自有代码警告视为错误。
- 新增接口有正向及边界测试；测试命名 action_condition_expectedResult；表驱动测试使用 QTest 数据；文件测试使用临时目录。
- 修改后运行 wbliu-code-check；若当前环境没有工作流，明确报告并按本规范自检。不自动运行 qt-cpp-review（含 qt-cpp-view）。
- 每批完成有可复现验证记录；验收后本地中文 Conventional Commit，不自动推送。合并外部代码前先保存提交。
- 外部习惯来源《AIAgent使用习惯记录.mm》只读；其中样式分歧已裁定为独立 QSS + C++ 加载。
