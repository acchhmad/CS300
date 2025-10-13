
//============================================================================
// Name        : VectorSorting.cpp
// Author      : Ahmad Mansour
// Course      : CS-300
// Description : Load bids from CSV and sort by title using Selection Sort and Quick Sort
//============================================================================

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Data model
//============================================================================
struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;
    Bid() : amount(0.0) {}
};

// Forward declarations
static double strToDouble(string str, char ch);
static void displayBid(const Bid& bid);
static void loadBids(const string& csvPath, vector<Bid>& bids);

// Sorting prototypes to match the starter API
void selectionSort(vector<Bid>& bids);
void quickSort(vector<Bid>& bids, int begin, int end);
int  partition(vector<Bid>& bids, int begin, int end);

//============================================================================
// Helpers
//============================================================================
static double strToDouble(string str, char ch) {
    // strip a single char (e.g., '$') and commas, then convert
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    return atof(str.c_str());
}

static void displayBid(const Bid& bid) {
    cout << bid.bidId << ": " << bid.title << " | "
         << fixed << setprecision(2) << bid.amount << " | "
         << bid.fund << endl;
}

static void loadBids(const string& csvPath, vector<Bid>& bids) {
    cout << "Loading CSV file " << csvPath << endl;

    clock_t ticks = clock();

    // The starter Parser supports header lookups by column name
    csv::Parser file = csv::Parser(csvPath);

    for (unsigned int i = 0; i < file.rowCount(); ++i) {
        Bid bid;
        // Common SNHU eBid columns (Id, Title, Fund, Amount)
        bid.bidId = file[i][0];
        bid.title = file[i][1];
        bid.fund  = file[i][8];               // Fund is column 8 in the provided CSV
        bid.amount = strToDouble(file[i][4], '$');  // Amount is column 4, strip currency
        bids.push_back(bid);
    }

    ticks = clock() - ticks;
    cout << file.rowCount() << " bids read" << endl;
    cout << "time: " << ticks << " clock ticks" << endl;
    cout << "time: " << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds" << endl << endl;
}

//============================================================================
// Selection Sort (by title)
//============================================================================
void selectionSort(vector<Bid>& bids) {
    size_t n = bids.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (bids[j].title < bids[minIdx].title) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(bids[i], bids[minIdx]);
        }
    }
}

//============================================================================
// Quicksort (by title)
//============================================================================
int partition(vector<Bid>& bids, int begin, int end) {
    // pivot at middle element (to avoid worst case on sorted data)
    int mid = begin + (end - begin) / 2;
    string pivot = bids[mid].title;

    int left = begin;
    int right = end;

    while (true) {
        while (bids[left].title < pivot)  ++left;
        while (bids[right].title > pivot) --right;

        if (left >= right) return right;

        swap(bids[left], bids[right]);
        ++left;
        --right;
    }
}

void quickSort(vector<Bid>& bids, int begin, int end) {
    if (begin >= end) return;

    int p = partition(bids, begin, end);
    quickSort(bids, begin, p);
    quickSort(bids, p + 1, end);
}

//============================================================================
// Main
//============================================================================
int main(int argc, char* argv[]) {
    string csvPath;
    if (argc == 2) {
        csvPath = argv[1];
    } else {
        csvPath = "eBid_Monthly_Sales.csv"; // default to the larger file
    }

    vector<Bid> bids;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. QuickSort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                bids.clear();
                loadBids(csvPath, bids);
                break;
            }
            case 2: {
                for (const auto& b : bids) {
                    displayBid(b);
                }
                cout << endl;
                break;
            }
            case 3: {
                clock_t ticks = clock();
                selectionSort(bids);
                ticks = clock() - ticks;
                cout << bids.size() << " bids sorted" << endl;
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds" << endl << endl;
                break;
            }
            case 4: {
                if (!bids.empty()) {
                    clock_t ticks = clock();
                    quickSort(bids, 0, static_cast<int>(bids.size()) - 1);
                    ticks = clock() - ticks;
                    cout << bids.size() << " bids sorted" << endl;
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds" << endl << endl;
                }
                break;
            }
            case 9:
                break;
            default:
                cout << "Invalid option." << endl << endl;
                break;
        }
    }

    cout << "Good bye." << endl;
    return 0;
}
