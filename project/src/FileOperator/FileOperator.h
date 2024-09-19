#ifndef __FILEOPERATOR_H_
#define __FILEOPERATOR_H_

#include <stdlib.h>

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
namespace fs = std::filesystem;

// 以标准IO为基础，构造 普通文件 操作对象
class FileOperator_C_TYPE {
 private:
  FILE* fd;

 public:
  FileOperator_C_TYPE(const string pathname, const string mode);
  FileOperator_C_TYPE(const FileOperator_C_TYPE& other) =
      delete;  // 禁止拷贝构造
  FileOperator_C_TYPE(FileOperator_C_TYPE&& other) = delete;  // 禁止移动构造
  FileOperator_C_TYPE& operator=(FileOperator_C_TYPE&& other) =
      delete;  // 禁止赋值构造

  // 常规IO操作
  void read_n_char(char* buff, int n);  // 从光标处开始读取 n 个字节
  void write_n_char(char* buff, const int n);  // 从光标处开始写入 n 个字节
  void copy_to(const string pathname);  // 将文件内容拷贝到其它文件

  void close();  // 关闭描述符
  ~FileOperator_C_TYPE();
};

// 以标准IO为基础，构造 二进制 文件操作对象
template <typename T>
class FileOperator_Bin_TYPE {
 private:
  FILE* fd;

 public:
  FileOperator_Bin_TYPE(const FileOperator_Bin_TYPE& other) =
      delete;  // 禁止拷贝构造
  FileOperator_Bin_TYPE(FileOperator_Bin_TYPE&& other) =
      delete;  // 禁止移动构造
  FileOperator_Bin_TYPE& operator=(FileOperator_Bin_TYPE&& other) =
      delete;  // 禁止赋值构造

  FileOperator_Bin_TYPE(const string pathname, const string mode) {
    this->fd = fopen(pathname.c_str(), mode.c_str());
    if (this->fd == NULL) cerr << "无法打开文件,错误码:" << errno << endl;
  }

  // 二进制 IO操作
  void read_n_char(T* buff, int n) {
    int ret = fread(buff, sizeof(buff[0]), n, this->fd);
    if (ret < n && ferror(this->fd)) {
      cerr << "读取失败，错误码：" << errno << endl;
    } else if (ret < n) {
      cerr << "读取到文件末尾，共读取 " << ret << " 个对象" << endl;
    } else {
      cout << "成功读取 " << n << " 个对象!" << endl;
    }
    
  }

  void write_n_char(T* buff, const int n) {
    int ret = fwrite(buff, sizeof(buff[0]), n, this->fd);
    if (ret < n) {
      cerr << "写入失败，只写入了 " << ret << " 个对象，错误码：" << errno
           << endl;
    } else {
      cout << "成功写入 " << n << " 个对象!" << endl;
    }
  }

  void copy_to(const string pathname)  // 将文件内容拷贝到其它文件
  {
    FILE* fw = fopen(pathname.c_str(), "wb+");
    if (fw == NULL) cerr << "无法打开文件,错误码:" << errno << endl;

    T buff;
    int ret;
    int i = 0;
    while (1) {
      ret = fread(&buff, sizeof(buff), 1, this->fd);
      if (ret == -1) {
        if (ret == feof(this->fd))
          cout << "读取完毕" << endl;
        else
          cerr << "读取失败" << errno;
        break;
      }
      i++;
      ret = fwrite(&buff, sizeof(buff), 1, fw);
    }
    cout << "拷贝完毕，共计拷贝 " << i << " 个 T 类型的二进制数据" << endl;
  }

  void fseek_to_head()  // 光标定位文件开头的位置
  {
    fseek(this->fd, 0, SEEK_SET);
  }

  ~FileOperator_Bin_TYPE() {
    if (this->fd != nullptr) fclose(this->fd);
  }
};

// 统计指定目录下各类文件的数量

#endif  // !__FILEOPERATOR_H_