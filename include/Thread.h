#pragma once

#include <cstdint>
#include <event2/event.h>
#include <event2/util.h>
#include <list>
#include <memory>
#include <mutex>

#include <ServerCMD.h>


class Thread {
public:
    Thread() = default;
    ~Thread();

    bool setup();

    void start();   // TODO: 与 activate 的区别
    void entry();
    void notify(evutil_socket_t sock) const;
    void activate();
    // void addTask(Task *task);
    void addTask(CmdServer *task);

    int tid = 0;

private:
    int notify_target_fd = 0;
    event_base *thread_evt_base = nullptr;  // 每个线程一个 even 循环，这很重要

    std::list<CmdServer*> task_list;
    std::mutex task_mutex;  // for task_list
};
