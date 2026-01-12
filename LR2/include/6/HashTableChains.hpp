#pragma once
#include <string>
#include <vector>
#include <optional>
#include <functional>

class HashTableChains {
public:
    struct Entry {
        std::string key;
        int value;
    };

    struct Node {
        Entry data;
        Node* next = nullptr;
    };

    class MyList {
    public:
        MyList() : head(nullptr) {}
        ~MyList();

        void push_back(const Entry& entry);
        void remove_if(std::function<bool(const Entry&)> predicate);
    
        struct Iterator {
            Node* current;
            bool operator!=(const Iterator& other) const { return current != other.current; }
            void operator++() { current = current->next; }
            Entry& operator*() { return current->data; }
        };
        Iterator begin() const { return {head}; }
        Iterator end() const { return {nullptr}; }

    private:
        Node* head;
    };

    explicit HashTableChains(size_t cap = 100003);
    
    void insert(const std::string& key, int value);
    std::optional<int> search(const std::string& key) const;
    void remove(const std::string& key);
    size_t size() const { return size_; }

private:
    size_t calculate_hash(const std::string& key) const;
    size_t capacity_;
    size_t size_ = 0;
    std::vector<MyList> table_; 
};