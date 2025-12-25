#include "db/storage/table_iterator.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

#include "common/config.hpp"
#include "db/storage/page.hpp"
#include "db/storage/slotted_page.hpp"

namespace storage {

TableIterator::TableIterator(TableHeap* table_heap, RID rid) : table_heap_(table_heap), rid_(rid) {
    if (rid_.page_id == Page::INVALID_PAGE_ID) {
        return;
    }

    if (rid_.slot_id == 0) {
        rid_.slot_id = std::numeric_limits<uint16_t>::max();
    } else {
        rid_.slot_id--;
    }

    advance();
}

TableIterator::~TableIterator(){};

TableIterator& TableIterator::operator++() {
    advance();
    return *this;
}

void TableIterator::advance() {
    if (rid_.page_id == Page::INVALID_PAGE_ID) {
        return;
    }

    page_id_t cur_page_id = rid_.page_id;
    uint16_t start_slot_id = rid_.slot_id + 1;

    for (;;) {
        Page* page = table_heap_->bpm_->fetchPage(cur_page_id);
        if (page == nullptr) {
            std::cout << "DEBUG: FAILED to fetch page " << cur_page_id << std::endl;
            rid_.page_id = Page::INVALID_PAGE_ID;
            return;
        }

        SlottedPage slotted(page);

        std::cout << "DEBUG: Scanning Page ID: " << cur_page_id << ", Num Slots: " << slotted.getNumSlots()
                  << ", Start Slot: " << start_slot_id << std::endl;

        for (uint16_t i = start_slot_id; i < slotted.getNumSlots(); ++i) {
            const SlotEntry* slot = slotted.getSlot(i);

            if (slot != nullptr && slot->length > 0) {
                rid_.page_id = cur_page_id;
                rid_.slot_id = i;

                current_tuple_ = slotted.GetTuple(i);

                table_heap_->bpm_->unpinPage(cur_page_id, false);
                return;
            }
        }

        page_id_t next_page_id = slotted.getNextPageId();

        std::cout << "DEBUG: Finished scanning Page ID: " << cur_page_id << ". Next Page ID: " << next_page_id
                  << std::endl;

        table_heap_->bpm_->unpinPage(cur_page_id, false);

        if (next_page_id == Page::INVALID_PAGE_ID) {
            rid_.page_id = Page::INVALID_PAGE_ID;
            return;
        }

        cur_page_id = next_page_id;
        start_slot_id = 0;
    }
}

const Tuple& TableIterator::operator*() const {
    return current_tuple_;
}

const Tuple* TableIterator::operator->() const {
    return &current_tuple_;
}

bool TableIterator::operator==(const TableIterator& other) const {
    return rid_.page_id == other.rid_.page_id && rid_.slot_id == other.rid_.slot_id;
}

bool TableIterator::operator!=(const TableIterator& other) const {
    return !(*this == other);
}

}  // namespace storage