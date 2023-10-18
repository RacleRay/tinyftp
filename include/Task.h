#pragma once

#include <event2/event.h>

class Task {
public:
    event_base* evt_base = nullptr;  // task 所属的那个 event 的指针

    int conn_sock = 0;  // task 连接的 socket
    int thread_id = 0;  // task 所属的线程 id

    virtual bool init() = 0;  // pure virtual function
};