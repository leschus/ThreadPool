// 线程入口的几种创建方式(包括传参方式)
//  * 使用全局函数: 示例见本文件
//  * 使用static成员函数: 用法与使用全局函数类似, 不再额外举例
//  * 使用non-static成员函数: 见thread_entrance_mf.cc, "mf"表示"member function"
//  * 使用匿名lambda函数: 见thread_entrance_lambda.cc

#include <string>
#include <iostream>
#include <thread>
using namespace std;

class Foo {
 public:
  explicit Foo(const string& name = "unknown") : name_(name) {
    cout << "Foo's c'tor" << endl;
  }
  Foo(const Foo& rhs) : name_(rhs.name_) {
    cout << "Foo's copy c'tor" << endl;
  }
  ~Foo() { cout << "Foo's d'tor" << endl; }
  const string& name() const { return name_; }
 private:
  string name_;
};

ostream& operator<<(ostream& os, const Foo& foo) {
  os << foo.name();
  return os;
}

// 全局函数作为线程入口.
// 接收多个参数, 不含指针/引用类型参数.
void ThreadFunc(int n, float f, string s, Foo foo) {
  cout << "In ThreadFunc: " << n << ", " << f << ", " << s << endl;
  cout << foo.name() << endl;
  cout << "End of ThreadFunc" << endl;
}

template <typename T>
void ThreadFuncWithPtr(T* p) {
  this_thread::sleep_for(1s);
  // 下面语句中的指针指向主线程中的栈内存, 而那部分内存已被释放.
  cout << "In ThreadFuncWithPtr: " << *p << endl;
  cout << "End of ThreadFuncWithPtr" << endl;
}

template <typename T>
void ThreadFuncWithReference(T& r) {
  this_thread::sleep_for(1s);
  cout << "In ThreadFuncWithReference: " << r << endl;
  cout << "End of ThreadFuncWithReference" << endl;
}

void ThreadCaller() {
  float x = 3.14;
  Foo foo("Bob");
  thread th(ThreadFunc, 10, x, "GODV", foo);
  th.join();
}

void ThreadWithPtrCaller() {
  thread th;
  {
    int n = 10;
    th = thread(ThreadFuncWithPtr<int>, &n);
    // 此block结束后, n将被释放
  }
  cout << "Out of block 1" << endl;
  th.join();
  {
    Foo foo("Mike");
    th = thread(ThreadFuncWithPtr<Foo>, &foo);
    // 此block结束后, foo将被释放
  }
  cout << "Out of block 2" << endl;
  th.join();
}

void ThreadWithReferenceCaller() {
  thread th;
  {
    int n = 10;
    th = thread(ThreadFuncWithReference<int>, ref(n));  // ref()显式指明n是引用
    // 此block结束后, n将被释放
  }
  cout << "Out of block 1" << endl;
  th.join();
  {
    Foo foo("Nancy");
    th = thread(ThreadFuncWithReference<Foo>, ref(foo));
    // 此block结束后, foo将被释放
  }
  cout << "Out of block 2" << endl;
  th.join();
}

int main() {
  // ThreadCaller();
  // ThreadWithPtrCaller();
  ThreadWithReferenceCaller();
  return 0;
}

// 调用ThreadCaller()的实际输出:
//
// Foo's c'tor
// Foo's copy c'tor
// Foo's copy c'tor
// Foo's d'tor
// In ThreadFunc: 10, 3.14, GODV
// Bob
// End of ThreadFunc
// Foo's d'tor
// Foo's d'tor
//
// (存在对foo的两次拷贝构造, 由此推测子线程内部使用的是传入参数的副本)