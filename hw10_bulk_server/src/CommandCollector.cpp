#include "CommandCollector.h"
#include <chrono>
#include <iostream>

static std::string currentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    return std::to_string(static_cast<long long>(t));
}

CommandCollector::CommandCollector(int N, Dispatcher& dispatcher)
    : N_(N), dispatcher_(dispatcher) {}

void CommandCollector::addCommand(const std::string& cmd) {
    if (commands_.empty()) {
        firstTime_ = currentTimestamp();
    }
    commands_.push_back(cmd);
}

void CommandCollector::openBrace() {
    if (braceCount_ == 0) {
        flushIfAny();
    }
    ++braceCount_;
}

void CommandCollector::closeBrace() {
    --braceCount_;
    if (braceCount_ == 0) {
        flushIfAny();
    }
    if (braceCount_ < 0) {
        std::cerr << "[WARN] Unmatched closing brace '}' ignored\n";
        braceCount_ = 0;
    }
}

void CommandCollector::tryFlushBySize() {
    if (braceCount_ == 0 && static_cast<int>(commands_.size()) == N_) {
        flushIfAny();
    }
}

void CommandCollector::finish() {
    if (braceCount_ == 0) {
        flushIfAny();
    } else {
        // ignore
        commands_.clear();
        firstTime_.clear();
    }
}

void CommandCollector::flushIfAny() {
    if (commands_.empty()) return;
    // move commands out
    VectorCommands bulk = std::move(commands_);
    std::string ts = std::move(firstTime_);

    // reset local state
    commands_.clear();
    firstTime_.clear();

    dispatcher_.submit(std::move(bulk), std::move(ts));
}
