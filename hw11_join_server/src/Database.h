#pragma once
#include <map>
#include <string>
#include <vector>
#include <tuple>

using Table = std::map<int, std::string>;

class Database {
public:
    // INSERT table id name
    bool insert(const std::string& table, int id, const std::string& name, std::string& error);

    // TRUNCATE table
    bool truncate(const std::string& table, std::string& error);

    // INTERSECTION
    std::vector<std::tuple<int, std::string, std::string>> intersection() const;

    // SYMMETRIC_DIFFERENCE
    std::vector<std::tuple<int, std::string, std::string>> symmetricDifference() const;

private:
    Table tableA_;
    Table tableB_;
    // Вспомогательная функция
    Table& getTable(const std::string& table, std::string& error);
    const Table& getTableConst(const std::string& table, std::string& error) const;
};
