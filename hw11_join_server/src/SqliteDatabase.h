#pragma once
#include "IDatabase.h"
#include <sqlite3.h>

class SqliteDatabase : public IDatabase {
public:
    explicit SqliteDatabase(const std::string& filename);
    ~SqliteDatabase() override;

    bool insert(const std::string& table,
                int id,
                const std::string& name,
                std::string& error) override;

    bool truncate(const std::string& table,
                  std::string& error) override;

    ResultRows intersection() const override;
    ResultRows symmetricDifference() const override;

private:
    sqlite3* db_ = nullptr;
};
