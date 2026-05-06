#include "ordering_system.h"
#include "priority_system.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <random>
#include <unordered_map>

#include <queue>

using namespace std;

//Creates 3 random number generators 
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> distr1(0, 14);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> distr2(0, 6);

    std::random_device rd3;
    std::mt19937 gen3(rd3());
    std::uniform_int_distribution<> itemCountDist(1, 3);

    //Stores names and keys in arrays
    string names[15] = { "Alice", "Ava", "Adam", "Erik", "Eve", "Frank","Leo","Max", "Jason", "Leslie" ,"Bob" , "Edwin", "Javier" , "Ashley" , "Ana"};
    string keys[7] = { "BURGER","FRIES","WATER","PIZZA","SODA","SALAD","ICECREAM" };


    //This will print the menu so the viewer can see whats available and their details
void printMainMenu(){
    cout << "\n";
    cout << "---Restaurant---             \n";
    cout << " 1. Run Simulation           \n";
    cout << " 2. Comparison of algorithms \n";
    cout << " 3. Oldest Orders First      \n";
    cout << " 4. Exit                      \n";
    cout << "-----------------------------\n";
    cout << "Choose an option: ";
}

    //option 1 simulation
    //

    void runSimulation(MenuSystem& menu){
        cout << "\n -- Simulation --\n\n";
        //this will start the timer
        auto programStart = chrono::steady_clock::now();

        //this will print the menu
        cout << " --Restuarant-- \n\n";
        menu.printMenu();
        PrioritySystem ps;

        int const NUMBCUSTOMER = 10;

    unordered_map<string, bool> usedNames; //hash map of usednames (prevents duplicates)

    for (int i = 0; i < NUMBCUSTOMER; i++) {
        int random1 = distr1(gen1);
        int random2 = distr2(gen2);

        int itemCount = itemCountDist(gen3);  //how many items in this order

        vector<MenuItem> orderItems;

        while (usedNames.contains(names[random1]))//Checks if name has been used
        {
            random1 = distr1(gen1);
        }

        for (int j = 0; j < itemCount; j++) {
            int randomKeyIndex = distr2(gen2);
            orderItems.push_back(menu.getMenuItem(keys[randomKeyIndex])); //adds another random menu item based on itemCount limit
        }

        //Places order and randomly picks drivethrough
        ps.placeOrder(names[random1], orderItems, (random1 % 2 == 0));
        usedNames[names[random1]] = random1;
    }


   
    //will show the queue before proccessing the orders
    cout << "\n---Initial Queue---\n";
    ps.printQueue();

    //this will process all of the 10 orders(unless we change the amount) and 
    //each item will pause depending on how big the order is

    int sizeIN = ps.getOrderQueue().size();
    vector<int> timeVec{ sizeIN };
    cout << "\n-- Processing the Orders --\n";

    int count = 0;
    while (!ps.getOrderQueue().empty()) {


        //updates the priorities based on the priorityRate


        //calculates the total time to complete the order
        int TimeToComplete = 0;
        if (!ps.getOrderQueue().empty()) {
            TimeToComplete = ps.getOrderQueue().top().timeToComplete;
        }
        std::cout << "\n|Order " << (count + 1) << "| " << endl;
        cout << "Time to prepare " << TimeToComplete << " millisecond(s)\n";

        //pause depending on the total time to complete order function
        this_thread::sleep_for(chrono::milliseconds(TimeToComplete));

        //updates the current time by timeToComplete
        for (int t = 0; t < TimeToComplete; t++) {
            ps.time();
        }

        //appends the the waittime for each order into a vector
        timeVec.push_back(ps.getCurrentTime() - ps.getOrderQueue().top().timeOrdered);
        //this just serves the next order
        if (count != 0) ps.updateQueue(TimeToComplete);
        ps.processNextOrder();
        if (distr1(gen1) == 1) {
          ps.placeOrder(names[distr1(gen1)], { menu.getMenuItem(keys[distr2(gen2)]) }, (distr1(gen1) % 2 == 0));
          sizeIN++; 
        }
        
        //show the remaining queue
        ps.printQueue();
        count++;
    }

    //This gives how long the program took to run
    auto programEnd = chrono::steady_clock::now();
    int totalRuntime = chrono::duration_cast<chrono::milliseconds> (programEnd - programStart).count();

    cout << "\n-- Done --\n";
    cout << "Total program runtime was: " << totalRuntime << " milliseconds\n";

    int avgWaitTime = 0;
    for (auto& time : timeVec) {
        avgWaitTime += time;
    }
    avgWaitTime = avgWaitTime / sizeIN;
    cout << "Average Wait Time: " << avgWaitTime << " milliseconds\n";
}


    //option 2 - Algoithim Comparison

void runComparison(MenuSystem& menu){
    auto start = chrono::steady_clock::now();
    cout << "\n -- Algorithm Comparison --\n";
    cout << " -- Comparing Priority Queue to Normal Queue --\n";
PrioritySystem ps;
queue<Order> NormalQueue;

int const NUMBCUSTOMER = 10;

unordered_map<string, bool> usedNames;
int tick = 0;

//this generates orders and adds to the queues
for (int i = 0; i < NUMBCUSTOMER; i++){
    int random1 = distr1(gen1);

    //randomly chooses food for the order
    int itemCount = itemCountDist(gen3);
    vector<MenuItem> orderItems;

    while(usedNames.contains(names[random1])){
        random1 = distr1(gen1);
    }

    for(int j = 0; j < itemCount; j++){
        int randomKeyIndex = distr2(gen2);
        orderItems.push_back(menu.getMenuItem(keys[randomKeyIndex]));
    }

    bool isDriveThrough = (random1 % 2 == 0);

    //place into priority Queue
    ps.placeOrder(names[random1], orderItems, isDriveThrough);
    ps.time();

    Order o(names[random1], orderItems, isDriveThrough);
    o.timeOrdered = tick;
    NormalQueue.push(o);

    usedNames[names[random1]] = true;
    tick++;
}
    
cout << "-- Priority Queue --\n";
vector<string> priorityOrder;
vector<int> PWait;
vector<int> PPriority;

int pqTick = tick;

while (!ps.getOrderQueue().empty()){
    Order next = ps.getOrderQueue().top();

    ps.processNextOrder();

    ps.updateQueue(next.timeToComplete);

    pqTick+= next.timeToComplete;

    int waitTime = pqTick - next.timeOrdered;

    this_thread::sleep_for(chrono::milliseconds(next.timeToComplete));

    if(distr1(gen1) == 1){
        int randName = distr1(gen1);
        while(usedNames.contains(names[randName])){
        randName = distr1(gen1);
    }

    string newName = names[randName];
    usedNames[newName] = true;

    vector<MenuItem> newItems = {menu.getMenuItem(keys[distr2(gen2)])};
    bool isDrive = (randName % 2 == 0);

    ps.placeOrder(newName, newItems, isDrive);

    Order o(newName, newItems, isDrive);
    o.timeOrdered = pqTick;
    NormalQueue.push(o);
    }

    priorityOrder.push_back(next.customerName);
    PWait.push_back(waitTime);
    PPriority.push_back(next.priority);

}
// random order
//This is the normal Priority
cout<<" -- Normal Priority Queue --\n";

vector<string> normalOrder;
vector<int> NormalWaits;
vector<int> NormalPriorities;

int normalTick = pqTick;

while (!NormalQueue.empty()){
    Order next = NormalQueue.front();
    NormalQueue.pop();

    normalTick += next.timeToComplete;

    int waitTime = normalTick - next.timeOrdered;
    this_thread::sleep_for(chrono::milliseconds(next.timeToComplete));

    normalOrder.push_back(next.customerName);
    NormalWaits.push_back(waitTime);
    NormalPriorities.push_back(next.priority);
}

int resultSize = min(priorityOrder.size(), normalOrder.size());

//This will be the side by side table
cout << "\n == Comparison Side-by-Side ==\n";
cout << left <<setw(10) << "Position" << setw(10) << "PQ Name" << setw(12) << "PQ Priority" << setw(10) << "PQ Wait |" << setw(12) << "Normal Name" << setw(16) << "Normal Priority" << setw(10) << "Normal Wait" << "\n";
cout << string(40, '-') << "|" << string(40, '-') << "\n";

for (int i = 0; i < resultSize; i++){
    cout << left << setw(10) << (i + 1) << setw(10) << priorityOrder[i] << setw(12) << PPriority[i] << setw(8) << PWait[i] << "| " << setw(12) << normalOrder[i] << setw(16) << NormalPriorities[i] << setw(10) << NormalWaits[i] << "\n";
}

//Summary of the results
double pqAvg = 0, normalAvg = 0;
int pqMax = 0, normalMax = 0;

string pqMaxName, normalMaxName;

for (int i = 0; i < resultSize; i++){
    pqAvg += PWait[i];
    normalAvg += NormalWaits[i];
    if(PWait[i] > pqMax){
        pqMax = PWait[i];
        pqMaxName = priorityOrder[i];
    }
    if(NormalWaits[i] > normalMax){
        normalMax = NormalWaits[i];
        normalMaxName = normalOrder[i];
    }
}
pqAvg /= resultSize;
normalAvg /= resultSize;

cout <<"\n --Summary-- \n";
cout << left << setw(22) << "" << setw(18) << "Priority Queue" << setw(18) <<" Normal Queue" << "\n";
cout << string(58, '-') << "\n";

cout << left << setw(22) << "Avg wait time: " << setw(18) << pqAvg << setw(18) << normalAvg << "\n";

cout << left << setw(22) << "Longest Wait: " << setw(18) << (pqMaxName + "(" + to_string(pqMax) + ")") << setw(18) << (normalMaxName + "(" + to_string(normalMax) + ")") << "\n";

cout << left << setw(22) << "First Served: " << setw(18) << priorityOrder[0] << setw(18) << normalOrder[0] << "\n";

cout << left << setw(22) << "Last Served: " << setw(18) << priorityOrder[resultSize-1] << setw(18) << normalOrder[resultSize-1] << "\n";

cout << "\n\n";
cout << "Priority queue first served: " << priorityOrder[0] << " (priority: " << PPriority[0] <<" pts)\n";
cout << "Normal Queue first served:  " << normalOrder[0] << " (arrived first without priority) \n\n";

auto end = chrono::steady_clock::now();
int runtime = chrono::duration_cast<chrono::milliseconds> (end-start).count();
cout << "Total comparison runtime: " << runtime << " milliseconds \n";


}

//The follwoing will be for comparing priority vs oldest orders first

void runOldestFirst(MenuSystem& menu){
    auto start = chrono::steady_clock::now();
    cout << "\n -- Oldest Orders First Algorithm --\n";
    cout<< " -- Comparing Priority Algorithm vs oldest Orders First --\n";

    //Generate Random orders
    int const NUMBCUSTOMER = 10;
    unordered_map<string, bool> usedNames;

    //this creates a temporary struct to hold our random generated data
    struct GeneratedOrder{
        string name;
        vector<MenuItem> items;
        bool isDriveThrough;
    };
    vector<GeneratedOrder> randomOrders;
    
    for(int i = 0; i < NUMBCUSTOMER; i++){
        int random1 = distr1(gen1);
        //checks for duplicates
        while(usedNames.contains(names[random1])){
            random1 = distr1(gen1);
        }
        string customerName = names[random1];
        usedNames[customerName]=true;

        //generates random items for this order
        int itemCount = itemCountDist(gen3);
        vector<MenuItem> orderItems;
        for (int j = 0; j < itemCount; j++){
            int randomKeyIndex = distr2(gen2);
            orderItems.push_back(menu.getMenuItem(keys[randomKeyIndex]));
        }

        //randomizes the drivethrough
        bool isDriveThrough = (distr1(gen1) % 2== 0);

        //saves the order to our list
        randomOrders.push_back({customerName, orderItems, isDriveThrough});
    }

PrioritySystem ps1;

for (int i = 0; i < NUMBCUSTOMER; i++){
    ps1.placeOrder(randomOrders[i].name, randomOrders[i].items, randomOrders[i].isDriveThrough);
    //each order will be placed at 1 tick apart
    ps1.time(); 
}
vector<string> P1Order;
vector<int> P1Wait;
vector<int> P1priority;
int P1Tick = NUMBCUSTOMER;

priority_queue<Order, vector<Order>, CompareByTime> timeQueue;
int tick = 0;

for (int i = 0; i < NUMBCUSTOMER; i++){
    Order o(randomOrders[i].name, randomOrders[i].items, randomOrders[i].isDriveThrough);
    //manually sets the tick so they differ
    o.timeOrdered = tick;
    timeQueue.push(o);
    tick++;
}

cout << "\n-- Priority Queue --\n";

while (!ps1.getOrderQueue().empty()){
    Order next = ps1.getOrderQueue().top();

    ps1.updateQueue(1);
    //increment clock by prep time of meal
    P1Tick+= next.timeToComplete;
    
    int waitTime = P1Tick - next.timeOrdered;
    this_thread::sleep_for(chrono::milliseconds(next.timeToComplete));

    P1Order.push_back(next.customerName);
    P1Wait.push_back(waitTime);
    P1priority.push_back(next.priority);

    ps1.processNextOrder();

        if(distr1(gen1) == 1){
        int randName = distr1(gen1);
        while(usedNames.contains(names[randName])){
        randName = distr1(gen1);
    }

    string newName = names[randName];
    usedNames[newName] = true;

    vector<MenuItem> newItems = {menu.getMenuItem(keys[distr2(gen2)])};
    bool isDrive = (randName % 2 == 0);

    ps1.placeOrder(newName, newItems, isDrive);

    Order o(newName, newItems, isDrive);
    o.timeOrdered = P1Tick;
    timeQueue.push(o);
    }
}
//This is the normal Priority
cout<<" -- Oldest Orders First--\n";

vector<string> timeOrder;
vector<int> timeWaits;
vector<int> timePriorities;

int timeTick = P1Tick;

while (!timeQueue.empty()){
    Order next = timeQueue.top();
    timeQueue.pop();
    //increments clock by prep time
    timeTick+= next.timeToComplete;

    int waitTime = timeTick - next.timeOrdered;
    this_thread::sleep_for(chrono::milliseconds(next.timeToComplete));

    timeOrder.push_back(next.customerName);
    timeWaits.push_back(waitTime);
    timePriorities.push_back(next.priority);
}

int resultSize = min(P1Order.size(), timeOrder.size());

//This will be the side by side table
cout << "\n == Comparison Side-by-Side ==\n";
cout << left <<setw(10) << "Position" << setw(10) << "PQ Name" << setw(12) << "PQ Priority" << setw(10) << "PQ Wait |" << setw(12) << "FCFS Name" << setw(16) << "FCFS Priority" << setw(10) << "FCFS Wait" << "\n";
cout << string(40, '-') << "|" << string(40, '-') << "\n";

for (int i = 0; i < resultSize; i++){
    cout << left << setw(10) << (i + 1) << setw(10) << P1Order[i] << setw(12) << P1priority[i] << setw(8) << P1Wait[i] << "| " << setw(12) << timeOrder[i] << setw(16) << timePriorities[i] << setw(10) << timeWaits[i] << "\n";
}

//Summary of the results
double p1Avg = 0, timeAvg = 0;
for (int i = 0; i < resultSize; i++){
    p1Avg += P1Wait[i];
    timeAvg += timeWaits[i];
}
p1Avg /= resultSize;
timeAvg /= resultSize;

cout <<"\n --Summary-- \n";
cout << left << setw(22) << "" << setw(18) << "Priority Queue" << setw(18) <<" Oldest-First" << "\n";
cout << string(58, '-') << "\n";

cout << left << setw(22) << "Avg wait time: " << setw(18) << p1Avg << setw(18) << timeAvg << "\n";

cout << left << setw(22) << "First Served: " << setw(18) << P1Order[0] << setw(18) << timeOrder[0] << "\n";

cout << left << setw(22) << "Last Served: " << setw(18) << P1Order[resultSize-1] << setw(18) << timeOrder[resultSize-1] << "\n";

cout << "\n\n";
cout << "Priority queue first served: " << P1Order[0] << " (priority: " << P1priority[0] <<" pts)\n";
cout << "Oldest First Queue first served:  " << timeOrder[0] << " (arrived first, priority: " << timePriorities[0] << "pts) \n\n";

auto end = chrono::steady_clock::now();
int runtime = chrono::duration_cast<chrono::milliseconds>(end-start).count();
cout << "Total comparison runtime: " <<runtime << " milliseconds\n";

}

int main() {

    //This will take name, price, priority, priorityRate timeToComplete
    //timeToComplete will be the amount of unit time the item takes to complete

    MenuSystem menu;
    //Key, name, price, priority, priority rate, time to complete
    menu.addMenuItem("BURGER", MenuItem("Cheeseburger", 5.99, 10, 3, 3));
    menu.addMenuItem("FRIES", MenuItem("Fries", 2.49, 4, 2, 1));
    menu.addMenuItem("WATER", MenuItem("Water", 1.99, 2, 1, 1));
    menu.addMenuItem("PIZZA", MenuItem("Pizza", 8.99, 15, 4, 5));
    menu.addMenuItem("SODA", MenuItem("Soda", 1.49, 3, 1, 1));
    menu.addMenuItem("SALAD", MenuItem("Salad", 4.99, 6, 2, 2));
    menu.addMenuItem("ICECREAM", MenuItem("Ice-Cream", 3.49, 15, 3, 2));

    int choice = 0;
    while (choice !=4){
        printMainMenu();
        cin >> choice;

        switch (choice){
            case 1:
                runSimulation(menu);
                break;
            case 2:
                runComparison(menu);
                break;
            case 3:
                runOldestFirst(menu);
                break;
            case 4:
                cout << "\nClosing System BYE\n";
                break;
            default:
                cout << "Invalid option. Choose either 1, 2, or 3\n";
                break;
        }
    }
    return 0;
}