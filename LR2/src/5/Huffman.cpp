#include "5/Huffman.hpp"
#include <iostream>
#include <algorithm>

void Huffman::MinHeap::push(std::shared_ptr<Huffman::Node> node) {
    data.push_back(node);
    int i = static_cast<int>(data.size()) - 1;
    while (i > 0 && data[i]->freq < data[(i - 1) / 2]->freq) {
        std::swap(data[i], data[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

std::shared_ptr<Huffman::Node> Huffman::MinHeap::pop() {
    if (data.size() == 0) return nullptr;
    
    auto res = data[0];
    data[0] = data[data.size() - 1];
    
    Array<std::shared_ptr<Huffman::Node>> newData(data.size() - 1);
    for (size_t k = 0; k < data.size() - 1; ++k) {
        newData.push_back(data[k]);
    }
    data = std::move(newData);

    if (data.size() > 0) {
        size_t i = 0;
        while (2 * i + 1 < data.size()) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            size_t smallest = left;
            if (right < data.size() && data[right]->freq < data[left]->freq) {
                smallest = right;
            }
            if (data[i]->freq <= data[smallest]->freq) break;
            std::swap(data[i], data[smallest]);
            i = smallest;
        }
    }
    return res;
}

void Huffman::buildCodes(std::shared_ptr<Huffman::Node> root, const std::string& code, HashTableOpen& table) {
    if (!root) return;
    if (!root->left && !root->right) {
        std::cout << root->ch << ": " << code << std::endl;
        return;
    }
    buildCodes(root->left, code + "0", table);
    buildCodes(root->right, code + "1", table);
}

void Huffman::compress(const std::string& text) {
    if (text.empty()) return;

    int freq[256] = {0};
    for (char c : text) {
        freq[static_cast<unsigned char>(c)]++;
    }

    MinHeap heap;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            heap.push(std::make_shared<Node>(static_cast<char>(i), freq[i]));
        }
    }

    if (heap.size() == 1) {
        auto root = heap.pop();
        std::cout << root->ch << ": 0" << std::endl;
        return;
    }

    while (heap.size() > 1) {
        auto left = heap.pop();
        auto right = heap.pop();
        auto combined = std::make_shared<Node>(left->freq + right->freq, left, right);
        heap.push(combined);
    }

    HashTableOpen dummy(100);
    buildCodes(heap.pop(), "", dummy);
}