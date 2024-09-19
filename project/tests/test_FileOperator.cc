
#include <iostream>
#include <vector>

#include "FileOperator.h"

int main(int argc, char const *argv[]) {
  FileOperator_C_TYPE fw{"./test.txt", "a+"};
  char writebuf[80] = "sadaksjhg65s4df6a4sd6f32d1fa6";
  char readbuf[80] = "";
  // fw.write_n_char(writebuf,20);
  // fw.close();
  fw.copy_to("./test1.txt");
  FileOperator_C_TYPE fr{"./test1.txt", "r+"};
  fr.read_n_char(readbuf, 20);
  cout << readbuf << endl;

  std::vector<int> *p = new std::vector<int>[10];

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      p[i].push_back(j);
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      cout << p[i][j] << " ";
    }
    cout << endl;
  }

  cout << "大小 : " << sizeof(p[0]) << endl;

  FileOperator_Bin_TYPE<std::vector<int>> bfw{"./test2.txt", "wb+"};

  bfw.write_n_char(p, 10);
  
  bfw.fseek_to_head();
  std::vector<int> *pr = new std::vector<int>[10];
  bfw.read_n_char(pr, 10);
  //   FileOperator_Bin_TYPE<std::vector<int>> bfr{"./test2.txt", "rb+"};
  //   bfr.read_n_char(pr, 1);
    for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      cout << pr[i][j] << " ";
    }
    cout << endl;
  }

  delete[] p;
  delete[] pr;

  return 0;
}
