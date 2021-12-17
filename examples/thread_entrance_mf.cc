// 使用non-static成员函数作为线程入口

// 设计考量:
// 使用一个类对象来进行线程的管理, 包括线程创建/销毁/etc, 将对象的数据成员作为
// 传递给线程入口函数的参数, 这样一来就一定程度上保证了参数和线程具有相同的生命周期,
// 从而避免出现"线程尚未结束, 但指针/引用参数所指涉的内容已经被销毁"的问题.

#include <string>
#include <iostream>
#include <thread>
using namespace std;

class MyThread {
 public:
  explicit MyThread(const string& s = "unknown") : name_(s) {}
  void ThreadMain() {
    cout << "In ThreadMain: " << name_ << endl;
    cout << "End of ThreadMain" << endl;
  }
 private:
  string name_;
};

void ThreadCaller() {
  MyThread myth;
  thread th(&MyThread::ThreadMain, &myth);  // 将myth的ThreadMain()作为线程入口
  th.join();
}

int main() {
  ThreadCaller();
  return 0;
}