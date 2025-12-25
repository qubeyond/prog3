#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "common/config.hpp"
#include "common/defs.hpp"
#include "db/storage/page.hpp"

namespace storage {

class DiskManager {
   public:
    static DiskManager& instance();

    DiskManager(const DiskManager&) = delete;
    DiskManager& operator=(const DiskManager&) = delete;

    void createDbFile(const std::string& db_file_name);
    void destroyDbFile(const std::string& db_file_name);
    void openDbFile(const std::string& db_file_name);
    void closeDbFile();

    void readPage(page_id_t page_id, Page& page);
    void writePage(page_id_t page_id, const Page& page);

    page_id_t allocatePage();
    void deallocatePage(page_id_t page_id);

    bool isPageAllocated(page_id_t page_id) const;

    size_t getPageSize() const;
    std::string getFilePath() const;

   private:
    DiskManager();
    ~DiskManager();

    void readMetadata();
    void writeMetadata();

    inline static constexpr size_t METADATA_NEXT_PAGE_ID_OFFSET = 0;
    inline static constexpr size_t METADATA_FREE_PAGES_COUNT_OFFSET = sizeof(page_id_t);
    inline static constexpr size_t METADATA_FREE_PAGES_LIST_OFFSET = sizeof(page_id_t) * 2;

    std::fstream db_io_;
    std::string db_file_path_;
    size_t page_size_;

    std::mutex mutex_;

    page_id_t next_page_id_ = 0;
    std::vector<page_id_t> free_pages_;
};

}  // namespace storage