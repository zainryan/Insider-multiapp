#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <string>
#include <vector>

using namespace std;

#define FILE_ROW_NUM (1024 * 1024 * 4)
#define FILE_COL_NUM (1024)

char random_char() {
  int min_ascii = 65;
  int max_ascii = 68;
  return min_ascii + rand() % (max_ascii - min_ascii + 1);
}

int main() {
  ios::sync_with_stdio(false);

  freopen("input.txt", "w", stdout);

  for (int i = 0; i < FILE_ROW_NUM; i++) {
    string line = "";
    for (int j = 0; j < FILE_COL_NUM - 1; j++) {
      line += random_char();
    }
    cout << line << endl;
  }

  return 0;
}
