#include "inventory_manager.h"

InventoryManager::InventoryManager(const std::string& db_name) : db(db_name) {
    db.execute("CREATE TABLE IF NOT EXISTS items ("
               "id INTEGER PRIMARY KEY, "
               "name TEXT, "
               "category TEXT, "
               "quantity INTEGER, "
               "price REAL);");
}

void InventoryManager::addItem(const std::string& name, const std::string& category, int quantity, double price) {
    std::string query = "INSERT INTO items (name, category, quantity, price) VALUES (?, ?, ?, ?);";
    db.execute(query, {name, category, std::to_string(quantity), std::to_string(price)});
}

void InventoryManager::editItem(int id, const std::string& name, const std::string& category, int quantity, double price) {
    std::string query = "UPDATE items SET name = ?, category = ?, quantity = ?, price = ? WHERE id = ?;";
    db.execute(query, {name, category, std::to_string(quantity), std::to_string(price), std::to_string(id)});
}

void InventoryManager::deleteItem(int id) {
    std::string query = "DELETE FROM items WHERE id = ?;";
    db.execute(query, {std::to_string(id)});
}

std::vector<Item> InventoryManager::getItems() const {
    return db.queryItems("SELECT id, name, category, quantity, price FROM items;");
}

Item InventoryManager::searchItemByName(const std::string& name) const {
    auto items = db.queryItems("SELECT id, name, category, quantity, price FROM items WHERE name = '" + name + "';");
    return items.empty() ? Item{} : items[0];
}
