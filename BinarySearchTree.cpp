#include <iostream>
#include "CSVparser.hpp"
using namespace std;

/*
    BinarySearchTree.cpp
    Implementation of a simple binary search tree to store bids by bidId.
*/

struct Node {
    Bid bid;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Bid aBid) : Node() {
        this->bid = aBid;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    Node* removeNode(Node* node, string bidId);
    void inOrder(Node* node);
    void preOrder(Node* node);
    void postOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();

    void InOrder();
    void PreOrder();
    void PostOrder();

    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/* Constructor */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/* Destructor */
BinarySearchTree::~BinarySearchTree() {
    if (root != nullptr) {
        std::function<void(Node*)> freeTree = [&](Node* node) {
            if (node != nullptr) {
                freeTree(node->left);
                freeTree(node->right);
                delete node;
            }
        };
        freeTree(root);
        root = nullptr;
    }
}

/* Insert a bid into the tree */
void BinarySearchTree::Insert(Bid bid) {
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        addNode(root, bid);
    }
}

/* Add node helper: recursively find where to place a new bid */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    if (bid.bidId < node->bid.bidId) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }
}

/* Search for a specific bid by ID */
Bid BinarySearchTree::Search(string bidId) {
    Node* current = root;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        else if (bidId < current->bid.bidId) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    Bid bid;
    return bid;
}

/* Remove a bid by ID */
void BinarySearchTree::Remove(string bidId) {
    root = removeNode(root, bidId);
}

/* Recursive remove helper */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    if (node == nullptr) {
        return node;
    }

    if (bidId < node->bid.bidId) {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId > node->bid.bidId) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}

/* In-order traversal */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/* Pre-order traversal */
void BinarySearchTree::PreOrder() {
    preOrder(root);
}

/* Post-order traversal */
void BinarySearchTree::PostOrder() {
    postOrder(root);
}

/* Recursive in-order traversal helper */
void BinarySearchTree::inOrder(Node* node) {
    if (node == nullptr) return;
    inOrder(node->left);
    cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
    inOrder(node->right);
}

/* Recursive pre-order traversal helper */
void BinarySearchTree::preOrder(Node* node) {
    if (node == nullptr) return;
    cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
    preOrder(node->left);
    preOrder(node->right);
}

/* Recursive post-order traversal helper */
void BinarySearchTree::postOrder(Node* node) {
    if (node == nullptr) return;
    postOrder(node->left);
    postOrder(node->right);
    cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
}
