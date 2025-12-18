#include "7/LruCache.hpp"

LruCache::LruCache(size_t capacity) 
    : capacity_(capacity), index_(capacity * 2 + 1) {}

LruCache::~LruCache() {
    Node* current = head_;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

size_t LruCache::get_hash(const std::string& key) const {
    size_t h = 0;
    for (char c : key) h = h * 31 + static_cast<unsigned char>(c);
    return h % index_.cap;
}

LruCache::NodePtrEntry* LruCache::find_entry(const std::string& key) {
    size_t idx = get_hash(key);
    size_t start = idx;
    while (index_.data[idx].occupied || index_.data[idx].deleted) {
        if (index_.data[idx].occupied && index_.data[idx].key == key) return &index_.data[idx];
        idx = (idx + 1) % index_.cap;
        if (idx == start) break;
    }
    return &index_.data[idx];
}

std::optional<std::string> LruCache::get(const std::string& key) {
    auto* entry = find_entry(key);
    if (!entry->occupied) return std::nullopt;
    moveToHead(entry->ptr);
    return entry->ptr->value;
}

void LruCache::put(const std::string& key, const std::string& value) {
    auto* entry = find_entry(key);
    if (entry->occupied) {
        entry->ptr->value = value;
        moveToHead(entry->ptr);
        return;
    }

    if (size_ == capacity_) {
        std::string tail_key = tail_->key;
        auto* to_del = find_entry(tail_key);
        to_del->occupied = false;
        to_del->deleted = true;
        removeTail();
        size_--;
    }

    Node* newNode = new Node{key, value};
    addToHead(newNode);
    entry->key = key;
    entry->ptr = newNode;
    entry->occupied = true;
    entry->deleted = false;
    size_++;
}

void LruCache::removeNode(Node* node) {
    if (node->prev) node->prev->next = node->next;
    else head_ = node->next;
    if (node->next) node->next->prev = node->prev;
    else tail_ = node->prev;
}

void LruCache::addToHead(Node* node) {
    node->next = head_;
    node->prev = nullptr;
    if (head_) head_->prev = node;
    head_ = node;
    if (!tail_) tail_ = node;
}

void LruCache::moveToHead(Node* node) {
    removeNode(node);
    addToHead(node);
}

void LruCache::removeTail() {
    if (!tail_) return;
    Node* temp = tail_;
    if (tail_->prev) {
        tail_ = tail_->prev;
        tail_->next = nullptr;
    } else {
        head_ = tail_ = nullptr;
    }
    delete temp;
}