// 使用匿名的lambda函数作为线程入口
// 包括两种情形:
//  * 在类外或类static成员函数中使用
//  * 在类non-static成员函数中使用

#include <thread>
#include <string>
#include <iostream>
using namespace std;

class Foo {
 public:
  explicit Foo(const string& s) : name_(s) {}
  void StartThread() {
    thread th([this]() {
      cout << "In StartThread's lambda function: " << name_ << endl;
    });
    th.join();
  }
 private:
  string name_;
};

void ThreadCaller() {
  thread th([](int i) {
    cout << "In lambda function: " << i << endl;
  }, 123);
  th.join();
}

void ThreadFooCaller() {
  Foo f("Ella");
  f.StartThread();
}


int main() {
  ThreadCaller();
  ThreadFooCaller();
}