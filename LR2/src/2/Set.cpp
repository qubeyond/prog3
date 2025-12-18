#include "2/Set.hpp"

Set::Set(size_t cap) : table_(cap) {}

void Set::add(const std::string& key) {
    table_.insert(key, 1);
}

void Set::del(const std::string& key) {
    table_.remove(key);
}

bool Set::contains(const std::string& key) const {
    return table_.search(key).has_value();
}

size_t Set::size() const noexcept {
    return table_.size();
}