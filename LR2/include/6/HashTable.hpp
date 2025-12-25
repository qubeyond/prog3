#pragma once
#include <string>
#include <memory>
#include <optional>

class HashTable {
public:
    struct Entry {
        std::string key;
        int value;
        bool occupied = false;
        bool deleted = false;
    };

    explicit HashTable(size_t cap = 100003);
    
    ~HashTable() = default;
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) noexcept = default;
    HashTable& operator=(HashTable&&) noexcept = default;

    void insert(const std::string& key, int value);
    std::optional<int> search(const std::string& key) const;
    void remove(const std::string& key);
    
    size_t size() const noexcept { return size_; }

private:
    size_t calculate_hash(const std::string& key) const;
    
    size_t capacity_;
    size_t size_ = 0;
    std::unique_ptr<Entry[]> table_;
};