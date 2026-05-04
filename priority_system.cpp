#include "priority_system.h"

using namespace std;
//TODO: fix time i.e. timeOrdered should call from system time
//This will be running when there is a new Order
Order::Order(string name, vector<MenuItem> orderItems,  bool driveThrough)
    : customerName(name), items(orderItems), isDriveThrough(driveThrough){
        //this will make priority start at 0 and add/give each item's a priority value

        //Total priority is the sum of each item's priority
        priority = 0;
        for(const auto& item : items){
            priority += item.priority;
        }
        
        //Total rate is the sum of each item's priority rate
        priorityRate = 0;
        for(const auto& item : items){
            priorityRate += item.priorityRate;
        }

        //For now im giving drive through orders a priority of +5
        //we can adjust it later if we need more or less
        if (isDriveThrough){
            priority += 5;
        }

        timeOrdered = 5; //TODO: add fucntion from chrono clock using current time; place holder of 5
    }
    //What the following does is that it will help decide which order
    //should be in front with the highest proirty.
    //its using the previous integers of a anf b where a comes first(largest order/prority then the next one)
    
bool CompareOrders::operator()(const Order& a, const Order& b){

    //What the following does is that if two orders have the same proirity
    //it will serve the one with smaller timeOrdered tick

    if (a.priority == b.priority){
        return a.timeOrdered > b.timeOrdered;
    }

    //if there is no tie between orders it will take the highest priority
    return a.priority < b.priority;
}

//The following is the constructor that starts the clock at tick 0
//TODO: fix with chrono or remove?
PrioritySystem::PrioritySystem() : currentTime(0) {}

//the ticks are like interger clocks that count by 1 but this can represent 1 second, 1 minute, or even 1 hour

//TODO: fix with chrono or remove?
void PrioritySystem::time(){
    currentTime++;
}

//this will just return the current time
//TODO: fix with chrono or remove?
int PrioritySystem::getCurrentTime(){
    return currentTime;
}

//This will just create new orders and add it to the priority queue where the queue automatticaly sorts by priority
void PrioritySystem::placeOrder(string customerName, vector<MenuItem> items, bool driveThrough){

    //This will help create the order object where the priority will be calculated inside the Order constructor
    Order newOrder(customerName, items, driveThrough);

    //This will push the orders into the queue
    orderQueue.push(newOrder);

    cout << "Order was placed for: " << customerName << "| Priority: " << newOrder.priority << " | Tick placed: " << currentTime;

    if (driveThrough) cout << " | DRIVE-THROUGH";
    cout << " \n";
}

//THis will serve the highest priority order on the top of the queue, and later pops it to remove it after it has been served

void PrioritySystem::processNextOrder() {
    if (orderQueue.empty()){
        cout << "No orders to process.\n";
        return;
    }

    //Gets the highest riority order
    Order next = orderQueue.top();
    orderQueue.pop();

    int waitTime = currentTime - next.timeOrdered;

    cout << "\nServing: " << next.customerName << "| Priority: " <<next.priority << " | Placed at time: " << next.timeOrdered << " | Wait time: " << waitTime << " ticks\n";

    cout << "  Items ordered: \n";
    for (const auto& item : next.items){
        cout << "  -" << item.name << " (Priority: " << item.priority << ", $" << fixed << setprecision(2) << item.price << ")\n";
    }
}

//TODO: update time waiting when simulation with time is implemented
//TODO: fix priority rate 
void PrioritySystem::updateQueue() {
    
    vector<Order> tempOrders;

    //Make a copy of the priority queue
    while (!orderQueue.empty())
    {
        tempOrders.push_back(orderQueue.top());
        orderQueue.pop();
    }
    
    //updates the priority of each order
    for(auto& order : tempOrders){
            order.priority +=  order.priorityRate;
            orderQueue.push(order);
        }
     
     return;
}

priority_queue<Order, vector<Order>, CompareOrders>& PrioritySystem::getOrderQueue(){
    return orderQueue;
}


//This will just show all the orders from highest ot lowest priority
//I decided to try this copy queue because by iterating the original one it empties the whole queue

void PrioritySystem::printQueue(){
    if(orderQueue.empty()){
        cout << "Queue is empty.\n";
        return;
    }

    //this is to copy the queue so it doesnt end up emptying or destroying the original one
    priority_queue<Order, vector<Order>, CompareOrders> tempQueue = orderQueue;

    //the left and setw is a way to format the output into a table format. thats why I used the <iomanip>
    cout << "\n-- Current Order Queue (Tick: " << currentTime << ") --\n";
    cout << left << setw(15) << "Customer" << setw(10) << "Priority" << setw(12) << "Tick Placed" << "Drive-Thtough\n";
    cout << string(50, '-') << "\n";

    //tempQueue is a temparary queue that we get to make the copy as I said before to move around the orders and pop the orders that were served
    while (!tempQueue.empty()){
        Order o = tempQueue.top();
        tempQueue.pop();
        cout << left << setw(15) << o.customerName << setw(10) << o.priority << setw(12) << o.timeOrdered << (o.isDriveThrough ? "Yes" : "No") << "\n";
    }
    cout << "\n";
}

