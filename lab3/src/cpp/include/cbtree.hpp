#pragma once

#include <queue>
#include <string>
#include <vector>

struct CBTNode {
    std::string data;
    CBTNode* left;
    CBTNode* right;

    CBTNode(const std::string& val) : data(val), left(nullptr), right(nullptr) {
    }
};

class CompleteBinaryTree {
   public:
    CompleteBinaryTree() : root(nullptr), nodeCount(0) {
    }
    ~CompleteBinaryTree() {
        clear();
    }

    void insert(const std::string& value);
    bool search(const std::string& value) const;
    void clear();
    std::string print() const;
    std::vector<std::string> toVector() const;

   private:
    CBTNode* root;
    size_t nodeCount;

    void clearRecursive(CBTNode* node);
    void printHelper(CBTNode* node, std::string prefix, bool isLeft, std::string& result) const;
};