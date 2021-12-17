#ifndef MUTEX_RAII_H_
#define MUTEX_RAII_H_

// 将RAII实践于mutex上: Locker类
// 创建Locker对象时, 即调用构造函数, 完成上锁;
// Locker对象被释放时, 即调用析构函数, 完成解锁.

#include <mutex>
#include <iostream>

class Locker {
 public:
  Locker(std::mutex* pm) : pm_(pm) {
    std::cout << "Lock" << std::endl << std::flush;
    pm_->lock();
  }
  ~Locker() {
    std::cout << "Unlock" << std::endl << std::flush;
    pm_->unlock();
  }
 private:
  std::mutex * pm_; // 使用引用也行, 目的都是为了从外部传入mutex对象, 本类只是用于
                    // 管理它的lock()和unlock()动作.
};


#endif // MUTEX_RAII_H_