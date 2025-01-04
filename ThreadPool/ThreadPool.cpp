#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads, ThreadMode mode) 
    : stop_flag(false), paused(false), thread_mode(mode) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stop_flag || this->paused ||!this->tasks.empty();
                    });
                    if ((this->stop_flag && this->tasks.empty()) || this->paused)
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                try {
                    task();
                } catch (const std::exception& e) {
                    std::cerr << "Exception in thread pool task: " << e.what() << std::endl;
                }
            }
        });

        if (thread_mode == ThreadMode::DETACH) {
            workers.back().detach();
        }
    }
}

void ThreadPool::setThreadCount(size_t threads) {
    size_t current_threads = workers.size();
    if (threads > current_threads) {
        for (size_t i = current_threads; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop_flag || this->paused ||!this->tasks.empty();
                        });
                        if ((this->stop_flag && this->tasks.empty()) || this->paused)
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    try {
                        task();
                    } catch (const std::exception& e) {
                        std::cerr << "Exception in thread pool task: " << e.what() << std::endl;
                    }
                }
            });
            if (thread_mode == ThreadMode::DETACH) {
                workers.back().detach();
            }
        }
    } else if (threads < current_threads) {
        size_t diff = current_threads - threads;
        for (size_t i = 0; i < diff; ++i) {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop_flag = true;
            }
            condition.notify_all();
        }
        for (size_t i = 0; i < diff; ++i) {
            if (workers.back().joinable()) {
                workers.back().join();
            }
            workers.pop_back();
        }
        stop_flag = false;
    }
}

void ThreadPool::pause() {
    paused = true;
}

void ThreadPool::resume() {
    paused = false;
    condition.notify_all();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
    }
    condition.notify_all();
    if (thread_mode == ThreadMode::JOIN) {
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
}

ThreadPool::~ThreadPool() {
    stop();
}