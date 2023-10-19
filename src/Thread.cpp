#include <event2/event.h>
#include <iostream>
#include <thread>
#include <unistd.h>

#include <DebugTool.h>
#include <ServerCMD.h>
#include <Task.h>
#include <Thread.h>


static void notify_callback(evutil_socket_t sock, short what, void *arg);


void Thread::start() {
    TESTOUT("Thread " << tid << " start");

    setup();

    std::thread t(&Thread::entry, this);
    t.detach();
}


// run the event loop in this thread
void Thread::entry() {
    std::cout << "Thread " << tid << " begin." << std::endl;
    event_base_dispatch(thread_evt_base);
    event_base_free(thread_evt_base);
    std::cout << "Thread " << tid << " end." << std::endl;
}


bool Thread::setup() {
    TESTOUT("Thread " << tid << " setup");

    int pipefds[2];
    if (pipe(pipefds) < 0) {
        std::cerr << "Failed to create pipe." << std::endl;
        return false;
    }

    // 写端：用于主线程通知子线程处理任务
    notify_target_fd = pipefds[1];

    // libevent 上下文
    event_config *cfg = event_config_new();
    event_config_set_flag(cfg, EVENT_BASE_FLAG_NOLOCK);
    this->thread_evt_base = event_base_new_with_config(cfg);
    event_config_free(cfg);
    if (!thread_evt_base) {
        std::cerr << "Failed to create libevent context." << std::endl;
        return false;
    }

    // 添加管道监听事件，用于激活线程池执行任务
    event* ev = event_new(thread_evt_base, pipefds[0], EV_READ | EV_PERSIST, notify_callback, this);
    event_add(ev, nullptr);

    return true;
}



// 功能只有在 activte Thread 后，读取到管道内容，然后显示cout的提示消息而已
void Thread::notify(evutil_socket_t sock) const {
    TESTOUT("Thread " << tid << " notified");

    char buf[2] = {0};
    size_t nret = read(sock, buf, 1);
    if (nret < 0) {
        return;
    }
    std::cout << "Thread " << tid << " notified " << buf[0] << std::endl;
}


static void notify_callback(evutil_socket_t sock, short what, void *arg) {
    Thread *th = (Thread *)arg;
    th->notify(sock);
}


void Thread::activate() {
    TESTOUT("Thread " << tid << " activate");

    size_t nret = write(notify_target_fd, "1", 1);
    if (nret <= 0) {
        std::cerr << "Failed to notify thread pool." << std::endl;
    }

    std::lock_guard<std::mutex> lock(task_mutex);
    if (task_list.empty()) {
        return;
    }

    CmdServer* task = task_list.front();
    task_list.pop_front();
    task->init();
}


void Thread::addTask(CmdServer* task) {
    if (task == nullptr) { return; }

    // 任务继承线程的libevent上下文
    // CMD 任务在这个上下文注册事件，使线程处理任务添加的事件
    task->evt_base = this->thread_evt_base;

    std::lock_guard<std::mutex> lock(task_mutex);
    task_list.push_back(task);
}


Thread::~Thread() {
    for (auto* task : task_list) {
        delete task;
    }
}
