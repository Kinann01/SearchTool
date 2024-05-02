#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <optional>
#include <iostream>

class Thread_pool_t {

public:
    using task_t = std::function<void()>;
    using optional_task_t = std::optional<task_t>;

private:
    std::vector<std::thread> workerThreads; 
    bool canAccess;
    bool poolTerminated;
    std::condition_variable cv;
    std::mutex queueMtx;
    std::queue<optional_task_t> tasks;
    
    bool canRunTask();
    static void executeTask(optional_task_t);
    bool canTerminateTask();
    void construct(std::size_t);

public:
    Thread_pool_t();
    explicit Thread_pool_t(std::size_t);
    void enqueue(task_t&&);
    void shutdown();
    ~Thread_pool_t();
};

#endif // THREAD_POOL_H