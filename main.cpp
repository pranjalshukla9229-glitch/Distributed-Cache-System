#include <iostream>

#include "distributed_cache.h"

using namespace std;

void showMenu() {

    cout << "\n=========================================\n";
    cout << " DISTRIBUTED CACHE SYSTEM\n";
    cout << " LRU + CONSISTENT HASHING\n";
    cout << "=========================================\n";

    cout << "\nChoose Operation:\n\n";

    cout << "1. Add Cache Node\n";

    cout << "2. Remove Cache Node\n";

    cout << "3. PUT Key-Value\n";

    cout << "4. GET Value\n";

    cout << "5. Display System\n";

    cout << "6. Exit\n";

    cout << "\nEnter Choice: ";
}

int main() {

    distributed_cache dc;

    int choice;

    while (true) {

        showMenu();

        cin >> choice;

        /*
        =========================================
                ADD NODE
        =========================================
        */

        if (choice == 1) {

            string nodeId;

            int capacity;

            cout << "\nEnter Node ID: ";

            cin >> nodeId;

            cout << "Enter Cache Capacity: ";

            cin >> capacity;

            dc.add_node(nodeId, capacity);
        }

        /*
        =========================================
                REMOVE NODE
        =========================================
        */

        else if (choice == 2) {

            string nodeId;

            cout << "\nEnter Node ID to Remove: ";

            cin >> nodeId;

            dc.remove_node(nodeId);
        }

        /*
        =========================================
                PUT OPERATION
        =========================================
        */

        else if (choice == 3) {

            string key;

            int value;

            cout << "\nEnter Key: ";

            cin >> key;

            cout << "Enter Value: ";

            cin >> value;

            dc.put(key, value);
        }

        /*
        =========================================
                GET OPERATION
        =========================================
        */

        else if (choice == 4) {

            string key;

            cout << "\nEnter Key: ";

            cin >> key;

            int value = dc.get(key);

            cout << "\nReturned Value: "
                 << value
                 << "\n";
        }

        /*
        =========================================
                DISPLAY SYSTEM
        =========================================
        */

        else if (choice == 5) {

            dc.display();
        }

        /*
        =========================================
                    EXIT
        =========================================
        */

        else if (choice == 6) {

            cout << "\nExiting System...\n";

            break;
        }

        /*
        =========================================
                INVALID INPUT
        =========================================
        */

        else {

            cout << "\nInvalid Choice\n";
        }
    }

    return 0;
}