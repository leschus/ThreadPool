#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t num_workers = kDefaultWorkersNum );
  ~ThreadPool();
  template <typename F, typename... Args>
  auto submit(F &&f, Args&&... args)
      ->  std::future<typename std::result_of<F(Args...)>::type>;

 private:
  enum { kDefaultWorkersNum = 4 };
  typedef std::function<void ()> Task;
  std::vector<std::thread> workers_;  // 工作线程组
  std::queue<Task> tasks_;            // 任务队列
  std::mutex mutex_;                  // 互斥量
  std::condition_variable condition_; // 条件变量
  bool is_stop_;                      // 勒令线程池停止的标志
};

// 初始化线程并绑定入口函数
ThreadPool::ThreadPool(size_t num_workers) : is_stop_(false) {
  for (size_t i = 0; i < num_workers; ++i) {
    workers_.emplace_back(
      [this] {
        for(;;) {
          Task task;
          {
            std::unique_lock<std::mutex> lock(this->mutex_);
            this->condition_.wait(lock,
              [this] {
                // 等待, 直到线程池被勒令停止, 或任务队列非空
                return this->is_stop_ || !this->tasks_.empty();
              }
            );
            if (this->is_stop_ && this->tasks_.empty()) {
              // 线程池被勒令停止, 且任务队列也为空, 则直接返回
              return;
            }
            // 执行到这里, 有两种情形:
            // 1) 线程池已被勒令停止, 但任务队列中尚有待执行的任务, 需要将它们
            // 执行完线程池才能最终停止;
            // 2) 线程池在正常运转, 且任务队列中存在待执行的任务;
            // 两种情形的处理方式均是取出队首任务并执行.
            task = std::move(this->tasks_.front());
            this->tasks_.pop();
          }
          task();
        }
      }
    );
  }
}

// 析构函数将阻塞等待所有工作线程结束
ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    is_stop_ = true;
  }
  condition_.notify_all();
  for (auto& worker: workers_) {
    worker.join();
  }
}

template <typename F, typename... Args>
auto ThreadPool::submit(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  typedef typename std::result_of<F(Args...)>::type return_type;
  // using return_type = typename std::result_of<F(Args...)>::type;
  auto task = std::make_shared<std::packaged_task<return_type()>> (
      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
      );
  std::future<return_type> ret = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mutex_);
    if (is_stop_) {
      // 在线程池已被勒令停止的状态下尝试提交任务, 将抛出异常
      throw std::runtime_error("submit on stopped ThreadPool");
    }
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return ret;
}

#endif // THREAD_POOL_H_