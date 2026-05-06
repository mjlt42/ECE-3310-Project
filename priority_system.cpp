#include "priority_system.h"

using namespace std;
//This will be running when there is a new Order
Order::Order(string name, vector<MenuItem> orderItems, bool driveThrough)
    : customerName(name), items(orderItems), isDriveThrough(driveThrough) {
    //this will make priority start at 0 and add/give each item's a priority value

    //Total priority is the sum of each item's priority
    //Total rate is the sum of each item's priority rate
    //Total time to complete is the sum of each item's time to complete
    priority = 0;
    priorityRate = 0;
    timeToComplete = 0;
    for (const auto& item : items) {
        priority += item.priority;
        priorityRate += item.priorityRate;
        timeToComplete += item.timeToComplete;
    }


    //Priority bias can be adjusted
    if (isDriveThrough) {
        priority += 5;
    }

    timeOrdered = 0;
}

//What the following does is that it will help decide which order
//should be in front with the highest proirty.
//its using the previous integers of a anf b where a comes first(largest order/prority then the next one)  
bool CompareOrders::operator()(const Order& a, const Order& b) {

    //What the following does is that if two orders have the same proirity
    //it will serve the one with smaller timeOrdered tick

    if (a.priority == b.priority) {
        return a.timeOrdered > b.timeOrdered;
    }

    //if there is no tie between orders it will take the highest priority
    return a.priority < b.priority;
}

//The following is the constructor that starts the clock at tick 0
PrioritySystem::PrioritySystem() : currentTime{ 0 } {}
//the ticks are like interger clocks that count by 1 but this can represent 1 second, 1 minute, or even 1 hour


void PrioritySystem::time() {
    currentTime++;
}

//this will just return the current time
int PrioritySystem::getCurrentTime() {
    return currentTime;
}

//This will just create new orders and add it to the priority queue where the queue automatticaly sorts by priority
void PrioritySystem::placeOrder(string customerName, vector<MenuItem> items, bool driveThrough) {

    //This will help create the order object where the priority will be calculated inside the Order constructor
    Order newOrder(customerName, items, driveThrough);
    newOrder.timeOrdered = currentTime;

    //This will push the orders into the queue
    orderQueue.push(newOrder);
    if (newOrder.priority >= 10) {
        cout << "Order was placed for: " << left << setw(7) << customerName << setw(14) << " | Priority: " << newOrder.priority << setw(17) << " | Tick placed: " << currentTime;
    }
    else
        cout << "Order was placed for: " << left << setw(7) << customerName << setw(15) << " | Priority: " << newOrder.priority << setw(17) << " | Tick placed: " << currentTime;
    if (driveThrough) cout << " | DRIVE-THROUGH";
    cout << " \n";
}

//THis will serve the highest priority order on the top of the queue, and later pops it to remove it after it has been served

void PrioritySystem::processNextOrder() {
    if (orderQueue.empty()) {
        cout << "No orders to process.\n";
        return;
    }

    //Gets the highest riority order
    Order next = orderQueue.top();
    orderQueue.pop();

    int waitTime = currentTime - next.timeOrdered;

    cout << "\nServing: " << next.customerName << " | Priority: " << next.priority << " | Placed at time: " << next.timeOrdered << " | Wait time: " << waitTime << " ticks\n";

    cout << "  Items ordered: \n";
    for (const auto& item : next.items) {
        cout << "  -" << item.name << " (Priority: " << item.priority << ", $" << fixed << setprecision(2) << item.price << ")\n";
    }
}


void PrioritySystem::updateQueue(int deltaTime) {

    vector<Order> tempOrders;

    //Make a copy of the priority queue
    while (!orderQueue.empty())
    {
        tempOrders.push_back(orderQueue.top());
        orderQueue.pop();
    }

    //updates the priority of each order
    for (auto& order : tempOrders) {
        order.priority += order.priorityRate * deltaTime;
    }
    orderQueue = priority_queue<Order, vector<Order>, CompareOrders>(tempOrders.begin(), tempOrders.end());

    return;
}

priority_queue<Order, vector<Order>, CompareOrders>& PrioritySystem::getOrderQueue() {
    return orderQueue;
}


//This will just show all the orders from highest to lowest priority

void PrioritySystem::printQueue() {
    if (orderQueue.empty()) {
        cout << "Queue is empty.\n";
        return;
    }

    //this is to copy the queue so it doesnt end up emptying or destroying the original one
    priority_queue<Order, vector<Order>, CompareOrders> tempQueue = orderQueue;

    //setw is a way to format the output into a table format.
    cout << "\n-- Current Order Queue (Tick: " << currentTime << ") --\n";
    cout << left << setw(15) << "Customer" << setw(10) << "Priority" << setw(12) << "Tick Placed" << "Drive-Through\n";
    cout << string(50, '-') << "\n";

  
    while (!tempQueue.empty()) {
        Order o = tempQueue.top();
        tempQueue.pop();
        cout << left << setw(15) << o.customerName << setw(10) << o.priority << setw(12) << o.timeOrdered << (o.isDriveThrough ? "Yes" : "No") << "\n";
    }
    cout << "\n";
}

