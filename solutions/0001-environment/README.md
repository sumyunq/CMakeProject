# 01 环境与验证参考解答

先独立记录环境，再用本页核对。不需要把报告发回会话才能继续。

## 构建和版本证据

在仓库根执行 `./scripts/Build.ps1 -Configuration Debug` 和 Release；各自运行 environment_probe。Qt、OpenCV、C++标准、架构、编译器应与README基线一致。把实际输出存入自己的报告；不要把本页的版本文字复制为实测输出。

头文件在编译期提供声明；导入库在链接期解析DLL接口；DLL在运行时加载。编译成功不证明运行时加载了正确DLL。本机预设选择安装前缀和DLL搜索路径；系统全局PATH不应为了此练习修改。

## 三种失败的判断

configure报找不到OpenCVConfig.cmake：检查安装和OpenCV_DIR。链接报unresolved external symbol：检查目标是否链接opencv_core等相应库及Debug/Release一致性。启动报缺DLL：检查运行脚本或部署目录，而不是改源代码include。

## 安全移除DLL实验

先运行Deploy.ps1，记录它输出的独立部署目录。复制该目录到自己新建的测试文件夹，关闭测试程序后，仅在该副本把一个opencv_core DLL改成`.disabled`后缀，执行副本environment_probe，记录加载错误。恢复文件名后再次成功运行。不要删除或重命名Qt/OpenCV原安装中的文件，也不要从其他版本拷DLL补洞。

## 验收

报告包含两种配置输出、各依赖实际路径、三类失败解释、临时部署副本失败及恢复证据。干净PATH启动能证明本机副本依赖较完整，不能证明另一台电脑、另一系统或所有插件均可用。
