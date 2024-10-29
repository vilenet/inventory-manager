#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "item.h"
#include "sqlite3.h"

class Database {
public:
    Database(const std::string& db_name);
    ~Database();

    bool execute(const std::string& query);  // Выполнить SQL-запрос без возврата результата
    bool execute(const std::string& query, const std::vector<std::string>& params); // Запрос с параметрами
    std::vector<Item> queryItems(const std::string& query) const; // Получение списка товаров

private:
    sqlite3* db;
};

#endif // end DATABASE_H