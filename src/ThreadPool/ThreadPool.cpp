#include "ThreadPool.hpp"
#include "Logger.hpp"

SET_LOG_MODULE_NAME("ExampleThreadPool")
SET_LOG_LEVEL(static_cast<LogLevel>(LOG_LEVEL_VERBOSE | LOG_LEVEL_DEBUG | LOG_LEVEL_INFO | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL))

ThreadPool::ThreadPool(std::size_t numThreads)
{
    LOG_INFO << "Thread Pool created: " << numThreads << " Threads" << LOG_NOOP;

    if (!numThreads) {
        throw std::invalid_argument("bad thread count! must be non-zero!");
    }
    start(numThreads);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void
ThreadPool::start(std::size_t numThreads)
{
    for (auto i = 0u; i < numThreads; ++i) {
        mThreads.emplace_back([=] {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lock{mEventMutex};
                    mEventVar.wait(lock, [=] {
                        return mStopping || !mTasks.empty();
                    });

                    if (mStopping && mTasks.empty()) {
                        break;
                    }

                    task = std::move(mTasks.front());
                    mTasks.pop();
                }

                task();
            }
        });
    }
}

void
ThreadPool::stop() noexcept
{
    {
        std::unique_lock<std::mutex> lock{mEventMutex};
        mStopping = true;
    }

    mEventVar.notify_all();

    for (auto& thread : mThreads) {
        thread.join();
    }
}
