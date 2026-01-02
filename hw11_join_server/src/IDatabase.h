#pragma once
#include <string>
#include <vector>
#include <tuple>

using ResultRow  = std::tuple<int, std::string, std::string>;
using ResultRows = std::vector<ResultRow>;

class IDatabase {
public:
    virtual ~IDatabase() = default;

    virtual bool insert(const std::string& table,
                        int id,
                        const std::string& name,
                        std::string& error) = 0;

    virtual bool truncate(const std::string& table,
                          std::string& error) = 0;

    virtual ResultRows intersection() const = 0;
    virtual ResultRows symmetricDifference() const = 0;
};
