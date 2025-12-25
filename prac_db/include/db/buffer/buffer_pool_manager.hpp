#pragma once

#include <cstddef>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "common/config.hpp"
#include "db/buffer/lru_replacer.hpp"
#include "db/storage/disk_manager.hpp"
#include "db/storage/page.hpp"

namespace storage {

class BufferPoolManager final {
   public:
    explicit BufferPoolManager(size_t pool_size);
    ~BufferPoolManager();

    BufferPoolManager(const BufferPoolManager&) = delete;
    void operator=(const BufferPoolManager&) = delete;

    Page* fetchPage(page_id_t page_id);
    bool unpinPage(page_id_t page_id, bool is_dirty);
    bool flushPage(page_id_t page_id);
    void flushAllPages();
    Page* newPage(page_id_t* page_id);
    bool deletePage(page_id_t page_id);

   private:
    Page* allocateFrame(frame_id_t* frame_id);
    void freeFrame(frame_id_t frame_id);

    size_t pool_size_;
    std::vector<Page> pages_;
    std::unordered_map<page_id_t, frame_id_t> page_table_;
    std::vector<frame_id_t> free_list_;
    std::unique_ptr<LRUReplacer> replacer_;
    DiskManager* disk_manager_;
    std::mutex latch_;
};

}  // namespace storage