#pragma once

#include <cstdint>
#include <event2/event.h>
#include <event2/util.h>
#include <list>
#include <memory>
#include <mutex>

class Task;

class Thread {
public:
    Thread();
    ~Thread();

    bool setup();

    void start();   // TODO: 与 activate 的区别
    void entry();
    void notify(evutil_socket_t sock) const;
    void activate();
    // void addTask(Task *task);
    void addTask(std::unique_ptr<Task> task_uptr);

    int tid = 0;

private:
    int notify_target_fd = 0;
    event_base *thread_evt_base = nullptr;  // 每个线程一个 even 循环，这很重要

    std::list<std::unique_ptr<Task>> task_list;
    std::mutex task_mutex;  // for task_list
};
