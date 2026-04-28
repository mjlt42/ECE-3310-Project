#include "ordering_system.h"
#include <iostream>
#include <iomanip>

// menu item specific functions

MenuItem::MenuItem(std::string name, double price, int priority)
    : name(name), price(price), priority(priority) {
}

// creates an menu item with a name, price, and priority value and attaches a key as an identifier
void MenuSystem::addMenuItem(const std::string& key, const MenuItem& item) {
    menu_[key] = item;
}

// fetches MenuItem using its identifier (should be useful when creating orders)
MenuItem& MenuSystem::getMenuItem(const std::string& key) {
    auto index = menu_.find(key);
    if (index == menu_.end()) {
        std::cout << "Menu Item not found: " + key;
    }
    return index->second; // this points to the reference that index is at and returns the MenuItem
}

void MenuSystem::printMenu() {
    std::cout << "Menu Items: \n";
    std::cout << std::left
        << std::setw(14) << "Key"
        << std::setw(20) << "Name"
        << std::setw(8) << "Price"
        << "Priority\n";
    std::cout << std::string(50, '-') << "\n";

    for (const auto& [key, item] : menu_) {
        std::cout << std::left
            << std::setw(14) << key
            << std::setw(20) << item.name
            << "$" << std::setw(7) << std::fixed << std::setprecision(2) << item.price
            << item.priority << "\n";
    }
    std::cout << "\n";
}