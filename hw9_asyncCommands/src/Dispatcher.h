#pragma once
#include <memory>
#include <thread>
#include <atomic>
#include "ThreadSafeQueue.h"
#include "SharedBulk.h"

using SharedBulkPtr = std::shared_ptr<SharedBulk>;

class Dispatcher {
public:
    Dispatcher();
    ~Dispatcher();

    // start workers; notifier will be used by log thread to print to console
    void start();

    // stop workers
    void stop();

    // submit new bulk (from CommandCollector)
    void submit(VectorCommands commands, std::string timestamp);

private:
    void logWorker();
    void fileWorker(int workerId);

    ThreadSafeQueue<SharedBulkPtr> logQueue_;
    ThreadSafeQueue<SharedBulkPtr> fileQueue_;

    std::thread logThread_;
    std::thread fileThread1_;
    std::thread fileThread2_;

    std::atomic<uint64_t> seqCounter_;
    std::atomic<bool> running_;
};
