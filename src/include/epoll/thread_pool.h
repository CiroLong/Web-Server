#ifndef WEBSERVER_THREAD_POOL_H
#define WEBSERVER_THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using std::thread, std::mutex, std::condition_variable, std::atomic,
    std::lock_guard, std::unique_lock;
using std::vector, std::queue, std::function;

class thread_poll {
private:
  using TaskFunction = std::function<void()>;

  vector<thread> poll;

  atomic<bool> started;
  atomic<int> poll_count;

public:
  mutex sync_lock;
  condition_variable condition;
  queue<TaskFunction> task_queue;

  thread_poll(int count = 8);
  ~thread_poll();

  void push(const TaskFunction &function);
};

#endif // WEBSERVER_THREAD_POOL_H