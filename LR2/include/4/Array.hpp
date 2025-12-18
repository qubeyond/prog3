#pragma once
#include <memory>
#include <string>

template <typename T>
class Array {
public:
    Array() : data_(nullptr), size_(0), capacity_(0) {}
    explicit Array(size_t initial_cap) : size_(0), capacity_(initial_cap) {
        data_ = std::make_unique<T[]>(capacity_);
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 8 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void reserve(size_t new_cap) {
        if (new_cap <= capacity_) return;
        auto new_data = std::make_unique<T[]>(new_cap);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_cap;
    }

    T& operator[](size_t idx) { return data_[idx]; }
    const T& operator[](size_t idx) const { return data_[idx]; }
    size_t size() const { return size_; }

private:
    std::unique_ptr<T[]> data_;
    size_t size_;
    size_t capacity_;
};