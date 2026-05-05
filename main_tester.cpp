#include "ordering_system.h"
#include "priority_system.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <random>
#include <unordered_map>

using namespace std;

int main() {

    //this will start the timer
    auto programStart = chrono::steady_clock::now();

    //This will take name, price, priority, priorityRate timeToComplete
    //timeToComplete will be the amount of unit time the item takes to complete

    MenuSystem menu;
        //Key, name, price, priority, priority rate, time to complete
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

    //Creates 2 random number generators
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> distr1(0, 9);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> distr2(0, 6);

    //Stores names and keys in arrays
    string names[10] ={"Alice", "Ava", "Adam", "Erik", "Eve", "Frank","Leo","Max", "Jason", "Leslie"};
    string keys[7] = {"BURGER","FRIES","WATER","PIZZA","SODA","SALAD","ICECREAM"};

    int const NUMBCUSTOMER = 10; //Number of customers
    unordered_map<string, bool> usedNames; //hash map of usednames (prevents duplicates)
    
    for (int i = 0; i < NUMBCUSTOMER; i++){
    int random1 = distr1(gen1);
    int random2 = distr2(gen2);

    while (usedNames.contains(names[random1]))//Checks if name has been used
    {
        random1 = distr1(gen1);
    }
    //Places order and randomly picks drivethrough
    ps.placeOrder(names[random1], {menu.getMenuItem(keys[random2])}, (random1%2 == 0));
    usedNames[names[random1]] = random1;
    }

    //Just the stndard orders, we can change these add more or less
    /*
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
    */
    //will show the queue before proccessing the orders
    cout << "\n---Inital Queue---\n";
    ps.printQueue();
    
    //this will process all of the 10 orders(unless we change the amount) and 
    //each item will pause depending on how big the order is

    int size = ps.getOrderQueue().size();
    vector<int> timeVec{size};
    cout << "\n-- Processing the Orders --\n";

    for (int i = 0; i < size; i++){
        

        //updates the priorities based on the priorityRate
        
        if (i != 0) ps.updateQueue(timeVec[i-1]);

        //calculates the total time to complete the order
        int TimeToComplete = 0;
        if (!ps.getOrderQueue().empty()){
            TimeToComplete = ps.getOrderQueue().top().timeToComplete;
        }
        std::cout <<"\n|Order " << (i + 1) << " of 10|";
        cout << "Time to prepare " << TimeToComplete << " milliseconds(s)\n";

        //pause depending on the total time to complete order function
        this_thread::sleep_for(chrono::milliseconds(TimeToComplete));
        
        //updates the current time by timeToComplete
        for (int t = 0; t < TimeToComplete ; t++){
            ps.time();
        }

        //appends the the waittime for each order into a vector
        timeVec.push_back(ps.getCurrentTime() - ps.getOrderQueue().top().timeOrdered);
        //this just serves the next order
        ps.processNextOrder();
        //TODO: work in progress, added to make it seem like a customer can order at random
        if (distr1(gen1) == 1) ps.placeOrder(names[distr1(gen1)], {menu.getMenuItem(keys[distr2(gen2)])}, (distr1(gen1)%2 == 0));

        //show the remaining queue
        ps.printQueue();

    }

    //This gives how long the program took to run
    auto programEnd = chrono::steady_clock::now();
    int totalRuntime = chrono::duration_cast<chrono::milliseconds> (programEnd - programStart).count();

    cout << "\n-- Done --";
    cout << "Total program runtime was: " << totalRuntime << " milliseconds\n";

    int avgWaitTime = 0;
     for(auto& time: timeVec){
            avgWaitTime += time;
        }
    avgWaitTime = avgWaitTime/size;
    cout << "Average Wait Time: " << avgWaitTime << " milliseconds\n";

    return 0;

    
}