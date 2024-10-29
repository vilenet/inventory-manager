#ifndef UI_H
#define UI_H

#include "inventory_manager.h"

class UI {
public:
    UI(InventoryManager& manager);
    void run();

private:
    InventoryManager& manager;
    void displayMenu();
    void viewInventory();
    void addItem();
    void editItem();  // Обновлено: добавлен параметр int id
    void deleteItem(); // Обновлено: добавлен параметр int id
    void searchItem();
};

#endif // end UI_H