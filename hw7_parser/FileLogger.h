#pragma once

#include <fstream>
#include <functional>
#include "Observer.h"

// --- File logger observer ---
class FileLogger : public Observer {
public:
    explicit FileLogger(std::function<std::string()> timeProvider)  
        : timeProvider_(std::move(timeProvider)) {}
    void onBulk(const VectorCommands& commands) override {
        if (commands.empty()) return;
        std::string timestamp = timeProvider_(); 
        std::string filename = "bulk" + timestamp + ".log";
        std::ofstream ofs(filename);
        if (!ofs) return;
        ofs << "bulk: ";
        for (size_t i = 0; i < commands.size(); ++i) {
            ofs << commands[i];
            if (i + 1 < commands.size()) ofs << ", ";
        }
        ofs << std::endl;
    }
    private:
    std::function<std::string()> timeProvider_;
};