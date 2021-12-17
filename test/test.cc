#include "thread_pool.h"

#include <cassert>

#include <functional>
#include <iostream>
#include <string>

namespace tptest {
  void Countdown(int n) {
    assert(n >= 0);
    while (n--) {
      // 每1s打印一次
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "From thread " << std::this_thread::get_id() << ": "
                << n << std::endl;
    }
  }

  void Greeting(const std::string& name, int times) {
    assert(times >= 0);
    while (times--) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "From thread " << std::this_thread::get_id() << ": "
                << "Hello, " << name << std::endl;
    }
  }
}

int main() {
  using namespace tptest;

  ThreadPool pool(3);
  pool.submit(Countdown, 10);
  pool.submit(Greeting, "Sean", 5);
  return 0;
}