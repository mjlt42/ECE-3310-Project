#include "ordering_system.h"
#include "priority_system.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <array>
#include <iomanip>

using namespace std;

int main() {

    // -------------------------------------------------------
    // Start the overall program timer
    // -------------------------------------------------------
    auto programStart = chrono::steady_clock::now();

    // -------------------------------------------------------
    // Setup Menu
    // -------------------------------------------------------
    MenuSystem menu;
    menu.addMenuItem("BURGER",   MenuItem("Cheeseburger", 5.99, 10, 3));
    menu.addMenuItem("FRIES",    MenuItem("Fries",        2.49,  4, 2));
    menu.addMenuItem("WATER",    MenuItem("Water",        1.99,  2, 1));
    menu.addMenuItem("PIZZA",    MenuItem("Pizza",        8.99, 15, 4));
    menu.addMenuItem("SODA",     MenuItem("Soda",         1.49,  3, 1));
    menu.addMenuItem("SALAD",    MenuItem("Salad",        4.99,  6, 2));
    menu.addMenuItem("ICECREAM", MenuItem("Ice Cream",    3.49, 15, 3));

    PrioritySystem ps;

    // -------------------------------------------------------
    // Arrays to store order time data
    // -------------------------------------------------------
    const int NUM_CUSTOMERS = 10;

    // Stores customer names in order they were placed
    array<string, NUM_CUSTOMERS> customerNames = {
        "Alice", "Bob", "Carol", "Dave", "Eve",
        "Frank", "Grace", "Hank", "Ivy", "Jack"
    };

    // orderTimes[i][0] = tick when placed
    // orderTimes[i][1] = real seconds when placed
    // orderTimes[i][2] = tick when served
    // orderTimes[i][3] = real seconds when served
    // orderTimes[i][4] = total real wait in seconds
    array<array<int, 5>, NUM_CUSTOMERS> orderTimes = {};

    // -------------------------------------------------------
    // Place 10 prebuilt orders and record placement times
    // -------------------------------------------------------
    cout << "=== RESTAURANT ORDERING SYSTEM ===\n\n";
    cout << "Loading 10 customers...\n\n";

    // This is the reference point for all time measurements
    auto simStart = chrono::steady_clock::now();

    // Helper lambda to record placement time for each customer
    // This avoids repeating the chrono code 10 times
    auto recordPlacement = [&](int index) {
        orderTimes[index][0] = ps.getCurrentTime();
        orderTimes[index][1] = chrono::duration_cast<chrono::seconds>
                               (chrono::steady_clock::now() - simStart).count();
    };

    // Place all 10 orders
    // Priority = sum of item priorities + 5 if drive-through
    ps.placeOrder("Alice", {menu.getMenuItem("PIZZA"),    menu.getMenuItem("SODA")},    false); // 18
    recordPlacement(0);

    ps.placeOrder("Bob",   {menu.getMenuItem("BURGER"),   menu.getMenuItem("FRIES")},   false); // 14
    recordPlacement(1);

    ps.placeOrder("Carol", {menu.getMenuItem("PIZZA")},                                 true);  // 20 w/ drive-through
    recordPlacement(2);

    ps.placeOrder("Dave",  {menu.getMenuItem("ICECREAM"), menu.getMenuItem("SODA")},    false); // 18
    recordPlacement(3);

    ps.placeOrder("Eve",   {menu.getMenuItem("BURGER")},                                true);  // 15 w/ drive-through
    recordPlacement(4);

    ps.placeOrder("Frank", {menu.getMenuItem("SALAD"),    menu.getMenuItem("WATER")},   false); // 8
    recordPlacement(5);

    ps.placeOrder("Grace", {menu.getMenuItem("FRIES"),    menu.getMenuItem("SODA")},    false); // 7
    recordPlacement(6);

    ps.placeOrder("Hank",  {menu.getMenuItem("PIZZA"),    menu.getMenuItem("BURGER")},  false); // 25
    recordPlacement(7);

    ps.placeOrder("Ivy",   {menu.getMenuItem("WATER"),    menu.getMenuItem("SODA")},    false); // 5
    recordPlacement(8);

    ps.placeOrder("Jack",  {menu.getMenuItem("WATER")},                                 false); // 2 LOWEST
    recordPlacement(9);

    // -------------------------------------------------------
    // Show initial queue before any processing
    // -------------------------------------------------------
    cout << "\n=== Initial Queue (All 10 Customers) ===\n";
    ps.printQueue();

    // -------------------------------------------------------
    // Process all 10 orders one per tick
    // Program pauses 2 seconds per order to simulate cooking
    // -------------------------------------------------------
    cout << "\n=== Processing Orders ===\n";

    for (int i = 0; i < NUM_CUSTOMERS; i++) {

        // Advance the integer tick clock
        ps.time();

        // Update all priorities using each item's priorityRate
        ps.updateQueue();

        cout << "\n[Order " << (i + 1) << " of " << NUM_CUSTOMERS << "]\n";
        cout << "Preparing order - please wait...\n";

        // Pause 2 real seconds to simulate cooking time
        this_thread::sleep_for(chrono::seconds(2));

        // Record the real time this order was served
        int servedSeconds = chrono::duration_cast<chrono::seconds>
                           (chrono::steady_clock::now() - simStart).count();

        // Serve the highest priority order
        ps.processNextOrder();

        // Store served time data in the array at index i
        orderTimes[i][2] = ps.getCurrentTime(); // tick served
        orderTimes[i][3] = servedSeconds;        // real seconds when served
        orderTimes[i][4] = servedSeconds - orderTimes[i][1]; // total wait

        // Show remaining queue after serving
        ps.printQueue();
    }

    // -------------------------------------------------------
    // Print full summary table from the arrays
    // -------------------------------------------------------
    cout << "\n=== ORDER TIME SUMMARY ===\n";
    cout << left
         << setw(10) << "Customer"
         << setw(13) << "Tick Placed"
         << setw(13) << "Sec Placed"
         << setw(13) << "Tick Served"
         << setw(13) << "Sec Served"
         << setw(13) << "Wait(secs)" << "\n";
    cout << string(75, '-') << "\n";

    // Track who waited longest
    int longestWait = 0;
    string longestWaitName = "";

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        cout << left
             << setw(10) << customerNames[i]
             << setw(13) << orderTimes[i][0]   // tick placed
             << setw(13) << orderTimes[i][1]   // real secs placed
             << setw(13) << orderTimes[i][2]   // tick served
             << setw(13) << orderTimes[i][3]   // real secs served
             << setw(13) << orderTimes[i][4]   // wait time
             << "\n";

        if (orderTimes[i][4] > longestWait) {
            longestWait    = orderTimes[i][4];
            longestWaitName = customerNames[i];
        }
    }

    // -------------------------------------------------------
    // Final results
    // -------------------------------------------------------
    auto programEnd = chrono::steady_clock::now();
    int totalRuntime = chrono::duration_cast<chrono::seconds>
                      (programEnd - programStart).count();

    cout << "\n=== FINAL RESULTS ===\n";
    cout << "Customer who waited longest: " << longestWaitName
         << " | Wait: " << longestWait << " seconds\n";
    cout << "Total program runtime:       " << totalRuntime << " seconds\n";

    return 0;
}