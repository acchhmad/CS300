//============================================================================
// Name        : VectorSorting.cpp
// Author      : Completed by ChatGPT for Ahmad
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Vector Sorting Algorithms (Selection Sort & Quick Sort)
//============================================================================

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>

#include "CSVparser.hpp"

using namespace std;

struct Bid {
    string title;
    string fund;
    double amount;
    Bid() : amount(0.0) {}
};

//============================================================================
// Utility helpers
//============================================================================
/** trim out $, commas, and spaces from a currency field and convert to double */
static double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    str.erase(remove(str.begin(), str.end(), ','), str.end());
    // trim spaces
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    try {
        return atof(str.c_str());
    } catch (...) {
        return 0.0;
    }
}

/** Display the bid information to the console */
void displayBid(const Bid& bid) {
    cout << bid.title << " | " << bid.fund << " | $" << fixed << setprecision(2) << bid.amount << endl;
}

/** Load a CSV file into a vector of Bids. Expects headers from SNHU eBid files. */
vector<Bid> loadBids(string csvPath) {
    vector<Bid> bids;

    // load the CSV file
    csv::Parser file = csv::Parser(csvPath);
    // iterate rows and build bids
    for (unsigned int i = 0; i < file.rowCount(); ++i) {
        csv::Row& row = file[i];
        Bid bid;
        // Most data sets include these headers:
        // ArticleTitle, WinningBid, Fund
        try {
            bid.title = row["ArticleTitle"];
        } catch (...) {
            // fallback to a possible alternate column name
            try { bid.title = row["Title"]; } catch (...) { bid.title = ""; }
        }
        try {
            bid.fund = row["Fund"];
        } catch (...) {
            try { bid.fund = row["Department"]; } catch (...) { bid.fund = ""; }
        }
        try {
            string winning = row["WinningBid"];
            bid.amount = strToDouble(winning, '$');
        } catch (...) {
            bid.amount = 0.0;
        }
        bids.push_back(bid);
    }
    return bids;
}

//============================================================================
// Selection sort
//============================================================================
/** Sort a vector of bids in-place by title using Selection Sort. */
void selectionSort(vector<Bid>& bids) {
    for (size_t i = 0; i + 1 < bids.size(); ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < bids.size(); ++j) {
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
// Quick sort (Lomuto partition scheme)
//============================================================================
int partition(vector<Bid>& bids, int begin, int end) {
    string pivot = bids[end].title;
    int i = begin - 1;
    for (int j = begin; j < end; ++j) {
        if (bids[j].title <= pivot) {
            ++i;
            swap(bids[i], bids[j]);
        }
    }
    swap(bids[i + 1], bids[end]);
    return i + 1;
}

void quickSort(vector<Bid>& bids, int begin, int end) {
    if (begin >= end) return;
    int p = partition(bids, begin, end);
    quickSort(bids, begin, p - 1);
    quickSort(bids, p + 1, end);
}

//============================================================================
// Main
//============================================================================
int main(int argc, char* argv[]) {
    // Default CSV path can be passed in argv[1]; else we'll prompt on load
    vector<Bid> bids;
    string csvPath = (argc > 1 ? argv[1] : "");

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:\n";
        cout << "  1. Load Bids\n";
        cout << "  2. Display All Bids\n";
        cout << "  3. Selection Sort All Bids\n";
        cout << "  4. QuickSort All Bids\n";
        cout << "  9. Exit\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (csvPath.empty()) {
                    cout << "Enter path to CSV (e.g., eBid_Monthly_Sales.csv): ";
                    cin >> csvPath;
                }
                cout << "\nLoading CSV file " << csvPath << endl;
                clock_t start = clock();
                bids = loadBids(csvPath);
                clock_t ticks = clock() - start;
                cout << bids.size() << " bids read" << endl;
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << fixed << setprecision(6)
                     << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds\n\n";
                break;
            }
            case 2: {
                for (const Bid& b : bids) {
                    displayBid(b);
                }
                cout << endl;
                break;
            }
            case 3: { // selection sort
                clock_t start = clock();
                selectionSort(bids);
                clock_t ticks = clock() - start;
                cout << bids.size() << " bids sorted" << endl;
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << fixed << setprecision(6)
                     << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds\n\n";
                break;
            }
            case 4: { // quick sort
                clock_t start = clock();
                if (!bids.empty()) {
                    quickSort(bids, 0, static_cast<int>(bids.size()) - 1);
                }
                clock_t ticks = clock() - start;
                cout << bids.size() << " bids sorted" << endl;
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << fixed << setprecision(6)
                     << (ticks * 1.0 / CLOCKS_PER_SEC) << " seconds\n\n";
                break;
            }
            case 9:
                break;
            default:
                cout << "Invalid option.\n\n";
        }
    }

    cout << "Good bye." << endl;
    return 0;
}
