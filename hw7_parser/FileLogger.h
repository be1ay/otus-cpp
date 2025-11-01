#pragma once

#include <fstream>
#include "Observer.h"

// --- File logger observer ---
class FileLogger : public Observer {
public:
    void onBulk(const VectorCommands& commands, const std::string& timestamp) override {
        if (commands.empty()) return;
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
};