#pragma once

#include <chrono>
#include <iostream>
#include "Observer.h"
#include "BulkNotifier.h"

// --- Вспомогательная функция для получения timestamp в секундах (Unix time) ---
static std::string currentTimestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    return std::to_string(static_cast<long long>(t));
}

// --- Компонент собирающий команды (не зависит от вывода/логирования) ---
class CommandCollector {
public:
    CommandCollector(int N, BulkNotifier& notifier)
        : N_(N), notifier_(notifier) {}

    // вызываем при чтении обычной команды (не { и не })
    void addCommand(const std::string& cmd) {
        if (commands_.empty()) {
            firstTime_ = currentTimestamp();
        }
        commands_.push_back(cmd);
    }

    // открыть динамический блок (встретили '{')
    void openBrace() {
        if (braceCount_ == 0) {
            // при переходе в динамический блок — завершаем текущий обычный пакет
            flushIfAny();
        }
        ++braceCount_;
    }

    // закрыть динамический блок (встретили '}')
    void closeBrace() {
        --braceCount_;
        if (braceCount_ == 0) {
            // завершили динамический блок — отправляем содержимое
            flushIfAny();
        }
        if (braceCount_ < 0) {
            std::cerr << "[WARN] Unmatched closing brace '}' ignored\n";;
            braceCount_=0;
        }
    }

    // проверить, готов ли обычный bulk по N
    void tryFlushBySize() {
        if (braceCount_ == 0 && static_cast<int>(commands_.size()) == N_) {
            flushIfAny();
        }
    }

    // вызывается в конце ввода: если мы НЕ внутри динамического блока, то отправляем остаток
    void finish() {
        if (braceCount_ == 0) {
            flushIfAny();
        } else {
            // блок игнорируется.
            commands_.clear();
        }
    }
    std::string getFirstTime() const { return firstTime_; }

private:
    void flushIfAny() {
        if (commands_.empty()) {
            return;
        }
        VectorCommands bulk = std::move(commands_);
        commands_.clear();
        
        notifier_.notify(bulk);
        firstTime_.clear();
    }

private:
    int N_;
    BulkNotifier& notifier_;
    VectorCommands commands_;
    int braceCount_ = 0;
    std::string firstTime_;
};

void processCommands(std::istream& in, CommandCollector& collector)
{
    std::string command;
    while (in >> command) {
        if (command == "EOF") break;

        if (command == "{") {
            collector.openBrace();
        }
        else if (command == "}") {
            collector.closeBrace();
        }
        else {
            collector.addCommand(command);
            collector.tryFlushBySize();
        }
    }
    collector.finish();
}
