#include "db/buffer/buffer_pool_manager.hpp"

#include <algorithm>

namespace storage {

BufferPoolManager::BufferPoolManager(size_t pool_size)
    : pool_size_(pool_size), disk_manager_(&DiskManager::instance()) {
    pages_.reserve(pool_size);
    for (size_t i = 0; i < pool_size; ++i) {
        pages_.emplace_back(disk_manager_->getPageSize());
        free_list_.push_back(static_cast<frame_id_t>(i));
    }
    replacer_ = std::make_unique<LRUReplacer>(pool_size);
}

BufferPoolManager::~BufferPoolManager() {
    flushAllPages();
}

Page* BufferPoolManager::allocateFrame(frame_id_t* frame_id) {
    if (!free_list_.empty()) {
        *frame_id = free_list_.back();
        free_list_.pop_back();
        return &pages_[*frame_id];
    }

    if (replacer_->victim(frame_id)) {
        Page* victim_page = &pages_[*frame_id];

        if (victim_page->isDirty()) {
            disk_manager_->writePage(victim_page->getPageId(), *victim_page);
        }

        page_table_.erase(victim_page->getPageId());

        victim_page->resetData();
        victim_page->setPageId(Page::INVALID_PAGE_ID);
        victim_page->setDirty(false);
        victim_page->setPinCount(0);

        return victim_page;
    }

    return nullptr;
}

void BufferPoolManager::freeFrame(frame_id_t frame_id) {
    Page* page = &pages_[frame_id];

    page_table_.erase(page->getPageId());

    if (page->isDirty()) {
        disk_manager_->writePage(page->getPageId(), *page);
    }

    page->resetData();
    page->setPageId(Page::INVALID_PAGE_ID);
    page->setDirty(false);
    page->setPinCount(0);

    free_list_.push_back(frame_id);
}

Page* BufferPoolManager::fetchPage(page_id_t page_id) {
    std::lock_guard<std::mutex> lock(latch_);

    if (page_table_.count(page_id)) {
        frame_id_t frame_id = page_table_[page_id];
        Page* page = &pages_[frame_id];
        page->incPinCount();

        replacer_->pin(frame_id);

        return page;
    }

    if (disk_manager_->isPageAllocated(page_id) == false) {
        return nullptr;
    }

    frame_id_t frame_id;
    Page* page = allocateFrame(&frame_id);

    if (page == nullptr) {
        return nullptr;
    }

    try {
        disk_manager_->readPage(page_id, *page);
    } catch (const std::exception& e) {
        page->setPageId(Page::INVALID_PAGE_ID);
        page->setPinCount(0); 
        free_list_.push_back(frame_id);
        return nullptr;
    }

    page->setPageId(page_id);
    page->setPinCount(1);
    page_table_[page_id] = frame_id;

    replacer_->pin(frame_id);

    return page;
}

bool BufferPoolManager::unpinPage(page_id_t page_id, bool is_dirty) {
    std::lock_guard<std::mutex> lock(latch_);

    if (!page_table_.count(page_id)) {
        return false;
    }

    frame_id_t frame_id = page_table_[page_id];
    Page* page = &pages_[frame_id];

    if (page->getPinCount() == 0) {
        return false;
    }

    page->decPinCount();
    page->setDirty(page->isDirty() || is_dirty);

    if (page->getPinCount() == 0) {
        replacer_->unpin(frame_id);
    }
    return true;
}

bool BufferPoolManager::flushPage(page_id_t page_id) {
    std::lock_guard<std::mutex> lock(latch_);

    if (!page_table_.count(page_id)) {
        return false;
    }

    frame_id_t frame_id = page_table_[page_id];
    Page* page = &pages_[frame_id];

    if (page->isDirty()) {
        disk_manager_->writePage(page_id, *page);
        page->setDirty(false);
    }
    return true;
}

void BufferPoolManager::flushAllPages() {
    std::lock_guard<std::mutex> lock(latch_);
    for (size_t i = 0; i < pool_size_; ++i) {
        Page* page = &pages_[i];
        if (page->getPageId() != Page::INVALID_PAGE_ID && page->isDirty()) {
            disk_manager_->writePage(page->getPageId(), *page);
            page->setDirty(false);
        }
    }
}

Page* BufferPoolManager::newPage(page_id_t* page_id) {
    std::lock_guard<std::mutex> lock(latch_);

    frame_id_t frame_id;
    Page* page = allocateFrame(&frame_id);

    if (page == nullptr) {
        return nullptr;
    }

    page_id_t new_page_id = disk_manager_->allocatePage();

    page->setPageId(new_page_id);
    page->setPinCount(1);
    page->setDirty(true);

    page_table_[new_page_id] = frame_id;

    replacer_->pin(frame_id);

    *page_id = new_page_id;
    return page;
}

bool BufferPoolManager::deletePage(page_id_t page_id) {
    std::lock_guard<std::mutex> lock(latch_);

    if (!page_table_.count(page_id)) {
        disk_manager_->deallocatePage(page_id);
        return true;
    }

    frame_id_t frame_id = page_table_[page_id];
    Page* page = &pages_[frame_id];

    if (page->getPinCount() > 0) {
        return false;
    }

    replacer_->pin(frame_id);

    freeFrame(frame_id);

    disk_manager_->deallocatePage(page_id);
    return true;
}

}  // namespace storage