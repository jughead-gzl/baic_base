#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace structure
{
namespace pool
{
class ThreadPool 
{
public:
    // 构造：创建 n 个工作线程
    explicit ThreadPool(std::size_t threadNum) : stop_(false)
    {
        if (threadNum == 0)
        {
            throw std::invalid_argument("ThreadPool requires at least one worker");
        }

        workers_.reserve(threadNum);
        for (std::size_t i = 0; i < threadNum; ++i)
        {
            workers_.emplace_back([this]() {
                while (true) 
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lck(mtx_);
                        // 等待有任务或者线程池停止
                        cond_.wait(lck, [this]() { return stop_ || !tasks_.empty(); });

                        // 停止且任务队列为空，退出线程
                        if (stop_ && tasks_.empty()) 
                        {
                            return;
                        }

                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task(); // 执行任务
                }
            });
        }
    }

    // 提交任意可调用对象，返回 future 获取返回值
    template <typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
    {
        using ReturnType = std::invoke_result_t<Func, Args...>;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<ReturnType> res = task->get_future();
        {
            std::lock_guard<std::mutex> lck(mtx_);
            if (stop_)
            {
                throw std::runtime_error("ThreadPool has stopped, cannot submit task");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        cond_.notify_one(); // 唤醒一个工作线程
        return res;
    }

    // 析构：优雅关闭线程池
    ~ThreadPool() 
    {
        {
            std::lock_guard<std::mutex> lck(mtx_);
            stop_ = true;
        }
        cond_.notify_all(); // 唤醒所有等待线程

        // 等待所有工作线程执行完毕
        for (auto& worker : workers_) 
        {
            if (worker.joinable()) 
            {
                worker.join();
            }
        }
    }

    // 禁止拷贝与移动
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

private:
    std::vector<std::thread> workers_;                // 工作线程数组
    std::queue<std::function<void()>> tasks_;         // 任务队列（缓冲区）
    std::mutex mtx_;
    std::condition_variable cond_;
    bool stop_;                                       // 线程池停止标记，由 mtx_ 保护
};  
};
};

