#pragma once

#include <memory>
#include <vector>

#include <ServerCMD.h>
#include <Thread.h>

class CmdServer;

class ThreadPool {
public:
    static ThreadPool& get() {
        static ThreadPool instance;
        return instance;
    }

    void init(int num_threads);
    void dispatch(std::unique_ptr<CmdServer> task);

private:
    int m_num_threads;
    int m_last_thread_id{-1};

    std::vector<std::unique_ptr<Thread>> threads;

    ThreadPool() {};
};