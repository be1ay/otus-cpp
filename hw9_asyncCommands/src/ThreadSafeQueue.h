#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() : closed_(false) {}

    void push(T v) {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            queue_.push(std::move(v));
        }
        cv_.notify_one();
    }

    // Wait and pop. Returns std::nullopt if closed and empty.
    std::optional<T> wait_and_pop() {
        std::unique_lock<std::mutex> ul(mtx_);
        cv_.wait(ul, [this]{ return closed_ || !queue_.empty(); });
        if (queue_.empty()) return std::nullopt;
        T val = std::move(queue_.front());
        queue_.pop();
        return val;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            closed_ = true;
        }
        cv_.notify_all();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return queue_.empty();
    }

private:
    mutable std::mutex mtx_;
    std::queue<T> queue_;
    std::condition_variable cv_;
    bool closed_;
};
