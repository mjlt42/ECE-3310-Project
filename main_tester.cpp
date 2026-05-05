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
    //(also i think we can just designate one device and one generator for all the random numbers instead of having 3)
    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> distr1(0, 9);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> distr2(0, 6);

    std::random_device rd3;
    std::mt19937 gen3(rd3());
    std::uniform_int_distribution<> itemCountDist(1, 3);

    //Stores names and keys in arrays
    string names[10] = { "Alice", "Ava", "Adam", "Erik", "Eve", "Frank","Leo","Max", "Jason", "Leslie" };
    string keys[7] = { "BURGER","FRIES","WATER","PIZZA","SODA","SALAD","ICECREAM" };


    //This will print the menu so the viewer can see whats available and their details
    //We can delete this later if needed
void printMainMenu(){
    cout << "\n";
    cout << "---Restaurant---             \n";
    cout << " 1. Run Simulation           \n";
    cout << " 2. Comparison of algorithms \n";
    cout << " 3. Exit                     \n";
    cout << "-----------------------------\n";
    cout << "Choose and option: ";
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

        int const NUMBCUSTOMER = 10; //Number of customers
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
    cout << "\n---Initial Queue---\n";
    ps.printQueue();

    //this will process all of the 10 orders(unless we change the amount) and 
    //each item will pause depending on how big the order is

    int size = ps.getOrderQueue().size();
    vector<int> timeVec{ size };
    cout << "\n-- Processing the Orders --\n";

    int count = 0;
    while (!ps.getOrderQueue().empty()) {


        //updates the priorities based on the priorityRate

        if (count != 0) ps.updateQueue(timeVec[count - 1]);

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
        ps.processNextOrder();
        //TODO: work in progress, added to make it seem like a customer can order at random
        if (distr1(gen1) == 1) ps.placeOrder(names[distr1(gen1)], { menu.getMenuItem(keys[distr2(gen2)]) }, (distr1(gen1) % 2 == 0));

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
    avgWaitTime = avgWaitTime / size;
    cout << "Average Wait Time: " << avgWaitTime << " milliseconds\n";
}
    //option 2 - Algoithim Comparison
    //
void runComparison(MenuSystem& menu){
    cout << "\n -- Algorithm Comparison --\n";

    //fixed orders this way comparison will be fair for both
    vector<pair<string, vector<string>>> orders={
        {"Alice", {"PIZZA", "SODA", "FRIES"}},
        {"Ava", {"BURGER", "FRIES"}},
        {"Adam", {"PIZZA"}},
        {"Erik", {"ICECREAM", "SODA", "WATER"}},
        {"Eve", {"BURGER", "SODA",}},
        {"Frank", {"SALAD", "WATER"}},
        {"Leo", {"FRIES", "SODA"}},
        {"Max", {"BURGER", "PIZZA", "SODA"}},
        {"Jason", {"WATER", "SODA"}},
        {"Leslie", {"WATER"}}

    };
    vector<bool> driveThroughs = {false, false, true, false, true, false, false, false, true, false};
cout << "-- Priority Queue --\n";
PrioritySystem ps;

for (int i = 0; i < 10; i++){
    vector<MenuItem> items;
    for (const auto& key : orders[i].second){
        items.push_back(menu.getMenuItem(key));
    }
    ps.placeOrder(orders[i].first, items, driveThroughs[i]);
}
vector<string> priorityOrder;
vector<int> PWait;
vector<int> PPriority;
int pqTick = 0;

while (!ps.getOrderQueue().empty()){
    ps.updateQueue(1);
    pqTick++;
    
    Order next = ps.getOrderQueue().top();
    int waitTime = pqTick - next.timeOrdered;

    priorityOrder.push_back(next.customerName);
    PWait.push_back(waitTime);
    PPriority.push_back(next.priority);

    ps.processNextOrder();
}
//This is the normal Priority
cout<<" -- Normal Priority Queue --\n";
queue<Order> NormalQueue;

for (int i = 0; i < 10; i++){
    vector<MenuItem> items;
    for (const auto& key : orders[i].second){
        items.push_back(menu.getMenuItem(key));
    }
    NormalQueue.push(Order(orders[i].first, items, driveThroughs[i]));
}

vector<string> normalOrder;
vector<int> NormalWaits;
vector<int> NormalPriorities;
int normalPos = 0;

while (!NormalQueue.empty()){
    Order next = NormalQueue.front();
    NormalQueue.pop();

    normalOrder.push_back(next.customerName);
    NormalWaits.push_back(normalPos);
    NormalPriorities.push_back(next.priority);
    normalPos++;
}
//This will be the side by side table
cout << "\n == Comparison Side-by-Side ==\n";
cout << left <<setw(5) << "Position" << setw(12) << "Priority Queue Name" << setw(10) << "Priority Queue priority" << setw(10) << "Priority Queue Wait" << setw(12) << "Normal Name" << setw(10) << "Normal Priority" << setw(10) << "Normal Wait" << "\n";
cout << string(69, '-') << "\n";

for (int i = 0; i < 10; i++){
    cout << left << setw(5) << (i + 1) << setw(12) << priorityOrder[i] << setw(10) << PPriority[i] << setw(10) << PWait[i] << setw(12) << normalOrder[i] << setw(10) << NormalPriorities[i] << setw(10) << NormalWaits[i] << "\n";
}

//Summary of the results
double pqAvg = 0, normalAvg = 0;
int pqMax = 0, normalMax = 0;
string pqMaxName, normalMaxName;
for (int i = 0; i < 10; i++){
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
pqAvg /= 10;
normalAvg /= 10;

cout <<"\n --Summary-- \n";
cout << left << setw(22) << "" << setw(18) << "Priority Queue" << setw(18) <<" Normal Queue" << "\n";
cout << string(58, '-') << "\n";

cout << left << setw(22) << "Avg wait time: " << setw(18) << pqAvg << setw(18) << normalAvg << "\n";

cout << left << setw(22) << "Longest Wait: " << setw(18) << (pqMaxName + "(" + to_string(pqMax) + ")") << setw(18) << (normalMaxName + "(" + to_string(normalMax) + ")") << "\n";

cout << left << setw(22) << "First Served: " << setw(18) << priorityOrder[0] << setw(18) << normalOrder[0] << "\n";

cout << left << setw(22) << "Last Served: " << setw(18) << priorityOrder[9] << setw(18) << normalOrder[9] << "\n";

cout << "\n --Conclusion-- \n";
cout << "Priority queue first served: " << priorityOrder[0] << " (priority: " << PPriority[0] <<" pts)\n";
cout << "Normal Queue first served:  " << normalOrder[0] << " (arrived first without priority) \n\n";
cout << "Priority Queue serves high value\n";
cout << "and drive-through orders first.\n";
cout << "Normal treats orders equally without high value of priority\n";



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
    while (choice !=3){
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
                cout << "\nClosing System BYE\n";
                break;
            default:
                cout << "Invalid option. Choose either 1, 2, or 3\n";
                break;
        }
    }
    return 0;
}