#ifndef priority_system_h
#define priority_system_h

#pragma once
#include "ordering_system.h"
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <iomanip>
using namespace std;


struct Order{
    string customerName;
    vector<MenuItem> items;
    bool isDriveThrough;

    int priority;
    int priorityRate;
    int timeOrdered;

    //constructor that wil help build the order
    //it also helps calculate the priority
    Order(string name, vector<MenuItem> items, bool driveThrough = false);
};

//this helps place two orders in the priority queue
struct CompareOrders {
    bool operator()(const Order& a, const Order& b);
};

//this will help manage the order queue and intger tick timer

class PrioritySystem {
    private:
        //this will keep the highes priority order in front
        priority_queue<Order, vector<Order>, CompareOrders> orderQueue;

        //A integer clock
        int currentTime;//TODO: fix with chrono or remove?

    public:
    //Constructor
        PrioritySystem();
    
        //moves the clock by 1
        void time();

        //returns the current clock value
        int getCurrentTime();

        //this adds a new order to the queue with the current time
        void placeOrder(string customerName, vector<MenuItem> items, bool driveThrough = false);
        
        //this will serve and move onto the next priority order from the queue
        void processNextOrder();

        //this will update the priorities of each item order
        void updateQueue();

        //prints the orders in the queue from highest priority to the lowest
        void printQueue();

        priority_queue<Order, vector<Order>, CompareOrders>& getOrderQueue();

};




#endif

