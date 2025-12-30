#pragma once

#include <string>
#include <string_view>

#include "singly_list.hpp"

class Stack {
   public:
    Stack() = default;
    ~Stack() = default;

    Stack(const Stack& other) = default;
    Stack& operator=(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(Stack&& other) noexcept = default;

    void push(std::string_view value);
    std::string pop();

    [[nodiscard]] std::string top() const;
    [[nodiscard]] std::string print() const;
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

   private:
    SinglyList m_list;
};