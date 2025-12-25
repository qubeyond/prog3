#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>

namespace storage {

class Tuple {
   public:
    Tuple(const char* data, uint16_t length) : length_(length), is_view_(false) {
        if (length_ > 0) {
            data_ = std::make_unique<char[]>(length_);
            std::memcpy(data_.get(), data, length_);
        }
    }

    Tuple() : data_(nullptr), raw_data_(nullptr), length_(0), is_view_(false) {
    }

    ~Tuple() = default;

    Tuple(const Tuple& other) : length_(other.length_), is_view_(false) {
        if (length_ > 0) {
            data_ = std::make_unique<char[]>(length_);
            std::memcpy(data_.get(), other.getData(), length_);
        }
    }

    Tuple& operator=(const Tuple& other) {
        if (this != &other) {
            length_ = other.length_;
            is_view_ = false;

            if (length_ > 0) {
                std::unique_ptr<char[]> temp_data = std::make_unique<char[]>(length_);
                std::memcpy(temp_data.get(), other.getData(), length_);

                data_ = std::move(temp_data);
                raw_data_ = nullptr;
            } else {
                data_.reset();
                raw_data_ = nullptr;
            }
        }
        return *this;
    }

    Tuple(const char* data, uint16_t length, bool view_only) : length_(length), is_view_(true) {
        if (!view_only) {
            throw std::runtime_error("View-only constructor must be used for pages.");
        }
        raw_data_ = const_cast<char*>(data);
        data_.reset();
    }

    const char* getData() const {
        if (length_ == 0)
            return nullptr;
        return is_view_ ? raw_data_ : data_.get();
    }

    uint16_t getLength() const {
        return length_;
    }

    bool isView() const {
        return is_view_;
    }

   private:
    std::unique_ptr<char[]> data_ = nullptr;
    char* raw_data_ = nullptr;
    uint16_t length_ = 0;
    bool is_view_ = false;
};

}  // namespace storage