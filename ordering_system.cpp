#include "ordering_system.h"
#include <iostream>
#include <iomanip>

// menu item specific functions

MenuItem::MenuItem(std::string name, double price, int priority, int priorityRate, int timeToComplete)
    : name(name), price(price), priority(priority), priorityRate(priorityRate), timeToComplete(timeToComplete){
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
        << std::setw(17) << "Name"
        << std::setw(8) << "Price"
        << std::setw(10) <<"Priority"
        << std::setw(15)<<"Priority Rate"
        <<"Time to Complete\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& [key, item] : menu_) {
        std::cout << std::left
            << std::setw(14) << key
            << std::setw(17) << item.name
            << "$" << std::setw(10) << std::fixed << std::setprecision(2) << item.price
            << std::setw(14) << item.priority << std::setw(15) << item.priorityRate
            << std::setw(15) << item.timeToComplete << "\n";


    }
    std::cout << "\n";
}