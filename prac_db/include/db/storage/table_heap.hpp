#pragma once

#include <mutex>

#include "db/buffer/buffer_pool_manager.hpp"
#include "db/storage/slotted_page.hpp"
#include "db/storage/tuple.hpp"

namespace storage {

struct RID {
    page_id_t page_id = Page::INVALID_PAGE_ID;
    uint16_t slot_id = 0;

    bool operator==(const RID& other) const {
        return page_id == other.page_id && slot_id == other.slot_id;
    }
};

class TableIterator;

class TableHeap {
    friend class TableIterator;

   public:
    explicit TableHeap(BufferPoolManager* bpm);
    TableHeap(BufferPoolManager* bpm, page_id_t first_page_id);

    ~TableHeap() = default;

    bool insertTuple(const Tuple& tuple, RID* rid);
    bool markDelete(const RID& rid);
    bool updateTuple(const RID& rid, const Tuple& tuple);
    Tuple getTuple(const RID& rid);

    TableIterator begin();
    TableIterator end();

    page_id_t getFirstPageId() const {
        return first_page_id_;
    }

   private:
    void initNewPage(Page* page);

    BufferPoolManager* bpm_;
    page_id_t first_page_id_;
    page_id_t last_page_id_;
    std::mutex latch_;
};

}  // namespace storage