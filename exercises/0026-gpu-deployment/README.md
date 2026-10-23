# GPU成本模型与Windows部署：独立练习

编写部署清单：Release程序、QtCore/Gui/Widgets、platforms/qwindows.dll、OpenCV实际导入DLL和MSVC运行时。在无开发环境Windows虚拟机运行环境探针和26实验；用依赖检查器确认x64且没有Debug DLL。若要GPU扩展，另建可选目标并记录驱动、设备、OpenCV构建选项与CPU回退。

## 提交证据

提交独立源码、输入生成规则、运行命令、数值表、PNG和失败案例解释。不得复制演示后仅改变量名。

## 评分

CPU输出正确20分、明确标记未验证GPU20分、部署依赖及位数20分、干净机记录20分、缺依赖和缺文件失败可见20分。GPU加速只作为额外实验，不计入离线必修通过条件。

完整答案见 ../../solutions/0026-gpu-deployment/README.md。自动演示通过不记录为掌握。
