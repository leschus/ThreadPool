#include "java_style_thread.h"

#include <string>
#include <iostream>
using namespace std;

class MyThread : public Thread {
 public:
  explicit MyThread(const string& s = "unknown") : name_(s) {}

  void Run() override {
    cout << "Start Running: " << name_ << endl;
    while (!is_stop()) {
      // 模拟线程工作
      this_thread::sleep_for(100ms);
      cout << "." << flush;
    }
    cout << "Stop Running" << endl;
  }
 private:
  string name_;
};

int main() {
  MyThread myth("God");
  this_thread::sleep_for(2s);
  myth.Stop();  // 要求子线程提前退出
  return 0;
}