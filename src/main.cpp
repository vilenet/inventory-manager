#include "inventory_manager.h"
#include "ui.h"

int main() 
{
    InventoryManager manager("inventory.db");
    UI ui(manager);
    ui.run();
    return 0;
}
