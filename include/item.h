#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    int id;                 // Уникальный идентификатор товара
    std::string name;       // Название товара
    std::string category;   // Категория товара
    int quantity;           // Количество товара
    double price;           // Цена товара
};

#endif // end ITEM_H