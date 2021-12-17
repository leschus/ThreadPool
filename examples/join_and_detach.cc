// 主线程和子线程如何实现简单的同步?
// 即: 如何保证主线程退出时, 其所有的子线程也已经退出

// 使用join()方法是可行的, 但这会导致主线程阻塞.
// 我们希望的是, 主线程和子线程能够同时工作, 并且当主线程完成工作时, 若子线程还未结束,
// 主线程应当有能力通知子线程提前结束, 随后主线程在得知子线程确实已结束后再退出.

// 目前的解决方案见testcase1()

#include <thread> // -lpthread
#include <iostream>

using std::cout;
using std::endl;

bool is_stop;

void ThreadFunc() {
  cout << "Begin ThreadFunc with ID: " << std::this_thread::get_id() << endl;
  for (int i = 0; i < 5; i++) {
    if (is_stop) {
      break;    // 提前退出循环
    }
    cout << "ThreadFunc: " << i << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); //  1s
  }
  cout << "End ThreadFunc" << endl;
}

void testcase1() {
  cout << "Test case 1" << endl;
  is_stop = false;
  std::thread th(ThreadFunc);
  // 模拟主线程执行工作(耗时2s)
  std::this_thread::sleep_for(std::chrono::seconds(2));
  is_stop = true; // 通知子线程结束
  th.join();  // 等待子线程结束, 并释放子线程资源
}

void testcase2() {
  cout << "Test case 2" << endl;
  std::thread th(ThreadFunc);
  th.detach();  // 将子线程和主线程分离, 此后th将失去对子线程的管控权.
                // *陷阱*: 主线程结束时, 子线程可能还在运行. 此时若子线程访问主线程中
                // 的数据, 将导致程序崩溃.
  cout << "Try get_id() after detach: " << th.get_id() << endl;
  // 退出这个block后, th对象被销毁, 但由于th已经与子线程无关联, 所有子线程不会受到影响.
}

void testcase3() {
  cout << "Test case 3" << endl;
  std::thread th(ThreadFunc);
  // 退出这个block后, th对象被销毁, 若此时子线程仍在执行, 它将被强制终止.
  // info: terminate called without an active exception
}

int main() {
  testcase1();
  testcase2();
  testcase3();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}