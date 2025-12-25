#pragma once

#include <cstdint>
#include <vector>

#include "db/storage/page.hpp"
#include "db/storage/tuple.hpp"

namespace storage {

struct SlotEntry {
    uint16_t offset;
    uint16_t length;
};

struct SlotData {
    uint16_t slot_id;
    SlotEntry slot;
};

class SlottedPage {
   public:
    static constexpr size_t NEXT_PAGE_ID_OFFSET = 0;
    static constexpr size_t HEADER_START_OFFSET = sizeof(page_id_t);

    explicit SlottedPage(Page* page);

    bool InsertTuple(const Tuple& tuple, uint16_t* slotId);
    bool DeleteTuple(uint16_t slotId);
    bool UpdateTuple(uint16_t slotId, const Tuple& newTuple);
    Tuple GetTuple(uint16_t slotId) const;

    uint16_t getFreeSpace() const;
    uint16_t getNumSlots() const;
    page_id_t getPageId() const;
    bool isEmpty() const;

    page_id_t getNextPageId() const;
    void setNextPageId(page_id_t next_page_id);

    SlotEntry* getSlot(uint16_t slotId);
    const SlotEntry* getSlot(uint16_t slotId) const;

   private:
    Page* page_;

    uint16_t getHeaderSize() const;

    uint16_t* getFreeSpacePtr();
    const uint16_t* getFreeSpacePtr() const;

    uint16_t* getNumSlotsPtr();
    const uint16_t* getNumSlotsPtr() const;

    void compactify();
};

}  // namespace storage