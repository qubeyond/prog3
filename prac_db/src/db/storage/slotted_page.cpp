#include "db/storage/slotted_page.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

namespace storage {

SlottedPage::SlottedPage(Page* page) : page_(page) {
    if (page_->getPageId() == Page::INVALID_PAGE_ID) {
        *getFreeSpacePtr() = page_->getSize();
        *getNumSlotsPtr() = 0;
        std::cout << "DEBUG: SlottedPage(" << page_->getPageId() << "): Initialized new page. Size=" << page_->getSize()
                  << ", FreePtr=" << page_->getSize() << ", NumSlots=0" << std::endl;
    } else {
        std::cout << "DEBUG: SlottedPage(" << page_->getPageId()
                  << "): Loaded existing page. FreePtr=" << *getFreeSpacePtr() << ", NumSlots=" << *getNumSlotsPtr()
                  << std::endl;
    }
}

uint16_t SlottedPage::getHeaderSize() const {
    return sizeof(uint16_t) * 2 + (*getNumSlotsPtr()) * sizeof(SlotEntry);
}

uint16_t* SlottedPage::getFreeSpacePtr() {
    return reinterpret_cast<uint16_t*>(page_->getData() + HEADER_START_OFFSET);
}

const uint16_t* SlottedPage::getFreeSpacePtr() const {
    return reinterpret_cast<const uint16_t*>(page_->getData() + HEADER_START_OFFSET);
}

uint16_t* SlottedPage::getNumSlotsPtr() {
    return reinterpret_cast<uint16_t*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t));
}

const uint16_t* SlottedPage::getNumSlotsPtr() const {
    return reinterpret_cast<const uint16_t*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t));
}

SlotEntry* SlottedPage::getSlot(uint16_t slotId) {
    if (slotId >= *getNumSlotsPtr()) {
        return nullptr;
    }
    return reinterpret_cast<SlotEntry*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t) * 2) + slotId;
}

const SlotEntry* SlottedPage::getSlot(uint16_t slotId) const {
    if (slotId >= *getNumSlotsPtr()) {
        return nullptr;
    }
    return reinterpret_cast<const SlotEntry*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t) * 2) + slotId;
}

uint16_t SlottedPage::getNumSlots() const {
    return *getNumSlotsPtr();
}

page_id_t SlottedPage::getPageId() const {
    return page_->getPageId();
}

uint16_t SlottedPage::getFreeSpace() const {
    size_t header_end = HEADER_START_OFFSET + getHeaderSize();
    if (*getFreeSpacePtr() < header_end) {
        return 0;
    }
    return *getFreeSpacePtr() - header_end;
}

bool SlottedPage::isEmpty() const {
    for (uint16_t i = 0; i < getNumSlots(); ++i) {
        if (getSlot(i)->length > 0) {
            return false;
        }
    }
    return true;
}

page_id_t SlottedPage::getNextPageId() const {
    return *reinterpret_cast<const page_id_t*>(page_->getData() + NEXT_PAGE_ID_OFFSET);
}

void SlottedPage::setNextPageId(page_id_t next_page_id) {
    *reinterpret_cast<page_id_t*>(page_->getData() + NEXT_PAGE_ID_OFFSET) = next_page_id;
    page_->setDirty(true);
}

bool SlottedPage::InsertTuple(const Tuple& tuple, uint16_t* slotId) {
    uint16_t tuple_size = tuple.getLength();
    uint16_t required_space = tuple_size + sizeof(SlotEntry);

    if (getFreeSpace() < required_space) {
        std::cout << "DEBUG: InsertTuple(" << getPageId() << "): FAILED. FreeSpace=" << getFreeSpace()
                  << ", Required=" << required_space << std::endl;
        return false;
    }

    uint16_t new_slot_id = *getNumSlotsPtr();

    SlotEntry* new_slot =
        reinterpret_cast<SlotEntry*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t) * 2) + new_slot_id;

    uint16_t new_tuple_offset = *getFreeSpacePtr() - tuple_size;

    new_slot->offset = new_tuple_offset;
    new_slot->length = tuple_size;

    std::memcpy(page_->getData() + new_tuple_offset, tuple.getData(), tuple_size);

    *getFreeSpacePtr() = new_tuple_offset;
    (*getNumSlotsPtr())++;
    page_->setDirty(true);

    *slotId = new_slot_id;
    std::cout << "DEBUG: InsertTuple(" << getPageId() << "): SUCCESS. SlotID=" << *slotId
              << ", Offset=" << new_tuple_offset << ", Length=" << tuple_size << ", NewNumSlots=" << *getNumSlotsPtr()
              << ", NewFreePtr=" << new_tuple_offset << std::endl;
    return true;
}

bool SlottedPage::DeleteTuple(uint16_t slotId) {
    std::cout << "DEBUG: DeleteTuple(" << getPageId() << "): Attempting to delete SlotID=" << slotId << std::endl;
    SlotEntry* slot = getSlot(slotId);
    if (slot == nullptr || slot->length == 0) {
        std::cout << "DEBUG: DeleteTuple(" << getPageId() << "): FAILED. Slot already empty or invalid." << std::endl;
        return false;
    }

    uint16_t old_offset = slot->offset;
    uint16_t old_length = slot->length;

    slot->offset = 0;
    slot->length = 0;
    page_->setDirty(true);

    std::cout << "DEBUG: DeleteTuple(" << getPageId() << "): SlotID=" << slotId
              << " marked for deletion. Offset=" << old_offset << ", Length=" << old_length << ". Calling compactify..."
              << std::endl;

    compactify();
    std::cout << "DEBUG: DeleteTuple(" << getPageId() << "): compactify finished." << std::endl;
    return true;
}

bool SlottedPage::UpdateTuple(uint16_t slotId, const Tuple& newTuple) {
    SlotEntry* slot = getSlot(slotId);
    if (slot == nullptr || slot->length == 0) {
        return false;
    }

    uint16_t old_size = slot->length;
    uint16_t new_size = newTuple.getLength();

    std::cout << "DEBUG: UpdateTuple(" << getPageId() << "): SlotID=" << slotId << ". OldSize=" << old_size
              << ", NewSize=" << new_size << std::endl;

    if (new_size <= old_size) {
        std::memcpy(page_->getData() + slot->offset, newTuple.getData(), new_size);
        slot->length = new_size;
        page_->setDirty(true);

        std::cout << "DEBUG: UpdateTuple(" << getPageId() << "): SlotID=" << slotId
                  << " updated in-place. NewLength=" << new_size << std::endl;
        return true;
    }

    uint16_t space_needed_for_new = new_size + sizeof(SlotEntry);
    uint16_t space_available = getFreeSpace() + old_size;

    if (space_available < space_needed_for_new) {
        std::cout << "DEBUG: UpdateTuple(" << getPageId() << "): FAILED. Not enough space for larger tuple."
                  << std::endl;
        return false;
    }

    uint16_t old_offset = slot->offset;
    slot->length = 0;
    slot->offset = 0;

    uint16_t new_tuple_offset = *getFreeSpacePtr() - new_size;
    std::memcpy(page_->getData() + new_tuple_offset, newTuple.getData(), new_size);

    *getFreeSpacePtr() = new_tuple_offset;

    slot->offset = new_tuple_offset;
    slot->length = new_size;

    std::cout << "DEBUG: UpdateTuple(" << getPageId() << "): SlotID=" << slotId
              << " moved. Calling compactify to reclaim old space." << std::endl;
    compactify();

    page_->setDirty(true);

    std::cout << "DEBUG: UpdateTuple(" << getPageId() << "): SlotID=" << slotId
              << " updated. NewOffset=" << slot->offset << ", NewLength=" << slot->length
              << ", NewFreePtr=" << *getFreeSpacePtr() << std::endl;
    return true;
}

Tuple SlottedPage::GetTuple(uint16_t slotId) const {
    const SlotEntry* slot = getSlot(slotId);
    if (slot == nullptr || slot->length == 0) {
        std::cout << "DEBUG: GetTuple(" << getPageId() << "): SlotID=" << slotId
                  << " is empty/invalid. Returning empty tuple." << std::endl;
        return Tuple();
    }
    std::cout << "DEBUG: GetTuple(" << getPageId() << "): SlotID=" << slotId << " found. Offset=" << slot->offset
              << ", Length=" << slot->length << std::endl;

    if (slot->length > 0) {
        const uint8_t* data_ptr_unsigned = reinterpret_cast<const uint8_t*>(page_->getData() + slot->offset);

        std::cout << "DEBUG: GetTuple(" << getPageId() << "): SlotID=" << slotId
                  << " First byte (Seed?): " << (int)data_ptr_unsigned[0] << std::endl;
    }

    return Tuple(page_->getData() + slot->offset, slot->length, true);
}

void SlottedPage::compactify() {
    uint16_t free_ptr = *getFreeSpacePtr();
    uint16_t page_end = page_->getSize();
    page_id_t page_id = getPageId();

    std::cout << "DEBUG: compactify(" << page_id << "): START. OldFreePtr=" << free_ptr
              << ", OldNumSlots=" << *getNumSlotsPtr() << std::endl;

    if (free_ptr == page_end) {
        std::cout << "DEBUG: compactify(" << page_id << "): Skip, page is fully compacted." << std::endl;
        return;
    }

    uint16_t old_num_slots = *getNumSlotsPtr();
    uint16_t current_offset = page_end;

    std::vector<SlotData> active_slots_data;

    for (uint16_t i = 0; i < old_num_slots; ++i) {
        SlotEntry* old_slot = getSlot(i);
        if (old_slot->length > 0) {
            active_slots_data.push_back({i, *old_slot});
        }
    }
    std::cout << "DEBUG: compactify(" << page_id << "): Found " << active_slots_data.size() << " active tuples."
              << std::endl;

    if (active_slots_data.empty()) {
        *getFreeSpacePtr() = page_end;
        *getNumSlotsPtr() = 0;
        page_->setDirty(true);
        std::cout << "DEBUG: compactify(" << page_id << "): All tuples deleted. NewFreePtr=" << page_end
                  << ", NewNumSlots=0." << std::endl;
        return;
    }

    std::sort(active_slots_data.begin(), active_slots_data.end(),
              [](const SlotData& a, const SlotData& b) { return a.slot.offset > b.slot.offset; });

    std::cout << "DEBUG: compactify(" << page_id << "): Sorted slots (by decreasing offset): ";
    for (const auto& data : active_slots_data) {
        std::cout << "S" << data.slot_id << "(Off:" << data.slot.offset << ", Len:" << data.slot.length << ") ";
    }
    std::cout << std::endl;

    std::vector<SlotEntry> temp_new_slots(old_num_slots);

    uint16_t tuples_moved = 0;
    for (const auto& data : active_slots_data) {
        uint16_t new_offset = current_offset - data.slot.length;

        std::cout << "DEBUG: compactify(" << page_id << "): Moving SlotID=" << data.slot_id << " from "
                  << data.slot.offset << " to " << new_offset << " (Len:" << data.slot.length << ")" << std::endl;

        std::memmove(page_->getData() + new_offset, page_->getData() + data.slot.offset, data.slot.length);
        tuples_moved++;

        temp_new_slots[data.slot_id].offset = new_offset;
        temp_new_slots[data.slot_id].length = data.slot.length;

        current_offset = new_offset;
    }
    std::cout << "DEBUG: compactify(" << page_id << "): Finished " << tuples_moved << " memmove operations."
              << std::endl;

    uint16_t new_num_slots = active_slots_data.size();
    SlotEntry* slot_start_ptr =
        reinterpret_cast<SlotEntry*>(page_->getData() + HEADER_START_OFFSET + sizeof(uint16_t) * 2);

    std::sort(active_slots_data.begin(), active_slots_data.end(),
              [](const SlotData& a, const SlotData& b) { return a.slot_id < b.slot_id; });

    for (uint16_t i = 0; i < new_num_slots; ++i) {
        uint16_t original_slot_id = active_slots_data[i].slot_id;
        slot_start_ptr[i] = temp_new_slots[original_slot_id];
        std::cout << "DEBUG: compactify(" << page_id << "): Rewriting Slot " << i << " (was S" << original_slot_id
                  << "). New Off=" << slot_start_ptr[i].offset << ", Len=" << slot_start_ptr[i].length << std::endl;
    }

    *getFreeSpacePtr() = current_offset;
    *getNumSlotsPtr() = new_num_slots;

    if (new_num_slots < old_num_slots) {
        size_t size_to_clear = (old_num_slots - new_num_slots) * sizeof(SlotEntry);
        std::memset(slot_start_ptr + new_num_slots, 0, size_to_clear);
        std::cout << "DEBUG: compactify(" << page_id << "): Cleared " << size_to_clear << " bytes of slot tail."
                  << std::endl;
    }

    std::cout << "DEBUG: compactify(" << page_id << "): END. NewFreePtr=" << *getFreeSpacePtr()
              << ", NewNumSlots=" << *getNumSlotsPtr() << std::endl;

    page_->setDirty(true);
}

}  // namespace storage