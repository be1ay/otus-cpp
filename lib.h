#pragma once

#include <string>
#include <array>

int version();

struct IP {
    std::array<int, 4> octets;
    bool operator<(const IP& other) const {
        return octets < other.octets;
    }
};

IP parse_ip(const std::string& s);