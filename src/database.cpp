#include "database.h"
#include <iostream>

// Конструктор: открытие базы данных
Database::Database(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

// Деструктор: закрытие базы данных
Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

// Выполнение запроса без возврата результата
bool Database::execute(const std::string& query) {
    char* errmsg;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errmsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        return false;
    }
    return true;
}

// Выполнение запроса с параметрами
bool Database::execute(const std::string& query, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Получение списка товаров
std::vector<Item> Database::queryItems(const std::string& query) const {
    std::vector<Item> items;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return items;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Item item;
        item.id = sqlite3_column_int(stmt, 0);
        item.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        item.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        item.quantity = sqlite3_column_int(stmt, 3);
        item.price = sqlite3_column_double(stmt, 4);
        items.push_back(item);
    }

    sqlite3_finalize(stmt);
    return items;
}
