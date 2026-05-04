#include "ordering_system.h"
#include "priority_system.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

using namespace std;

int main() {

    //this will start the timer
    auto programStart = chrono::steady_clock::now();

    //This will take name, price, priority, priorityRate timeToComplete
    //timeToComplete will be the amount of seconds the item takes to complete

    MenuSystem menu;
    menu.addMenuItem("BURGER",  MenuItem("Cheeseburger", 5.99, 10, 3, 3));
    menu.addMenuItem("FRIES",  MenuItem("Fries", 2.49, 4, 2, 1));
    menu.addMenuItem("WATER",  MenuItem("Water", 1.99, 2, 1, 1));
    menu.addMenuItem("PIZZA",  MenuItem("Pizza", 8.99, 15, 4, 5));
    menu.addMenuItem("SODA",  MenuItem("Soda", 1.49, 3, 1, 1));
    menu.addMenuItem("SALAD",  MenuItem("Salad", 4.99, 6, 2, 2));
    menu.addMenuItem("ICECREAM",  MenuItem("Ice-Cream", 3.49, 15, 3, 2));

    PrioritySystem ps;

    //This will print the menu so the viewer can see whats available and their details
    //We can delete this later if needed
    cout <<"---Restaurant---\n\n";
    menu.printMenu();

    //Just the stndard orders, we can change these add more or less
    cout << "\n Placing orders..\n\n";
    ps.placeOrder("Alice", {menu.getMenuItem("PIZZA"),
                           menu.getMenuItem("SODA"),
                           menu.getMenuItem("FRIES")}, false);
    ps.placeOrder("Ava", {menu.getMenuItem("BURGER"),
                           menu.getMenuItem("FRIES")}, false);
    ps.placeOrder("Adam", {menu.getMenuItem("PIZZA")}, true);
    ps.placeOrder("Erik", {menu.getMenuItem("ICECREAM"),
                           menu.getMenuItem("SODA"),
                           menu.getMenuItem("FRIES")}, false);
    ps.placeOrder("Eve", {menu.getMenuItem("BURGER")}, true);
    ps.placeOrder("Frank", {menu.getMenuItem("SALAD"),
                           menu.getMenuItem("WATER")}, false);
    ps.placeOrder("Leo", {menu.getMenuItem("BURGER"),
                           menu.getMenuItem("PIZZA"),
                           menu.getMenuItem("WATER")}, false);
    ps.placeOrder("Max", {menu.getMenuItem("SALAD"),
                           menu.getMenuItem("SODA"),
                           menu.getMenuItem("FRIES")}, true);
    ps.placeOrder("Jason", {menu.getMenuItem("WATER"),
                           menu.getMenuItem("SODA")}, false);
    ps.placeOrder("Leslie", {menu.getMenuItem("PIZZA"),
                           menu.getMenuItem("SODA")}, true);

    //will show the queue before proccessing the orders
    cout << "\n---Inital Queue---\n";
    ps.printQueue();

    //this will process all of the 10 orders(unless we change the amount) and 
    //each item will pause depending on how big the order is

    cout << "\n-- Processing the Orders --\n";

    for (int i = 0; i < 10; i++){
        //this will help advance the time(tick)
        ps.time();

        //updates the priorities based on the priorityRate
        ps.updateQueue();

        //calculates the total time to complete the order
        int timeToComplete = 0;
        if (!ps.getOrderQueue().empty()){
            for(const auto& item : ps.getOrderQueue().top().items){
                timeToComplete += item.timeToComplete;
            }
        }
        cout <<"\n|Order " << (i + 1) << " of 10|";
        cout << "Time to prepare " << timeToComplete << " second(s)\n";

        //pause depending on the total time to complete order function
        this_thread::sleep_for(chrono::seconds(timeToComplete));

        //updates the current time by timeToComplete
        for (int t = 0; t < timeToComplete - 1; t++){
            ps.time();
        }

        //this just serves the next order
        ps.processNextOrder();

        //show the remaining queue
        ps.printQueue();

    }

    //This gives how long the program took to run
    auto programEnd = chrono::steady_clock::now();
    int totalRuntime = chrono::duration_cast<chrono::seconds> (programEnd - programStart).count();

    cout << "\n-- Done --";
    cout << "Total program runtime was: " << totalRuntime << " seconds\n";

    return 0;


}