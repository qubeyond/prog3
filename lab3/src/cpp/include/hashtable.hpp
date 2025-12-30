#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

class HashTable {
   private:
    struct HashNode {
        std::string key;
        std::string value;
        HashNode* next{nullptr};

        HashNode(std::string_view k, std::string_view v) : key(k), value(v) {
        }
    };

    std::vector<HashNode*> m_buckets;
    size_t m_capacity;
    size_t m_count{0};
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;

    [[nodiscard]] size_t hashFunction(std::string_view key) const;
    void resize();
    void clear() noexcept;

   public:
    explicit HashTable(size_t initialCapacity = 16);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(HashTable&& other) noexcept;

    void insert(std::string_view key, std::string_view value);
    bool remove(std::string_view key);

    [[nodiscard]] std::string get(std::string_view key) const;
    [[nodiscard]] bool contains(std::string_view key) const;
    [[nodiscard]] std::string print() const;

    [[nodiscard]] size_t size() const noexcept {
        return m_count;
    }
    [[nodiscard]] bool empty() const noexcept {
        return m_count == 0;
    }

    [[nodiscard]] std::vector<std::pair<std::string, std::string>> getAll() const {
        std::vector<std::pair<std::string, std::string>> all_data;
        for (const auto* node : m_buckets) {
            const HashNode* curr = node;
            while (curr) {
                all_data.push_back({curr->key, curr->value});
                curr = curr->next;
            }
        }
        return all_data;
    }
};