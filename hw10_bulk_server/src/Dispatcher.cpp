#include "Dispatcher.h"
#include <iostream>
#include <fstream>

Dispatcher::Dispatcher()
    : seqCounter_(0), running_(false)
{}

Dispatcher::~Dispatcher() {
    stop();
}

void Dispatcher::start() {
    if (running_.exchange(true)) return;

    // log thread: pops from logQueue and calls notifier.notify
    logThread_ = std::thread(&Dispatcher::logWorker, this);

    // two file workers
    fileThread1_ = std::thread(&Dispatcher::fileWorker, this, 1);
    fileThread2_ = std::thread(&Dispatcher::fileWorker, this, 2);
}

void Dispatcher::stop() {
    if (!running_.exchange(false)) return;

    logQueue_.close();
    fileQueue_.close();

    if (logThread_.joinable()) logThread_.join();
    if (fileThread1_.joinable()) fileThread1_.join();
    if (fileThread2_.joinable()) fileThread2_.join();
}

void Dispatcher::submit(VectorCommands commands, std::string timestamp) {
    auto item = std::make_shared<SharedBulk>();
    item->commands = std::move(commands);
    item->timestamp = std::move(timestamp);
    item->seq = seqCounter_.fetch_add(1, std::memory_order_relaxed);

    // push into both queues
    logQueue_.push(item);
    fileQueue_.push(item);
}

void Dispatcher::logWorker() {
    while (true) {
        auto opt = logQueue_.wait_and_pop();
        if (!opt) break;
        SharedBulkPtr item = *opt;
        if (!item) continue;

        std::cout << "bulk: ";
        for (size_t i = 0; i < item->commands.size(); ++i) {
            std::cout << item->commands[i];
            if (i + 1 < item->commands.size()) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void Dispatcher::fileWorker(int workerId) {
    while (true) {
        auto opt = fileQueue_.wait_and_pop();
        if (!opt) break;
        SharedBulkPtr item = *opt;
        if (!item) continue;

        // Make unique filename: bulk<TIMESTAMP>_<SEQ>_<WORKER>.log
        std::string filename = "bulk" + item->timestamp + "_" + std::to_string(item->seq) + "_" + std::to_string(workerId) + ".log";
        std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
        if (!ofs) {
            std::cerr << "Dispatcher: cannot open file " << filename << std::endl;
            continue;
        }
        ofs << "bulk: ";
        for (size_t i = 0; i < item->commands.size(); ++i) {
            ofs << item->commands[i];
            if (i + 1 < item->commands.size()) ofs << ", ";
        }
        ofs << std::endl;
    }
}
