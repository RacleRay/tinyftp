#include <thread>

#include <DebugTool.h>
#include <Thread.h>
#include <ThreadPool.h>


void ThreadPool::init(int num_threads) {
    TESTOUT("main thread id: " << std::this_thread::get_id() << "init");

    this->m_num_threads = num_threads;
    this->m_last_thread_id = -1;

    for (int i = 0; i < this->m_num_threads; i++) {
        std::cout << "create thread " << i << std::endl;
        auto thread = std::make_unique<Thread>();

        thread->tid = i;
        thread->start();

        threads.push_back(std::move(thread));

        // wait for thread starting 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void ThreadPool::dispatch(CmdServer* task) {
    TESTOUT("main thread id: " << std::this_thread::get_id() << " ThreadPool::dispatch");

    if (task == nullptr) {
        return ;
    }    

    int thread_id = (m_last_thread_id + 1) % m_num_threads;
    m_last_thread_id = thread_id;

    threads[thread_id]->addTask(task);
    threads[thread_id]->activate();
}