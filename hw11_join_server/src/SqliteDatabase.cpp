#include "SqliteDatabase.h"
#include <stdexcept>
#include <iostream>

SqliteDatabase::SqliteDatabase(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK) {
        throw std::runtime_error("Cannot open SQLite database");
    }

    const char* createA =
        "CREATE TABLE IF NOT EXISTS A ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL);";

    const char* createB =
        "CREATE TABLE IF NOT EXISTS B ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL);";

    char* err = nullptr;
    sqlite3_exec(db_, createA, nullptr, nullptr, &err);
    sqlite3_exec(db_, createB, nullptr, nullptr, &err);
    if (err) sqlite3_free(err);
}

SqliteDatabase::~SqliteDatabase() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool SqliteDatabase::insert(const std::string& table,
                            int id,
                            const std::string& name,
                            std::string& error)
{
    std::string sql = "INSERT INTO " + table + " (id, name) VALUES (?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        error = "prepare failed";
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error = "duplicate " + std::to_string(id);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool SqliteDatabase::truncate(const std::string& table,
                              std::string& error)
{
    std::string sql = "DELETE FROM " + table + ";";

    char* err = nullptr;
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        error = "truncate failed";
        if (err) sqlite3_free(err);
        return false;
    }

    return true;
}

ResultRows SqliteDatabase::intersection() const {
    ResultRows rows;

    const char* sql =
        "SELECT A.id, A.name, B.name "
        "FROM A JOIN B ON A.id = B.id "
        "ORDER BY A.id;";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);

        const unsigned char* a = sqlite3_column_text(stmt, 1);
        const unsigned char* b = sqlite3_column_text(stmt, 2);

        rows.emplace_back(
            id,
            a ? reinterpret_cast<const char*>(a) : "",
            b ? reinterpret_cast<const char*>(b) : ""
        );
    }

    sqlite3_finalize(stmt);
    return rows;
}

ResultRows SqliteDatabase::symmetricDifference() const {
    ResultRows rows;

    const char* sql =
        "SELECT id, name, '' FROM A WHERE id NOT IN (SELECT id FROM B) "
        "UNION ALL "
        "SELECT id, '', name FROM B WHERE id NOT IN (SELECT id FROM A) "
        "ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);

        const unsigned char* a = sqlite3_column_text(stmt, 1);
        const unsigned char* b = sqlite3_column_text(stmt, 2);

        rows.emplace_back(
            id,
            a ? reinterpret_cast<const char*>(a) : "",
            b ? reinterpret_cast<const char*>(b) : ""
        );
    }

    sqlite3_finalize(stmt);
    return rows;
}
