#include "6/HashTableChains.hpp"

HashTableChains::MyList::~MyList() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void HashTableChains::MyList::push_back(const Entry& entry) {
    Node* newNode = new Node{entry, head}; 
    head = newNode;
}

void HashTableChains::MyList::remove_if(std::function<bool(const Entry&)> predicate) {
    Node* current = head;
    Node* prev = nullptr;
    while (current) {
        if (predicate(current->data)) {
            Node* toDelete = current;
            if (prev) prev->next = current->next;
            else head = current->next;
            current = current->next;
            delete toDelete;
            return;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

HashTableChains::HashTableChains(size_t cap) : capacity_(cap), table_(cap) {}

size_t HashTableChains::calculate_hash(const std::string& key) const {
    size_t hash = 0;
    size_t i = 0;
 
    while (i < key.length()) {
        size_t part = 0;
        for (size_t j = 0; j < 3 && i < key.length(); ++j) {
            part = part * 100 + static_cast<unsigned char>(key[i++]);
        }
        hash += part; 
    }

    return hash % capacity_;
}

void HashTableChains::insert(const std::string& key, int value) {
    auto& bucket = table_[calculate_hash(key)];
    for (auto& entry : bucket) {
        if (entry.key == key) {
            entry.value = value;
            return;
        }
    }
    bucket.push_back({key, value});
    size_++; 
}

std::optional<int> HashTableChains::search(const std::string& key) const {
    const auto& bucket = table_[calculate_hash(key)];
    for (const auto& entry : bucket) {
        if (entry.key == key) return entry.value;
    }
    return std::nullopt;
}

void HashTableChains::remove(const std::string& key) {
    auto& bucket = table_[calculate_hash(key)];
    bucket.remove_if([&key](const Entry& e) { return e.key == key; });
    size_--; // Добавил декремент размера
}