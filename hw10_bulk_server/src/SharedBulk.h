#pragma once
#include <cstdint>
#include <string>
#include <vector>

using VectorCommands = std::vector<std::string>;
struct SharedBulk {
    VectorCommands commands;
    std::string timestamp;
    uint64_t seq = 0;
};
