#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <memory>

namespace storage {

using page_id_t = int;
using frame_id_t = int;

class Page {
   public:
    inline static constexpr page_id_t INVALID_PAGE_ID = -1;

    explicit Page(size_t page_size) : page_size_(page_size), data_(std::make_unique<char[]>(page_size)) {
        resetData();
    }

    ~Page() = default;

    Page(const Page&) = delete;
    Page& operator=(const Page&) = delete;

    Page(Page&&) noexcept = default;
    Page& operator=(Page&&) noexcept = default;

    char* getData() {
        return data_.get();
    }

    const char* getData() const {
        return data_.get();
    }

    void resetData() {
        if (data_) {
            std::fill_n(data_.get(), page_size_, 0);
        }
    }

    page_id_t getPageId() const {
        return page_id_;
    }
    void setPageId(page_id_t page_id) {
        page_id_ = page_id;
    }

    int getPinCount() const {
        return pin_count_;
    }
    void setPinCount(int pin_count) {
        pin_count_ = pin_count;
    }

    void incPinCount() {
        pin_count_++;
    }
    void decPinCount() {
        pin_count_--;
    }

    bool isDirty() const {
        return is_dirty_;
    }
    void setDirty(bool is_dirty) {
        is_dirty_ = is_dirty;
    }

    frame_id_t getFrameId() const {
        return frame_id_;
    }
    void setFrameId(frame_id_t frame_id) {
        frame_id_ = frame_id;
    }

    size_t getSize() const {
        return page_size_;
    }

   private:
    page_id_t page_id_ = INVALID_PAGE_ID;
    frame_id_t frame_id_ = -1;
    int pin_count_ = 0;
    bool is_dirty_ = false;
    size_t page_size_;
    std::unique_ptr<char[]> data_;
};

}  // namespace storage