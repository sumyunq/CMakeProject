#include "FileOperator.h"


//常规 文件

FileOperator_C_TYPE::FileOperator_C_TYPE(const string pathname,
                                         const string mode) {
  this->fd = fopen(pathname.c_str(), mode.c_str());
  if (this->fd == NULL) cerr << "无法打开文件,错误码:" << errno << endl;
}

void FileOperator_C_TYPE::read_n_char(char* buff, int n) {
  strcpy(buff, "");
  int c;
  int i = 0;
  do {
    if (feof(this->fd)) break;
    c = fgetc(this->fd);
    buff[i] = static_cast<char>(c);
    ++i;
    --n;
  } while (n != 0);
}

void FileOperator_C_TYPE::write_n_char(char* buff, const int n) {
  int ret;
  for (int i = 0; i < n; i++) {
    ret = fputc(static_cast<int>(buff[i]), this->fd);
    if (ret < 0) cerr << "写入失败，错误码：" << errno;
  }
}

//注意判断读取文件文件结束的位置：一定要在读取和写入之间
void FileOperator_C_TYPE::copy_to(const string pathname) {
  FILE* fw = fopen(pathname.c_str(), "w+");
  if (fw == NULL) cerr << "无法打开文件,错误码:" << errno << endl;
  int i = 0;
  int ret;
  int c;
  while (1) {
    c = fgetc(this->fd);    //read
    if (c == -1) {
      ret = feof(this->fd);
      if (ret)
        cout << "传输完毕" << endl;
      else
        cerr << "失败，错误码：" << errno;
      break;
    }
    ++i;
    ret = fputc(static_cast<int>(c), fw);   //write
  }
  fclose(fw);
  cout << "拷贝成功，共计拷贝 " << i << " 个字符" << endl;
}

void FileOperator_C_TYPE::close() {
  if (this->fd != NULL) fclose(this->fd);
}
FileOperator_C_TYPE::~FileOperator_C_TYPE() {
  if (this->fd != NULL) fclose(this->fd);
}



