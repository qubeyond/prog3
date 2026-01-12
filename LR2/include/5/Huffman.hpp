#pragma once
#include <string>
#include <memory>
#include "6/HashTableOpen.hpp"
#include "4/Array.hpp"

class Huffman {
public:
    struct Node {
        char ch;
        int freq;
        std::shared_ptr<Node> left, right;

        Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
        Node(int f, std::shared_ptr<Node> l, std::shared_ptr<Node> r) 
            : ch('\0'), freq(f), left(l), right(r) {}
    };

    static void compress(const std::string& text);

private:
    struct MinHeap {
        Array<std::shared_ptr<Node>> data;
        void push(std::shared_ptr<Node> node);
        std::shared_ptr<Node> pop();
        size_t size() const { return data.size(); }
    };

    static void buildCodes(std::shared_ptr<Node> root, const std::string& code, HashTableOpen& table);
};