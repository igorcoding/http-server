#include "thread_pool.h"

thread_pool::thread_pool(size_t threads_count)
    : _threads_count(threads_count),
      _available(threads_count),
      _running(true)
{
    for (size_t i = 0; i < _threads_count; ++i) {
        std::thread t(std::bind(&thread_pool::worker, this));
        _threads.emplace_back(std::move(t));
    }
}

void thread_pool::worker()
{
    while (_running) {
        std::unique_lock<std::mutex> lock(_m);
        _condition.wait(lock, [this]{ return !(_tasks.empty() && _running); });

        if (!_running) {
            // throw ?
            return;
        }

        {
            auto task = _tasks.front();
            _tasks.pop();

            lock.unlock();

            try {
                task();
            } catch (std::exception& e) {

            }
        }

        lock.lock();
        ++_available;
    }
}


template <typename Task>
void thread_pool::execute(Task task)
{
    std::unique_lock<std::mutex> lock(_m);

    _tasks.push(std::function<void()>(task));
    --_available;

    lock.unlock();
    _condition.notify_one();
}










