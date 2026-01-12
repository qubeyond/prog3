#include "6/HashTableOpen.hpp"

HashTableOpen::HashTableOpen(size_t cap) : capacity_(cap), table_(cap) {}

size_t HashTableOpen::calculate_hash(const std::string& key) const {
    size_t hash = 0;
    for (char c : key) hash = hash * 31 + static_cast<unsigned char>(c);
    return hash % capacity_;
}

void HashTableOpen::insert(const std::string& key, int value) {
    size_t idx = calculate_hash(key);
    for (size_t i = 0; i < capacity_; ++i) {
        size_t curr = (idx + i) % capacity_;
        if (!table_[curr].occupied) {
            table_[curr] = {key, value, true, false};
            size_++; 
            return;
        }
        if (table_[curr].key == key && !table_[curr].deleted) {
            table_[curr].value = value;
            return;
        }
    }
}

std::optional<int> HashTableOpen::search(const std::string& key) const {
    size_t idx = calculate_hash(key);
    for (size_t i = 0; i < capacity_; ++i) {
        size_t curr = (idx + i) % capacity_;
        if (!table_[curr].occupied && !table_[curr].deleted) break;
        if (table_[curr].occupied && table_[curr].key == key) return table_[curr].value;
    }
    return std::nullopt;
}

void HashTableOpen::remove(const std::string& key) {
    size_t idx = calculate_hash(key);
    for (size_t i = 0; i < capacity_; ++i) {
        size_t curr = (idx + i) % capacity_;
        if (!table_[curr].occupied && !table_[curr].deleted) break;
        if (table_[curr].occupied && table_[curr].key == key) {
            table_[curr].occupied = false;
            table_[curr].deleted = true;
            size_--; 
            return;
        }
    }
}