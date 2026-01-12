#pragma once
#include <string>
#include <vector>
#include <optional>

class HashTableOpen {
public:
    struct Entry {
        std::string key;
        int value;
        bool occupied = false;
        bool deleted = false;
    };

    explicit HashTableOpen(size_t cap = 100003);
    void insert(const std::string& key, int value);
    std::optional<int> search(const std::string& key) const;
    void remove(const std::string& key);
    size_t size() const { return size_; }

private:
    size_t calculate_hash(const std::string& key) const;
    size_t capacity_;
    std::vector<Entry> table_;
    size_t size_ = 0;
};