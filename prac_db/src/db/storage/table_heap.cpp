#include "db/storage/table_heap.hpp"

#include <iostream>

#include "db/storage/table_iterator.hpp"

namespace storage {

void TableHeap::initNewPage(Page* page) {
    SlottedPage slotted_page(page);
    slotted_page.setNextPageId(Page::INVALID_PAGE_ID);

    uint16_t* free_space_ptr = reinterpret_cast<uint16_t*>(page->getData() + SlottedPage::HEADER_START_OFFSET);
    uint16_t* num_slots_ptr =
        reinterpret_cast<uint16_t*>(page->getData() + SlottedPage::HEADER_START_OFFSET + sizeof(uint16_t));
    *free_space_ptr = page->getSize();
    *num_slots_ptr = 0;
}

TableHeap::TableHeap(BufferPoolManager* bpm) : bpm_(bpm) {
    page_id_t page_id;
    Page* page = bpm_->newPage(&page_id);
    if (page == nullptr) {
        throw std::runtime_error("Cannot allocate first page for TableHeap");
    }

    initNewPage(page);

    first_page_id_ = page_id;
    last_page_id_ = page_id;
    bpm_->unpinPage(page_id, true);
}

TableHeap::TableHeap(BufferPoolManager* bpm, page_id_t first_page_id)
    : bpm_(bpm), first_page_id_(first_page_id), last_page_id_(first_page_id) {
}

bool TableHeap::insertTuple(const Tuple& tuple, RID* rid) {
    std::lock_guard<std::mutex> lock(latch_);

    page_id_t cur_page_id = last_page_id_;

    while (true) {
        Page* page = bpm_->fetchPage(cur_page_id);
        if (page == nullptr) {
            if (cur_page_id != first_page_id_) {
                cur_page_id = first_page_id_;
                continue;
            }
            return false;
        }

        SlottedPage slotted(page);
        uint16_t slot_id;

        if (slotted.InsertTuple(tuple, &slot_id)) {
            rid->page_id = cur_page_id;
            rid->slot_id = slot_id;
            last_page_id_ = cur_page_id;

            bpm_->unpinPage(cur_page_id, true);
            return true;
        }

        page_id_t next_page_id = slotted.getNextPageId();

        if (next_page_id == Page::INVALID_PAGE_ID) {
            page_id_t new_page_id;
            Page* new_page = bpm_->newPage(&new_page_id);

            if (!new_page) {
                bpm_->unpinPage(cur_page_id, false);
                return false;
            }

            slotted.setNextPageId(new_page_id);

            initNewPage(new_page);
            SlottedPage new_slotted(new_page);

            if (!new_slotted.InsertTuple(tuple, &slot_id)) {
                bpm_->unpinPage(cur_page_id, true);
                bpm_->unpinPage(new_page_id, false);
                return false;
            }

            rid->page_id = new_page_id;
            rid->slot_id = slot_id;

            bpm_->unpinPage(cur_page_id, true);
            bpm_->unpinPage(new_page_id, true);

            last_page_id_ = new_page_id;

            return true;
        }

        bpm_->unpinPage(cur_page_id, false);
        cur_page_id = next_page_id;
    }
}

Tuple TableHeap::getTuple(const RID& rid) {
    Page* page = bpm_->fetchPage(rid.page_id);
    if (!page)
        return Tuple();

    SlottedPage slotted(page);
    Tuple t = slotted.GetTuple(rid.slot_id);

    Tuple ret(t.getData(), t.getLength());

    bpm_->unpinPage(rid.page_id, false);
    return ret;
}

bool TableHeap::markDelete(const RID& rid) {
    Page* page = bpm_->fetchPage(rid.page_id);
    if (!page)
        return false;

    SlottedPage slotted(page);
    bool success = slotted.DeleteTuple(rid.slot_id);

    bpm_->unpinPage(rid.page_id, success);
    return success;
}

bool TableHeap::updateTuple(const RID& rid, const Tuple& tuple) {
    Page* page = bpm_->fetchPage(rid.page_id);
    if (!page)
        return false;

    SlottedPage slotted(page);
    bool success = slotted.UpdateTuple(rid.slot_id, tuple);

    bpm_->unpinPage(rid.page_id, success);
    return success;
}

TableIterator TableHeap::begin() {
    return TableIterator(this, RID{first_page_id_, 0});
}

TableIterator TableHeap::end() {
    return TableIterator(this, RID{Page::INVALID_PAGE_ID, 0});
}

}  // namespace storage