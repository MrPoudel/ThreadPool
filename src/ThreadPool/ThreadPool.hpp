#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <class F, class... Args>
    auto
    enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using ReturnType = typename std::result_of<F(Args...)>::type;
        auto task        = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<ReturnType> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            if (mStopping) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            mTasks.emplace([task] {
                (*task)();
            });
        }

        mEventVar.notify_one();
        return result;
    }

private:
    std::vector<std::thread> mThreads;
    std::condition_variable  mEventVar;
    std::mutex               mEventMutex;
    bool                     mStopping = false;

    std::queue<Task> mTasks;

    void start(std::size_t numThreads);
    void stop() noexcept;
};

#endif  // THREADPOOL_H
