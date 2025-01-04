#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <utility>
#include <stdexcept>
#include <atomic>
#include <iostream>
#include <chrono>


class ThreadPool {
public:
    enum class ThreadMode {
        JOIN,
        DETACH
    };

    ThreadPool(size_t threads = std::thread::hardware_concurrency(), ThreadMode mode = ThreadMode::JOIN);

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    void setThreadCount(size_t threads);
    void pause();
    void resume();
    void stop();

    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop_flag;
    std::atomic<bool> paused;
    ThreadMode thread_mode;

    void workerThread();
};

#include "ThreadPool.tpp"

#endif // THREAD_POOL_H