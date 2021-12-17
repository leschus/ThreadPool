// 使用call_once来保证某个函数最多只能被调用一次

#include <thread>
#include <mutex>
#include <iostream>

void SystemInit() {
  std::cout << "System init..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "System init complete." << std::endl;
}

// 确保SystemInit()最多只能被调用一次
void SystemInitOnce() {
  static std::once_flag flag;
  std::call_once(flag, SystemInit);
}

int main() {
  SystemInitOnce();
  SystemInitOnce();
  for (int i = 0; i < 3; i++) {
    std::thread th([i]() {
      std::cout << i << std::endl;
      SystemInitOnce();
    });
    th.detach();
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}