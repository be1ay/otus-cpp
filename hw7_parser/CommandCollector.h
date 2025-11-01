#pragma once

#include <chrono>
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
        if (firstCommandInBlock_) {
            firstTime_ = currentTimestamp();
            firstCommandInBlock_ = false;
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
        if (braceCount_ > 0) --braceCount_;
        if (braceCount_ == 0) {
            // завершили динамический блок — отправляем содержимое
            flushIfAny();
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
            firstCommandInBlock_ = true;
        }
    }

private:
    void flushIfAny() {
        if (commands_.empty()) {
            firstCommandInBlock_ = true;
            return;
        }
        VectorCommands bulk = std::move(commands_);
        std::string timestamp = firstTime_;
        commands_.clear();
        firstCommandInBlock_ = true;
        firstTime_.clear();

        notifier_.notify(bulk, timestamp);
    }

private:
    int N_;
    BulkNotifier& notifier_;
    VectorCommands commands_;
    int braceCount_ = 0;
    bool firstCommandInBlock_ = true;
    std::string firstTime_;
};