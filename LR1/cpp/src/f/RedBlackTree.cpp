#include "f/RedBlackTree.hpp"
#include <iostream>
#include <fstream>

static void leftRotate(RBTree &tree, RBNode* x);
static void rightRotate(RBTree &tree, RBNode* y);
static void insertFixup(RBTree &tree, RBNode* z);
static void transplant(RBTree &tree, RBNode* u, RBNode* v);
static void deleteFixup(RBTree &tree, RBNode* x);

void initTree(RBTree &tree) {
    tree.root = nullptr;
}

void insert(RBTree &tree, int val) {
    RBNode* z = new RBNode{val, RED, nullptr, nullptr, nullptr};
    RBNode* y = nullptr;
    RBNode* x = tree.root;
    while (x != nullptr) {
        y = x;
        if (z->value < x->value) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == nullptr) tree.root = z;
    else if (z->value < y->value) y->left = z;
    else y->right = z;
    insertFixup(tree, z);
}

RBNode* find(RBTree &tree, int val) {
    RBNode* cur = tree.root;
    while (cur != nullptr) {
        if (cur->value == val) return cur;
        if (val < cur->value) cur = cur->left;
        else cur = cur->right;
    }
    return nullptr;
}

bool remove(RBTree &tree, int val) {
    RBNode* z = find(tree, val);
    if (!z) return false;
    RBNode* y = z;
    Color yOriginalColor = y->color;
    RBNode* x = nullptr;
    if (!z->left) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = z->right;
        while (y->left) y = y->left;
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (yOriginalColor == BLACK && x) deleteFixup(tree, x);
    return true;
}

static void inorderRec(RBNode* node) {
    if (!node) return;
    inorderRec(node->left);
    std::cout << node->value << " ";
    inorderRec(node->right);
}

void inorderPrint(RBTree &tree) {
    inorderRec(tree.root);
    std::cout << std::endl;
}

static void clearRec(RBNode* node) {
    if (!node) return;
    clearRec(node->left);
    clearRec(node->right);
    delete node;
}

void clearTree(RBTree &tree) {
    clearRec(tree.root);
    tree.root = nullptr;
}

void readFromFile(RBTree &tree, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) insert(tree, val);
    in.close();
}

static void writeRec(RBNode* node, std::ofstream &out) {
    if (!node) return;
    writeRec(node->left, out);
    out << node->value << " ";
    writeRec(node->right, out);
}

void writeToFile(RBTree &tree, const std::string &filename) {
    std::ofstream out(filename);
    writeRec(tree.root, out);
    out.close();
}

static void leftRotate(RBTree &tree, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) tree.root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rightRotate(RBTree &tree, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) tree.root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

static void insertFixup(RBTree &tree, RBNode* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { z = z->parent; leftRotate(tree, z); }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) { z = z->parent; rightRotate(tree, z); }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    if (tree.root) tree.root->color = BLACK;
}

static void transplant(RBTree &tree, RBNode* u, RBNode* v) {
    if (!u->parent) tree.root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

static void deleteFixup(RBTree &tree, RBNode* x) {
    while (x != tree.root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree.root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree.root;
            }
        }
    }
    x->color = BLACK;
}
