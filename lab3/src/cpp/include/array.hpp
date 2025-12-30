#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

class Array {
   private:
    std::unique_ptr<std::string[]> data;
    size_t m_size{0};
    size_t m_capacity{0};
    void resize(size_t new_capacity);

   public:
    explicit Array(size_t initial_capacity = 10);
    ~Array() = default;
    Array(const Array& other);
    Array& operator=(const Array& other);
    Array(Array&& other) noexcept = default;
    Array& operator=(Array&& other) noexcept = default;

    void push(std::string_view value);
    void insert(size_t index, std::string_view value);
    const std::string& get(size_t index) const;
    void set(size_t index, std::string_view value);
    bool remove(size_t index);
    [[nodiscard]] size_t getSize() const noexcept {
        return m_size;
    }
    [[nodiscard]] size_t getCapacity() const noexcept {
        return m_capacity;
    }
    std::string print() const;
};