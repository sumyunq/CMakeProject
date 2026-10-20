# 官方资料索引

核查日期：2026-10-20。教材以实际 5.0.0 源码和安装后的头文件为准；滚动文档若变化，先核对版本。

## Knowledge
- [spdlog 1.17.0](https://github.com/gabime/spdlog/tree/v1.17.0)：日志后端固定版本；异步队列、source_loc、错误处理和专属线程池见 include/spdlog。
- [spdlog 异步日志](https://github.com/gabime/spdlog/wiki/Asynchronous-logging)：区分异步 flush 请求与真正排空；本工程采用专属线程池，停止不影响其他库。
- [OpenCV 5 文档](https://docs.opencv.org/5.0/)：课程入口与模块组织；查询 API 时核对安装版本。
- [OpenCV 5.0.0 源码](https://github.com/opencv/opencv/tree/5.0.0)：实际行为的首要依据；入门查看 modules/core/include/opencv2/core/mat.hpp 与 modules/core/src/matrix.cpp。
- [4 到 5 迁移指南](https://github.com/opencv/opencv/wiki/OpenCV-4-to-5-migration)：对照旧教程的模块、类型、推理接口差异。
- [OpenCV 构建配置](https://docs.opencv.org/4.x/db/d05/tutorial_config_reference.html)：理解 BUILD_LIST 等构建选项；这是 4.x 资料，选项是否可用必须以 5.0.0 配置结果为准。
- [Qt 6.8 平台支持](https://doc.qt.io/qt-6.8/supported-platforms.html)：核对 Windows、MSVC 与架构组合。
- [Qt 6.8 QImage](https://doc.qt.io/qt-6.8/qimage.html)：像素格式、bytesPerLine、外部内存生命周期及 copy()。
- [Qt 6.8 CMake 入门](https://doc.qt.io/qt-6.8/cmake-get-started.html)：Qt 自动生成工具和目标组织。
- [CMake 3.30 预设](https://cmake.org/cmake/help/v3.30/manual/cmake-presets.7.html)：公共配置与本机配置分离。
- [MSVC 运行库配置](https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library)：理解 /MD、/MDd 及混用风险。

## Wisdom
- [OpenCV 官方论坛](https://forum.opencv.org/)：查找失败案例；结论仍要用固定版本的小实验验证。

## Gaps
- 课程 2 之后尚未编写；各课引入新领域时追加原始论文、官方算法文档与数据来源。
- 尚未评估工业相机、真实缺陷样本或 GPU 环境；不把模拟图像上的结果当作生产精度。
