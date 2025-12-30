#include "hashtable.hpp"

#include <functional>
#include <sstream>

HashTable::HashTable(size_t initialCapacity) : m_capacity(initialCapacity) {
    m_buckets.resize(m_capacity, nullptr);
}

void HashTable::clear() noexcept {
    for (auto node : m_buckets) {
        while (node) {
            HashNode* next = node->next;
            delete node;
            node = next;
        }
    }
    m_buckets.clear();
    m_count = 0;
}

HashTable::~HashTable() {
    clear();
}

HashTable::HashTable(HashTable&& other) noexcept
    : m_buckets(std::move(other.m_buckets)), m_capacity(other.m_capacity), m_count(other.m_count) {
    other.m_capacity = 0;
    other.m_count = 0;
}

HashTable& HashTable::operator=(HashTable&& other) noexcept {
    if (this != &other) {
        clear();
        m_buckets = std::move(other.m_buckets);
        m_capacity = other.m_capacity;
        m_count = other.m_count;
        other.m_capacity = 0;
        other.m_count = 0;
    }
    return *this;
}

size_t HashTable::hashFunction(std::string_view key) const {
    return std::hash<std::string_view>{}(key) % m_capacity;
}

void HashTable::resize() {
    size_t oldCapacity = m_capacity;
    auto oldBuckets = std::move(m_buckets);

    m_capacity = oldCapacity * 2;
    m_buckets.assign(m_capacity, nullptr);
    m_count = 0;

    for (auto node : oldBuckets) {
        while (node) {
            insert(node->key, node->value);
            HashNode* next = node->next;
            delete node;
            node = next;
        }
    }
}

void HashTable::insert(std::string_view key, std::string_view value) {
    size_t index = hashFunction(key);
    HashNode* node = m_buckets[index];

    while (node) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }

    auto* newNode = new HashNode(key, value);
    newNode->next = m_buckets[index];
    m_buckets[index] = newNode;
    m_count++;

    if (static_cast<double>(m_count) / m_capacity > LOAD_FACTOR_THRESHOLD) {
        resize();
    }
}

bool HashTable::remove(std::string_view key) {
    size_t index = hashFunction(key);
    HashNode* node = m_buckets[index];
    HashNode* prev = nullptr;

    while (node) {
        if (node->key == key) {
            if (prev)
                prev->next = node->next;
            else
                m_buckets[index] = node->next;

            delete node;
            m_count--;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}

std::string HashTable::get(std::string_view key) const {
    size_t index = hashFunction(key);
    HashNode* node = m_buckets[index];

    while (node) {
        if (node->key == key) return node->value;
        node = node->next;
    }
    return "";
}

bool HashTable::contains(std::string_view key) const {
    size_t index = hashFunction(key);
    HashNode* node = m_buckets[index];

    while (node) {
        if (node->key == key) return true;
        node = node->next;
    }
    return false;
}

std::string HashTable::print() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto* node : m_buckets) {
        while (node) {
            if (!first) oss << ", ";
            oss << node->key << ": " << node->value;
            first = false;
            node = node->next;
        }
    }
    oss << "}";
    return oss.str();
}

