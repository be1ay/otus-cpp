#pragma once
#include <iostream>
#include "Observer.h"

class ConsolePrinter : public Observer {
public:
    void onBulk(const VectorCommands& commands) override {
        if (commands.empty()) return;
        std::cout << "bulk: ";
        for (size_t i = 0; i < commands.size(); ++i) {
            std::cout << commands[i];
            if (i + 1 < commands.size()) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};