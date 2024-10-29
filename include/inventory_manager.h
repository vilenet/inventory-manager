#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <string>
#include <vector>
#include "database.h"
#include "item.h"

class InventoryManager {
public:
    InventoryManager(const std::string& db_name);
    
    void addItem(const std::string& name, const std::string& category, int quantity, double price);
    void editItem(int id, const std::string& name, const std::string& category, int quantity, double price);
    void deleteItem(int id);
    std::vector<Item> getItems() const;
    Item searchItemByName(const std::string& name) const;

private:
    Database db;
};
#endif // end INVENTORY_MANAGER_H