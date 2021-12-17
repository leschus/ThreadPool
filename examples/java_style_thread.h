#ifndef JAVA_STYLE_THREAD_H_
#define JAVA_STYLE_THREAD_H_

// 实现一个简单的Java风格的Thread类
// 该类提供一个Run()接口作为线程入口, 派生类需要提供Run的实现
// 另外, 该类提供基本的线程管理能力: 创建/等待结束/要求提前结束/etc.

#include <thread>

class Thread {
 public:
  Thread() : is_stop_(false) {
    th_ = std::thread(Run, this);
  }
  Thread(const Thread&)  = delete;

  virtual ~Thread() {
    Wait();
  }
  void Wait() {
    if (th_.joinable()) {
      th_.join();
    }
  }
  void Stop() {
    is_stop_ = true;
    Wait();
  }
  bool is_stop() const { return is_stop_; }

  Thread& operator=(const Thread&) = delete;
 private:
  virtual void Run() = 0; // 纯虚函数, 派生类必须提供实现
  std::thread th_;
  bool is_stop_;
};

#endif // JAVA_STYLE_THREAD_H_