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

// =======================
// Инициализация
// =======================
void initTree(RBTree &tree);

// =======================
// Операции
// =======================
void insert(RBTree &tree, int val);
bool remove(RBTree &tree, int val);
RBNode* find(RBTree &tree, int val);
void inorderPrint(RBTree &tree);

// =======================
// Работа с файлами
// =======================
void readFromFile(RBTree &tree, const std::string &filename);
void writeToFile(RBTree &tree, const std::string &filename);

// =======================
// Очистка
// =======================
void clearTree(RBTree &tree);