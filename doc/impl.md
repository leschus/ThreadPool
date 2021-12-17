# 线程池的设计与实现

thread_pool的组件:

* `vector<thread> workers_`: 工作线程组
* `queue<Task> tasks_`: 任务队列

thread_pool的基本操作:

* `thread_pool::thread_pool(...)`: 创建线程池并初始化
* `thread_pool::submit(...)`: 添加新任务
* `thread_pool::~thread_pool()`: 销毁线程池

其它:

* `mutex mutex_`: 互斥量
* `condition_variable condition_`: 条件变量
* `bool is_stop_`: 停止标志