#include <iostream>
#include <iomanip>
#include <string>
#include "CSVparser.hpp"
using namespace std;

/*
    HashTable.cpp
    Implementation of a hash table using chaining with singly linked lists.
*/

struct Node {
    Bid bid;
    Node* next;

    Node() : next(nullptr) {}
    Node(Bid aBid) : bid(aBid), next(nullptr) {}
};

class HashTable {
private:
    static const unsigned int DEFAULT_SIZE = 179; // prime table size to reduce collisions
    Node** table;  // array of linked list heads
    unsigned int tableSize;

    unsigned int Hash(string key);
    void FreeTable();

public:
    HashTable();
    virtual ~HashTable();

    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/* Constructor: initialize hash table */
HashTable::HashTable() {
    tableSize = DEFAULT_SIZE;
    table = new Node * [tableSize];
    for (unsigned int i = 0; i < tableSize; ++i) {
        table[i] = nullptr;
    }
}

/* Destructor: free memory */
HashTable::~HashTable() {
    FreeTable();
}

/* Simple string hash function (djb2 variation) */
unsigned int HashTable::Hash(string key) {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return static_cast<unsigned int>(hash % tableSize);
}

/* Insert a bid into the hash table */
void HashTable::Insert(Bid bid) {
    unsigned int key = Hash(bid.bidId);
    Node* newNode = new Node(bid);

    if (table[key] == nullptr) {
        table[key] = newNode;
    } else {
        newNode->next = table[key];
        table[key] = newNode;
    }
}

/* Display all bids (bucket order) */
void HashTable::PrintAll() {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            cout << "Key " << i << ": "
                << current->bid.bidId << " | "
                << current->bid.title << " | "
                << fixed << setprecision(2) << current->bid.amount
                << " | " << current->bid.fund << endl;
            current = current->next;
        }
    }
}

/* Remove a bid by ID */
void HashTable::Remove(string bidId) {
    unsigned int key = Hash(bidId);
    Node* current = table[key];
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            if (previous == nullptr) {
                table[key] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            return;
        }
        previous = current;
        current = current->next;
    }
}

/* Search for a bid by ID */
Bid HashTable::Search(string bidId) {
    unsigned int key = Hash(bidId);
    Node* current = table[key];

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        current = current->next;
    }
    Bid emptyBid;
    return emptyBid; // not found
}

/* Free memory for all nodes */
void HashTable::FreeTable() {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    delete[] table;
}
