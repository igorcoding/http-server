#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class thread_pool
{
public:
    thread_pool(size_t threads_count);
    ~thread_pool();

    template <typename Task> void execute(Task task);
    void stop() { _running = false; }

private:
    void worker();

private:
    size_t _threads_count;
    size_t _available;
    std::vector<std::thread> _threads;
    std::condition_variable _condition;
    std::mutex _m;
    bool _running;

    std::queue<std::function<void()>> _tasks;
};

#endif // THREADPOOL_H
