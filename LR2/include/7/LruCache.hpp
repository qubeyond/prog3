#pragma once
#include <string>
#include <optional>
#include "6/HashTable.hpp"

class LruCache {
    struct Node {
        std::string key;
        std::string value;
        Node* prev = nullptr;
        Node* next = nullptr;
    };

public:
    explicit LruCache(size_t capacity);
    ~LruCache();

    LruCache(const LruCache&) = delete;
    LruCache& operator=(const LruCache&) = delete;

    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);

private:
    void moveToHead(Node* node);
    void removeNode(Node* node);
    void addToHead(Node* node);
    void removeTail();

    size_t capacity_;
    size_t size_ = 0;
    Node* head_ = nullptr;
    Node* tail_ = nullptr;

    struct NodePtrEntry {
        std::string key;
        Node* ptr = nullptr;
        bool occupied = false;
        bool deleted = false;
    };

    struct PointerTable {
        std::unique_ptr<NodePtrEntry[]> data;
        size_t cap;
        PointerTable(size_t c) : data(std::make_unique<NodePtrEntry[]>(c)), cap(c) {}
    } index_;

    size_t get_hash(const std::string& key) const;
    NodePtrEntry* find_entry(const std::string& key);
};