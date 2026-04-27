#include "ordering_system.h"

#include <iostream>

int main() {
    // testing the menu item object/functions
    OrderingSystem test;

    std::cout << "Testing Menu Items: \n";
    test.addMenuItem("WATER", MenuItem("Water", 1.99, 2));
    test.addMenuItem("BURGER", MenuItem("Cheeseburger", 5.99, 10));
    test.addMenuItem("FRIES", MenuItem("Fries", 2.49, 4));

    test.printMenu();

    return 0;
}