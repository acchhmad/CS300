
//============================================================================
// Name        : LinkedList.cpp
// Author      : Ahmad Mansour
// Version     : 1.0
// Copyright   : Copyright  2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Bid structure definition
//============================================================================
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() : amount(0.0) {}
};

// Forward declarations used by main
static Bid getBid();
static void displayBid(const Bid& bid);
static double strToDouble(string str, char ch);

//============================================================================
// Linked-List class definition
//============================================================================
class LinkedList {
private:
    // Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node* next;
        Node() : next(nullptr) {}
        Node(Bid aBid) : bid(aBid), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int    size;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid  Search(string bidId);
    int  Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Initialize housekeeping variables
    head = nullptr;
    tail = nullptr;
    size = 0;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // Start at the head and delete each node
    Node* cur = head;
    while (cur != nullptr) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = tail = nullptr;
    size = 0;
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // Create new node
    Node* node = new Node(bid);

    // if there is nothing at the head...
    if (head == nullptr) {
        // new node becomes the head and the tail
        head = tail = node;
    } else {
        // make current tail node point to the new node
        tail->next = node;
        // and tail becomes the new node
        tail = node;
    }
    // increase size count
    ++size;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Create new node
    Node* node = new Node(bid);

    // if there is already something at the head...
    if (head != nullptr) {
        // new node points to current head as its next node
        node->next = head;
    } else {
        // list was empty: this node is also the tail
        tail = node;
    }

    // head now becomes the new node
    head = node;
    // increase size count
    ++size;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // start at the head
    Node* cur = head;
    // while current node is not equal to nullptr
    while (cur != nullptr) {
        // output the current bid
        displayBid(cur->bid);
        // set current to the next node
        cur = cur->next;
    }
}

/**
 * Remove a specified bid
 */
void LinkedList::Remove(string bidId) {
    // if list is empty, nothing to do
    if (head == nullptr) return;

    // special case: removing the head
    if (head->bid.bidId == bidId) {
        Node* tmp = head;
        head = head->next;
        if (tmp == tail) {
            // it was the only node
            tail = nullptr;
        }
        delete tmp;
        --size;
        return;
    }

    // otherwise, search for the node keeping track of the previous
    Node* prev = head;
    Node* cur  = head->next;
    while (cur != nullptr) {
        if (cur->bid.bidId == bidId) {
            prev->next = cur->next;
            if (cur == tail) {
                tail = prev;
            }
            delete cur;
            --size;
            return;
        }
        prev = cur;
        cur  = cur->next;
    }
    // not found: nothing removed
}

/**
 * Search for the specified bid
 */
Bid LinkedList::Search(string bidId) {
    // Start at the head
    Node* cur = head;
    // keep searching until current node not equal to nullptr
    while (cur != nullptr) {
        // if the current node matches, return it
        if (cur->bid.bidId == bidId) {
            return cur->bid;
        }
        // otherwise go to the next node
        cur = cur->next;
    }
    // not found: return an empty bid
    return Bid();
}

/**
 * Returns size
 */
int LinkedList::Size() {
    return size;
}

/**
 * Load bids from a CSV file into the provided list.
 */
void loadBids(string csvPath, LinkedList& list) {
    cout << "Loading CSV file " << csvPath << endl;

    clock_t ticks = clock();

    csv::Parser file = csv::Parser(csvPath);

    // Assume headers: Id, Title, Fund, Amount
    for (unsigned int i = 0; i < file.rowCount(); ++i) {
        Bid bid;
        bid.bidId = file[i][0];
        bid.title = file[i][1];
        bid.fund  = file[i][8];   // some datasets use column 8 for Fund
        string amountStr = file[i][4];
        // remove $ and commas
        bid.amount = strToDouble(amountStr, '$');

        list.Append(bid);
    }

    ticks = clock() - ticks;
    cout << file.rowCount() << " bids read" << endl;
    cout << "time: " << ticks << " clock ticks" << endl;
    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // process command line arguments
    string csvPath;
    if (argc == 2) {
        csvPath = argv[1];
    } else {
        // default to the larger data set; can be changed by the grader
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a LinkedList to hold all bids
    LinkedList bidList;

    // Define a bid instance for user input/find results
    Bid bid;

    int choice = 0;
    string bidKey = "98109";

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin  >> choice;

        switch (choice) {
            case 1:
                bid = getBid();
                bidList.Prepend(bid);
                displayBid(bid);
                break;

            case 2:
                loadBids(csvPath, bidList);
                break;

            case 3:
                bidList.PrintList();
                break;

            case 4:
                ticks = clock();
                bid = bidList.Search(bidKey);
                ticks = clock() - ticks;

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                break;

            case 5:
                bidList.Remove(bidKey);
                break;

            case 9:
                break;

            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }

    cout << "Good bye." << endl;
    return 0;
}

/**
 * Display the bid information to the console (sample format)
 */
void displayBid(const Bid& bid) {
    cout << bid.bidId << ": " << bid.title << " | "
         << fixed << setprecision(2) << bid.amount << " | "
         << bid.fund << endl;
}

/**
 * Prompt user for a bid
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    getline(cin, bid.fund);

    cout << "Enter amount: ";
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Simple method to convert a string to a double
 * after stripping out unwanted char
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    return atof(str.c_str());
}
