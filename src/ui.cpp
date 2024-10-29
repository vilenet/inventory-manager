#include "ui.h"
#include <pdcurses.h>
#include <iostream>
#include <string>

UI::UI(InventoryManager& manager) : manager(manager) {}

void UI::run() {
    initscr();           // Инициализация ncurses
    noecho();
    cbreak();
    keypad(stdscr, TRUE); // Разрешение на использование клавиш-стрелок

    displayMenu();

    endwin();            // Завершение работы ncurses
}

// Параметры таблицы
#define ROWS_PER_PAGE 5  // Количество строк на экране

void UI::displayMenu() {
    int choice = 0;
    int key;
    const int numOptions = 6;
    const char* options[numOptions] = {
        "View Inventory",
        "Add New Item",
        "Edit Item",
        "Delete Item",
        "Search Item",
        "Exit"
    };

    do {
        clear();
        // Рисуем рамку вокруг окна и выводим заголовок
        box(stdscr, 0, 0);
        mvprintw(1, (COLS - 20) / 2, "Inventory Management");

        // Отображаем меню с подсветкой выбранного элемента
        for (int i = 0; i < numOptions; ++i) {
            if (i == choice) {
                attron(A_REVERSE); // Подсветка выбранного пункта
                mvprintw(3 + i, (COLS - 20) / 2, options[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, (COLS - 20) / 2, options[i]);
            }
        }
        refresh();

        key = getch();
        switch (key) {
            case KEY_UP:
                choice = (choice - 1 + numOptions) % numOptions;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % numOptions;
                break;
            case '\n': // Обработка нажатия Enter
                switch (choice) {
                    case 0: viewInventory(); break;
                    case 1: addItem(); break;
                    case 2: editItem(); break;
                    case 3: deleteItem(); break;
                    case 4: searchItem(); break;
                    case 5: return; // Выход из программы
                }
                break;
        }
    } while (choice != 5);
}

void UI::viewInventory() {
    clear();
    printw("ID | Name | Category | Qty | Price\n");
    for (const auto& item : manager.getItems()) {
        printw("%d | %s | %s | %d | %.2f\n", item.id, item.name.c_str(), item.category.c_str(), item.quantity, item.price);
    }
    printw("\nPress any key to return to the main menu...");
    getch();
}

void UI::addItem() {
    int height = 10, width = 40;
    int starty = (LINES - height) / 2;  // Центрируем окно по вертикали
    int startx = (COLS - width) / 2;    // Центрируем окно по горизонтали

    WINDOW* win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Add New Item");

    char name[50], category[50];
    int quantity;
    double price;

    echo();
    mvwprintw(win, 3, 1, "Enter item name: ");
    wgetstr(win, name);

    mvwprintw(win, 4, 1, "Enter category: ");
    wgetstr(win, category);

    mvwprintw(win, 5, 1, "Enter quantity: ");
    wscanw(win, "%d", &quantity);

    mvwprintw(win, 6, 1, "Enter price: ");
    wscanw(win, "%lf", &price);

    noecho();

    // Добавляем товар через InventoryManager
    manager.addItem(name, category, quantity, price);

    mvwprintw(win, 8, 1, "Item added successfully!");
    mvwprintw(win, 9, 1, "Press any key to continue...");
    wrefresh(win);
    wgetch(win);  // Ждем, пока пользователь нажмет клавишу

    delwin(win);  // Удаляем окно после выполнения действия
}

void UI::editItem() {
    clear();
    printw("Edit Item\n");

    int id;
    printw("Enter ID of the item to edit: ");
    scanw("%d", &id);

    // Проверка наличия товара с данным ID
    auto items = manager.getItems();
    bool found = false;
    for (const auto& item : items) {
        if (item.id == id) {
            found = true;
            break;
        }
    }

    if (!found) {
        printw("\nItem with ID %d not found!\nPress any key to return to the main menu...", id);
        getch();
        return;
    }

    char name[50], category[50];
    int quantity;
    double price;

    printw("Enter new item name: ");
    getstr(name);

    printw("Enter new category: ");
    getstr(category);

    printw("Enter new quantity: ");
    scanw("%d", &quantity);

    printw("Enter new price: ");
    scanw("%lf", &price);

    // Обновляем товар через InventoryManager
    manager.editItem(id, name, category, quantity, price);

    printw("\nItem updated successfully!\nPress any key to return to the main menu...");
    getch();
}

void UI::deleteItem() {
    clear();
    printw("Delete Item\n");

    int id;
    printw("Enter ID of the item to delete: ");
    scanw("%d", &id);

    // Проверка наличия товара с данным ID
    auto items = manager.getItems();
    bool found = false;
    for (const auto& item : items) {
        if (item.id == id) {
            found = true;
            break;
        }
    }

    if (!found) {
        printw("\nItem with ID %d not found!\nPress any key to return to the main menu...", id);
        getch();
        return;
    }

    // Удаляем товар через InventoryManager
    manager.deleteItem(id);

    printw("\nItem deleted successfully!\nPress any key to return to the main menu...");
    getch();
}

void UI::searchItem() {
    clear();
    printw("Search Item\n");

    char name[50];
    printw("Enter name of the item to search: ");
    getstr(name);

    auto items = manager.getItems();
    bool found = false;
    for (const auto& item : items) {
        if (item.name == name) {
            found = true;
            printw("\nID: %d\nName: %s\nCategory: %s\nQuantity: %d\nPrice: %.2f\n",
                   item.id, item.name.c_str(), item.category.c_str(), item.quantity, item.price);
            break;
        }
    }

    if (!found) {
        printw("\nItem with name '%s' not found!", name);
    }

    printw("\nPress any key to return to the main menu...");
    getch();
}