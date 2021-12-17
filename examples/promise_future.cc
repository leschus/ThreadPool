#include <thread>
#include <future>
#include <iostream>
#include <string>
using namespace std;

void TestFuture(promise<string> ps) {
  cout << "Begin TestFuture" << endl;
  this_thread::sleep_for(1s);
  cout << "Begin set_value" << endl;
  ps.set_value("hello, world");
  cout << "End set_value" << endl;
  this_thread::sleep_for(1s);
  cout << "End TestFuture" << endl;
}

void ThreadCaller() {
  // 异步传输 变量存储
  promise<string> ps;
  // 用来获取线程异步返回值
  auto future = ps.get_future();
  thread th(TestFuture, move(ps));
  cout << "begin future get()" << endl;
  // get()阻塞等待线程中执行set_value(), 一旦完成set_value则不再阻塞
  string s = future.get();
  cout << "end future get()" << endl;
  th.join();
  cout << s << endl;
}

int main() {
  ThreadCaller();
}