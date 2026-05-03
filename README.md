# ECE-3310-Project
Repository for storing our files for our ECE 3310 project

## Contributors
- [Osvaldo Perez](https://github.com/OsvaldoPerez43)
- [Frederick Kamdon](https://github.com/styrofoambowlingball)
- [Matthew Lopez Tarsky](https://github.com/mjlt42)

## Abstract
Our project is a Restaurant Ordering System. When a customer places an order, their order is added to the queue, and its position in the queue is determined by its priority value. The priority value of an order is determined by the predetermined priority value of each individual item (i.e. the priority value of an order is the sum of the priority value of item A, item B, etc.). The queue is arranged in order of highest priority value to lowest priority value; The queue would have to be updated in real time or periodically. There are other factors such as drivethough, time, time limit, etc. 

For the data structures we would use, we can use a priority queue, since a normal queue can not be sorted based on a value. The queue handles an order object, which will store the order details (items, name, time placed, etc.) Also, we can use a dictionary/hash table to store the menu items (along with the priority values, priority inflation rates, etc.) Finally, we suspect we might have to refine or change how we determine the priority and how it changes based on testing.

## UML Diagram

## Work Divisions
We will be dividing the project in three sections, one will work on the priority system, the list/queue, and the hash table. 
- The person to work on the List/Queue will be responsible for the build of the Priority Queue
- The priority system will be responsible for the sorting and timing of the algorithms.
- The person working on the hash table will be responsible for Menu systems such as customer orders
