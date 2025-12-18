#include "6/HashTable.hpp"

HashTable::HashTable(size_t cap) : capacity_(cap), table_(std::make_unique<Entry[]>(cap)) {}

size_t HashTable::calculate_hash(const std::string& key) const {
    size_t hash = 0;
    for (size_t i = 0; i < key.length(); i += 3) {
        std::string part = key.substr(i, 3);
        size_t part_val = 0;
        for (char c : part) part_val = part_val * 31 + static_cast<unsigned char>(c);
        hash += part_val;
    }
    return hash % capacity_;
}

void HashTable::insert(const std::string& key, int value) {
    size_t idx = calculate_hash(key);
    size_t start_idx = idx;

    while (table_[idx].occupied) {
        if (table_[idx].key == key) {
            table_[idx].value = value;
            return;
        }
        idx = (idx + 1) % capacity_;
        if (idx == start_idx) return;
    }

    table_[idx].key = key;
    table_[idx].value = value;
    table_[idx].occupied = true;
    table_[idx].deleted = false;
    size_++;
}

std::optional<int> HashTable::search(const std::string& key) const {
    size_t idx = calculate_hash(key);
    size_t start_idx = idx;

    while (table_[idx].occupied || table_[idx].deleted) {
        if (table_[idx].occupied && table_[idx].key == key) {
            return table_[idx].value;
        }
        idx = (idx + 1) % capacity_;
        if (idx == start_idx) break;
    }
    return std::nullopt;
}

void HashTable::remove(const std::string& key) {
    size_t idx = calculate_hash(key);
    size_t start_idx = idx;

    while (table_[idx].occupied || table_[idx].deleted) {
        if (table_[idx].occupied && table_[idx].key == key) {
            table_[idx].occupied = false;
            table_[idx].deleted = true;
            size_--;
            return;
        }
        idx = (idx + 1) % capacity_;
        if (idx == start_idx) break;
    }
}