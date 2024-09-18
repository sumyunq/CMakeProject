
cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.10)

project(wbliu)

# 指定生成可执行文件
add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cc)

# 包含子目录
add_subdirectory(src)

# 头文件包含路径
include_directories(${PROJECT_SOURCE_DIR}/include)






