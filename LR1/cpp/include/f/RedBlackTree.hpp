#pragma once
#include <string>

enum Color { RED, BLACK };

struct RBNode {
    int value;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
};

struct RBTree {
    RBNode* root;
};

void initTree(RBTree &tree);

void insert(RBTree &tree, int val);
bool remove(RBTree &tree, int val);

RBNode* find(RBTree &tree, int val);

void printTree(RBTree &tree); 
void inorderPrint(RBTree &tree); 

void readFromFile(RBTree &tree, const std::string &filename);
void writeToFile(RBTree &tree, const std::string &filename);

void clearTree(RBTree &tree);
