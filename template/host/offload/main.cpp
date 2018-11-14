#include <cassert>
#include <chrono>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <mutex>
#include <omp.h>
#include <queue>
#include <thread>
#include <unistd.h>

#include <insider_runtime.hpp>
#include <stdio.h>
#include <time.h>

using namespace std;

#define READ_BUF_SIZE (2 * 1024 * 1024)

int fd;
int buf[READ_BUF_SIZE];

mutex cout_mutex;

class Timer {
public:
  Timer() : m_beg(clock_::now()) {}
  void reset() { m_beg = clock_::now(); }

  double elapsed() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(clock_::now() -
                                                                 m_beg)
        .count();
  }

private:
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::ratio<1>> second_;
  std::chrono::time_point<clock_> m_beg;
};

void work(int tid) {
  Timer timer;

  string real_file_path = "/mnt/input_" + to_string(tid) + ".txt";
  const char *virt_path = (char *)reg_virt_file(real_file_path.c_str());
  int fd = iopen(tid, virt_path, 0);
  bool fin_file = false;
  int total_read_bytes = 0;

  while (!fin_file) {
    int read_bytes = 0;
    while (read_bytes != READ_BUF_SIZE) {
      int tmp = iread(fd, buf, READ_BUF_SIZE - read_bytes);
      if (!tmp) {
        fin_file = true;
        break;
      } else {
        read_bytes += tmp;
      }
    }
    total_read_bytes += read_bytes;
  }
  iclose(fd);

  cout_mutex.lock();
  cout << "tid = " << tid << ", total_read_bytes = " << total_read_bytes
       << endl;
  cout << "tid = " << tid << ", elapsed time = " << timer.elapsed() / 1000
       << endl;
  cout_mutex.unlock();
}

int main(int argc, char **argv) {
  std::thread t0(work, 0);
  std::thread t1(work, 1);
  std::thread t2(work, 2);
  while (true)
    ;

  return 0;
}
