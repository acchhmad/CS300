
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to hold course information
struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};

// Node for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// BST class to store and manage courses
class CourseBST {
private:
    Node* root;

    void addNode(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        } else if (course.courseNumber < node->course.courseNumber) {
            addNode(node->left, course);
        } else {
            addNode(node->right, course);
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseName << endl;
            inOrder(node->right);
        }
    }

    Course* search(Node* node, string courseNumber) {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return &node->course;
        if (courseNumber < node->course.courseNumber)
            return search(node->left, courseNumber);
        else
            return search(node->right, courseNumber);
    }

    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    CourseBST() : root(nullptr) {}
    ~CourseBST() { destroy(root); }

    void Insert(Course course) { addNode(root, course); }

    void PrintCourseList() {
        if (root == nullptr) {
            cout << "No data loaded. Please load the data structure first." << endl;
            return;
        }
        cout << "\nHere is a sample schedule:\n" << endl;
        inOrder(root);
        cout << endl;
    }

    void PrintCourseInfo(string courseNumber) {
        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
        Course* course = search(root, courseNumber);

        if (course == nullptr) {
            cout << "Course not found." << endl;
            return;
        }

        cout << course->course.courseNumber << ", " << course->course.courseName << endl;
        if (course->course.prerequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        } else {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course->course.prerequisites.size(); ++i) {
                cout << course->course.prerequisites[i];
                if (i < course->course.prerequisites.size() - 1)
                    cout << ", ";
            }
            cout << endl;
        }
    }
};

// Function to load courses from a CSV file
void LoadCourses(string filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, courseName;
        string prereq;

        getline(ss, courseNumber, ',');
        getline(ss, courseName, ',');

        Course course;
        course.courseNumber = courseNumber;
        course.courseName = courseName;

        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }

        bst.Insert(course);
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
}

int main() {
    CourseBST bst;
    string userInput;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "\nWhat would you like to do? ";
        cin >> userInput;

        try {
            choice = stoi(userInput);
        } catch (...) {
            cout << "\n" << userInput << " is not a valid option." << endl;
            continue;
        }

        if (choice == 1) {
            cin.ignore();
            cout << "\nEnter the file name to load (press Enter for default 'ABCU_Advising_Program_Input.csv'): ";
            string filename;
            getline(cin, filename);

            if (filename.empty())
                filename = "ABCU_Advising_Program_Input.csv";

            LoadCourses(filename, bst);
            dataLoaded = true;

        } else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            } else {
                bst.PrintCourseList();
            }

        } else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            } else {
                cout << "\nWhat course do you want to know about? ";
                string courseNumber;
                cin >> courseNumber;
                bst.PrintCourseInfo(courseNumber);
            }

        } else if (choice == 9) {
            cout << "\nThank you for using the course planner!" << endl;
            break;

        } else {
            cout << "\n" << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
