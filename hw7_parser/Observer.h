#pragma once
#include <vector>
#include <string>

using VectorCommands = std::vector<std::string>;

struct Observer {
    virtual ~Observer() = default;
    virtual void onBulk(const VectorCommands& commands) = 0;
};