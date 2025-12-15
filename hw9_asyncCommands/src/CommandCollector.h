#pragma once
#include <string>
#include "SharedBulk.h"
#include "Dispatcher.h"

// CommandCollector no longer notifies observers directly.
// It submits bulks to Dispatcher.
class CommandCollector {
public:
    CommandCollector(int N, Dispatcher& dispatcher);

    void addCommand(const std::string& cmd);
    void openBrace();
    void closeBrace();
    void tryFlushBySize();
    void finish();

private:
    void flushIfAny();

    int N_;
    Dispatcher& dispatcher_;
    VectorCommands commands_;
    int braceCount_ = 0;
    std::string firstTime_;
};
