#include "Database.h"

using ResultRow = std::tuple<int, std::string, std::string>;
using ResultRows = std::vector<ResultRow>;

bool Database::insert(const std::string &table, int id, const std::string &name, std::string &error)
{
    auto& tmap = getTable(table, error);
    if (!error.empty()) return false;

    if(tmap.count(id)){
        error = "duplicate " + std::to_string(id);
        return false;
    }
    tmap[id] = name;
    return true;
}

bool Database::truncate(const std::string &table, std::string &error)
{
    auto& tmap = getTable(table, error);
    if (!error.empty()) return false;
    tmap.clear();
    return true;
}

ResultRows Database::intersection() const {
    ResultRows result;

    std::string errA, errB;
    const auto& A = getTableConst("A", errA);
    const auto& B = getTableConst("B", errB);

    if (!errA.empty() || !errB.empty())
        return result;

    auto itA = A.begin();
    auto itB = B.begin();

    while (itA != A.end() && itB != B.end()) {
        if (itA->first == itB->first) {
            // id совпал → пересечение
            result.emplace_back(
                itA->first,
                itA->second,
                itB->second
            );
            ++itA;
            ++itB;
        }
        else if (itA->first < itB->first) {
            ++itA;
        }
        else {
            ++itB;
        }
    }

    return result;
}


ResultRows Database::symmetricDifference() const {
    ResultRows result;

    std::string errA, errB;
    const auto& A = getTableConst("A", errA);
    const auto& B = getTableConst("B", errB);

    if (!errA.empty() || !errB.empty())
        return result;

    auto itA = A.begin();
    auto itB = B.begin();

    while (itA != A.end() && itB != B.end()) {
        if (itA->first == itB->first) {
            // id есть в обеих таблицах → пропускаем
            ++itA;
            ++itB;
        }
        else if (itA->first < itB->first) {
            // id есть только в A
            result.emplace_back(
                itA->first,
                itA->second,
                ""              // нет значения в B
            );
            ++itA;
        }
        else {
            // id есть только в B
            result.emplace_back(
                itB->first,
                "",
                itB->second     // нет значения в A
            );
            ++itB;
        }
    }

    // Добавляем хвост A
    while (itA != A.end()) {
        result.emplace_back(
            itA->first,
            itA->second,
            ""
        );
        ++itA;
    }

    // Добавляем хвост B
    while (itB != B.end()) {
        result.emplace_back(
            itB->first,
            "",
            itB->second
        );
        ++itB;
    }

    return result;
}

Table &Database::getTable(const std::string &table, std::string &error)
{
    if (table == "A") return tableA_;
    if (table == "B") return tableB_;
    error = "unknown table " + table;
    static Table dummy;
    return dummy;
}

const Table &Database::getTableConst(const std::string &table, std::string &error) const
{
    if (table == "A") return tableA_;
    if (table == "B") return tableB_;
    error = "unknown table " + table;
    static const Table dummy;
    return dummy;
}