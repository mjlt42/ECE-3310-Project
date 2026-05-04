#include "ordering_system.h"
#include "priority_system.h"
#include <iostream>

int main() {
    // testing the menu item object/functions
    MenuSystem test;

    std::cout << "Testing Menu Items: \n";
    test.addMenuItem("WATER", MenuItem("Water", 1.99, 10, 5));
    test.addMenuItem("BURGER", MenuItem("Cheeseburger", 5.99, 2, 3));
    test.addMenuItem("FRIES", MenuItem("Fries", 2.49, 4, 6));
    test.addMenuItem("SODA", MenuItem("Soda", 1.49, 10, 5));

    test.printMenu();
    
    PrioritySystem q;
    q.placeOrder("Paul", {test.getMenuItem("FRIES"), test.getMenuItem("WATER") }, true);
    q.placeOrder("Noah", {test.getMenuItem("SODA"), test.getMenuItem("BURGER") }, false);
    q.printQueue();
    q.processNextOrder();
    q.printQueue();
    

    return 0;
}