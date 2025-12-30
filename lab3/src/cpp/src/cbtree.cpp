#include "cbtree.hpp"

#include <sstream>

void CompleteBinaryTree::insert(const std::string& value) {
    CBTNode* newNode = new CBTNode(value);
    if (!root) {
        root = newNode;
        nodeCount = 1;
        return;
    }

    std::queue<CBTNode*> q;
    q.push(root);

    while (!q.empty()) {
        CBTNode* temp = q.front();
        q.pop();

        if (!temp->left) {
            temp->left = newNode;
            break;
        } else {
            q.push(temp->left);
        }

        if (!temp->right) {
            temp->right = newNode;
            break;
        } else {
            q.push(temp->right);
        }
    }
    nodeCount++;
}

bool CompleteBinaryTree::search(const std::string& value) const {
    if (!root) return false;
    std::queue<CBTNode*> q;
    q.push(root);
    while (!q.empty()) {
        CBTNode* temp = q.front();
        q.pop();
        if (temp->data == value) return true;
        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }
    return false;
}

void CompleteBinaryTree::clear() {
    clearRecursive(root);
    root = nullptr;
    nodeCount = 0;
}

void CompleteBinaryTree::clearRecursive(CBTNode* node) {
    if (node) {
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }
}

std::vector<std::string> CompleteBinaryTree::toVector() const {
    std::vector<std::string> res;
    if (!root) return res;
    std::queue<CBTNode*> q;
    q.push(root);
    while (!q.empty()) {
        CBTNode* temp = q.front();
        q.pop();
        res.push_back(temp->data);
        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }
    return res;
}

std::string CompleteBinaryTree::print() const {
    if (!root) return "Empty Tree\n";
    std::string result = "";
    printHelper(root, "", false, result);
    return result;
}

void CompleteBinaryTree::printHelper(CBTNode* node, std::string prefix, bool isLeft,
                                     std::string& result) const {
    if (!node) return;
    result += prefix + (isLeft ? "├── " : "└── ") + node->data + "\n";
    if (node->left || node->right) {
        std::string newPrefix = prefix + (isLeft ? "│   " : "    ");
        printHelper(node->left, newPrefix, true, result);
        printHelper(node->right, newPrefix, false, result);
    }
}