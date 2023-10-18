#pragma once

#include <memory>
#include <vector>

class Task;
class Thread;

class ThreadPool {
public:
    static ThreadPool& getInstance() {
        static ThreadPool instance;
        return instance;
    }

    void init(int num_threads);
    void dispatch(std::unique_ptr<Task> task);

private:
    int m_num_threads;
    int m_last_thread_id{-1};

    std::vector<std::unique_ptr<Thread>> threads;

    ThreadPool() = default;
};