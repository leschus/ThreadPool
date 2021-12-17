// 使用thread类创建和管理线程
// 涉及到以下概念:
//    * 绑定线程工作函数ThreadFunc
//    * 获取线程ID: this_thread::get_id()
//    * 挂起当前线程: this_thread::sleep_for(...)
//    * 阻塞以等待子线程结束: th.join()

#include <thread> // -lpthread
#include <iostream>

using std::cout;
using std::endl;

void ThreadFunc() {
  // 获取所在的子线程ID并打印
  cout << "Begin ThreadFunc with ID: " << std::this_thread::get_id() << endl;
  // 睡眠
  for (int i = 0; i < 5; i++) {
    cout << "ThreadFunc: " << i << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); //  1s
  }
  cout << "End ThreadFunc" << endl;
}

int main() {
  // 获取主线程id并打印
  cout << "Main thread ID: " << std::this_thread::get_id() << endl;

  // 创建线程(创建成功即启动线程)
  std::thread th(ThreadFunc);

  cout << "Begin waiting sub thread..." << endl;
  // 阻塞主线程, 等待子线程结束
  th.join();
  cout << "End waitting sub thread." << endl;

  return 0;
}