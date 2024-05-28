
#include "ThreadPool.hpp"
#include "Logger.hpp"

std::mutex guard_global;

SET_LOG_MODULE_NAME("ExampleModule")
// SET_LOG_LEVEL(static_cast<LogLevel>(LOG_LEVEL_VERBOSE | LOG_LEVEL_DEBUG | LOG_LEVEL_INFO | LOG_LEVEL_WARNING | LOG_LEVEL_ERROR | LOG_LEVEL_FATAL))

SET_LOG_LEVEL(static_cast<LogLevel>(LOG_LEVEL_VERBOSE | LOG_LEVEL_INFO))

int
printNumber(int number)
{
    std::unique_lock<std::mutex> guard{guard_global};
    std::cout << "Number: " << number << std::endl;
    return number;
}

int
main()
{
    // Prints logs for enabled log levels
    LOG_VERBOSE << "...:: SAMPLE_APP ::..." << LOG_NOOP;
    LOG_DEBUG << "Test Message" << LOG_NOOP;
    LOG_INFO << "Test Message" << LOG_NOOP;
    LOG_WARNING << "Test Message" << LOG_NOOP;
    LOG_ERROR << "Test Message" << LOG_NOOP;
    LOG_FATAL << "Test Message" << LOG_NOOP;

    std::cout << LOG_NOOP;
    std::cout << LOG_NOOP;

    {
        LOG_INFO << ">>> Example threadpool 1 <<< " << LOG_NOOP;

        ThreadPool pool;
        std::mutex cout_guard;

        // Enqueue tasks
        for (auto i = 0u; i <= 10; i++) {
            pool.enqueue([&cout_guard, i = i]() {
                {
                    std::unique_lock<std::mutex> guard{cout_guard};  // Do we really need it?
                    LOG_INFO << "Task " << i << " is printed by thread - " << std::this_thread::get_id() << LOG_NOOP;
                }
            });
        }
    }

    LOG_INFO  << ">>> Threadpool 1 destroyed! <<< " <<  LOG_NOOP;
    LOG_INFO  <<  LOG_NOOP;
    
    {
        std::cout << ">>> Example threadpool 2 <<< " <<  LOG_NOOP;

        ThreadPool pool;
        int        sum = 0;

        std::mutex                    cout_guard;
        std::vector<std::future<int>> result;

        // Enqueue tasks
        for (int i = 1; i < 10; ++i) {
            result.push_back(pool.enqueue(printNumber, i));
        }

        for (auto& r : result) {
            sum += r.get();
        }

        LOG_INFO << "Sum of returned values:" << sum << LOG_NOOP;
    }


    LOG_INFO << ">>> Threadpool 2 destroyed! <<< " << LOG_NOOP;

    return 0;
}
