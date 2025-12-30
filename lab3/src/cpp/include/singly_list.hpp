#pragma once

#include <sstream>
#include <string>
#include <string_view>

class SinglyList {
   private:
    struct Node {
        std::string data;
        Node* next;
        explicit Node(std::string_view value) : data(value), next(nullptr) {
        }
    };

    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    size_t m_size{0};

    void clear() noexcept;

   public:
    SinglyList() = default;
    ~SinglyList();

    SinglyList(const SinglyList& other);
    SinglyList& operator=(const SinglyList& other);

    SinglyList(SinglyList&& other) noexcept;
    SinglyList& operator=(SinglyList&& other) noexcept;

    void push_head(std::string_view value);
    void push_tail(std::string_view value);

    bool del_head();
    bool del_tail();
    bool del_value(std::string_view value);

    [[nodiscard]] bool find(std::string_view value) const;
    [[nodiscard]] std::string get(size_t index) const;
    [[nodiscard]] std::string print() const;
    [[nodiscard]] size_t getSize() const noexcept {
        return m_size;
    }

    bool insert_before_value(std::string_view target, std::string_view value);
    bool insert_after_value(std::string_view target, std::string_view value);
    bool insert_before_index(size_t index, std::string_view value);
    bool insert_after_index(size_t index, std::string_view value);

    bool del_before_value(std::string_view target);
    bool del_after_value(std::string_view target);
    bool del_before_index(size_t index);
    bool del_after_index(size_t index);
};