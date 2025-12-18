#pragma once
#include "6/HashTable.hpp"
#include <string>

class Set {
public:
    explicit Set(size_t cap = 100003);

    void add(const std::string& key);
    void del(const std::string& key);
    bool contains(const std::string& key) const;
    size_t size() const noexcept;

private:
    HashTable table_;
};