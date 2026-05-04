#ifndef ordering_system_h
#define ordering_system_h

#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>


// MenuItem object 
struct MenuItem {
    std::string name;
    double price;
    int priority; //priority pts
    int priorityRate; //priority pts per unit time
    int timeToComplete;

    MenuItem() = default;
    MenuItem(std::string name, double price, int priority, int priorityRate, int timeToComplete);
};
//hello

class MenuSystem {
private:
    // apparently this just kinda makes a hash map am i even allowed to use this?
    std::unordered_map<std::string, MenuItem> menu_;
public:
    void addMenuItem(const std::string& key, const MenuItem& item);
    MenuItem& getMenuItem(const std::string& key);
    void printMenu();
};

#endif