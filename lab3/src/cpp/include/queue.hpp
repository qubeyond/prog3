#pragma once

#include <string>
#include <string_view>

#include "singly_list.hpp"

class Queue {
   public:
    Queue() = default;
    ~Queue() = default;

    Queue(const Queue& other) = default;
    Queue& operator=(const Queue& other) = default;
    Queue(Queue&& other) noexcept = default;
    Queue& operator=(Queue&& other) noexcept = default;

    void push(std::string_view value);
    std::string pop();

    [[nodiscard]] std::string front() const;
    [[nodiscard]] std::string print() const;
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

   private:
    SinglyList m_list;
};