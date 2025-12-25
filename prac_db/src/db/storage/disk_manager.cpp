#include "db/storage/disk_manager.hpp"

#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <vector>

namespace storage {

using common::defs::METADATA_PAGE_ID;

DiskManager& DiskManager::instance() {
    static DiskManager inst;
    return inst;
}

DiskManager::DiskManager() {
    auto& cfg = common::Config::instance();
    page_size_ = cfg.getInt("page_size", common::defs::PAGE_SIZE);

    std::string db_dir = cfg.getString("db_path", common::defs::DEFAULT_DB_PATH);

    if (!std::filesystem::exists(db_dir)) {
        std::filesystem::create_directories(db_dir);
    }

    db_file_path_ = db_dir;
}

DiskManager::~DiskManager() {
    closeDbFile();
}

void DiskManager::createDbFile(const std::string& db_file_name) {
    std::filesystem::path full_path = std::filesystem::path(db_file_path_) / db_file_name;

    if (std::filesystem::exists(full_path)) {
        return;
    }

    std::ofstream ofs(full_path, std::ios::binary);
    if (!ofs.is_open()) {
        std::string msg = "Cannot create DB file: " + full_path.string();
        throw std::runtime_error(msg);
    }

    Page metadata_page(page_size_);
    char* data = metadata_page.getData();
    page_id_t initial_next_id = 1;
    page_id_t initial_count = 0;

    std::memcpy(data + METADATA_NEXT_PAGE_ID_OFFSET, &initial_next_id, sizeof(page_id_t));
    std::memcpy(data + METADATA_FREE_PAGES_COUNT_OFFSET, &initial_count, sizeof(page_id_t));

    ofs.write(metadata_page.getData(), page_size_);

    ofs.close();
}

void DiskManager::destroyDbFile(const std::string& db_file_name) {
    std::filesystem::path full_path = std::filesystem::path(db_file_path_) / db_file_name;
    if (std::filesystem::exists(full_path)) {
        std::filesystem::remove(full_path);
    }
}

void DiskManager::openDbFile(const std::string& db_file_name) {
    closeDbFile();
    std::filesystem::path full_path = std::filesystem::path(db_file_path_) / db_file_name;

    db_io_.open(full_path, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_io_.is_open()) {
        std::string msg = "Cannot open DB file: " + full_path.string();
        throw std::runtime_error(msg);
    }

    readMetadata();
}

void DiskManager::closeDbFile() {
    if (db_io_.is_open()) {
        writeMetadata();
        db_io_.close();
        next_page_id_ = 0;
        free_pages_.clear();
    }
}

void DiskManager::readPage(page_id_t page_id, Page& page) {
    if (!db_io_.is_open()) {
        std::string msg = "Attempt to read page " + std::to_string(page_id) + " but DB file is not open.";
        throw std::runtime_error(msg);
    }

    if (page_id < 0 && page_id != common::defs::METADATA_PAGE_ID) {
        std::string msg = "Negative page id: " + std::to_string(page_id);
        throw std::runtime_error(msg);
    }

    std::streamoff offset = static_cast<std::streamoff>(page_id) * page_size_;

    db_io_.clear();
    db_io_.seekg(offset);
    if (db_io_.fail()) {
        std::string msg =
            "Failed to seek to page offset " + std::to_string(offset) + " for page " + std::to_string(page_id);
        throw std::runtime_error(msg);
    }

    db_io_.read(page.getData(), page_size_);

    if (db_io_.fail() && !db_io_.eof()) {
        std::string msg = "Failed to read page " + std::to_string(page_id);
        throw std::runtime_error(msg);
    }

    if (db_io_.gcount() < static_cast<std::streamsize>(page_size_)) {
        page.resetData();
        std::string msg = "Attempted to read non-existent page " + std::to_string(page_id) + ". Only read " +
                          std::to_string(db_io_.gcount()) + " bytes.";
        throw std::out_of_range(msg);
    }

    page.setPageId(page_id);
}

void DiskManager::writePage(page_id_t page_id, const Page& page) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (page_id < 0) {
        throw std::runtime_error("Negative page id");
    }

    if (!db_io_.is_open()) {
        throw std::runtime_error("DB is not open");
    }

    if (page_id == METADATA_PAGE_ID) {
        writeMetadata();
        return;
    }

    db_io_.clear();
    db_io_.seekp(0, std::ios::end);
    std::streampos end_pos = db_io_.tellp();
    page_id_t existing_pages = static_cast<page_id_t>(end_pos / static_cast<std::streamoff>(page_size_));

    if (page_id >= existing_pages) {
        std::vector<char> zero(page_size_, 0);
        for (page_id_t id = existing_pages; id < page_id; ++id) {
            db_io_.write(zero.data(), page_size_);
        }
    }

    std::streamoff offset = static_cast<std::streamoff>(page_id) * page_size_;

    db_io_.clear();
    db_io_.seekp(offset);

    db_io_.write(page.getData(), page_size_);
    db_io_.flush();
}

page_id_t DiskManager::allocatePage() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!db_io_.is_open()) {
        std::string msg = "Attempt to allocate new page but DB file is not open.";
        throw std::runtime_error(msg);
    }

    page_id_t new_page_id;

    if (!free_pages_.empty()) {
        new_page_id = free_pages_.back();
        free_pages_.pop_back();
    } else {
        new_page_id = next_page_id_;
        next_page_id_++;

        if (new_page_id == METADATA_PAGE_ID) {
            new_page_id = next_page_id_;
            next_page_id_++;
        }

        std::streamoff offset = static_cast<std::streamoff>(new_page_id) * page_size_;
        db_io_.clear();
        db_io_.seekp(offset);

        std::vector<char> zero_data(page_size_, 0);
        db_io_.write(zero_data.data(), page_size_);

        if (db_io_.fail()) {
            std::string msg = "Failed to allocate new page.";
            throw std::runtime_error(msg);
        }

        db_io_.flush();
    }

    writeMetadata();

    return new_page_id;
}

void DiskManager::deallocatePage(page_id_t page_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (page_id == METADATA_PAGE_ID || page_id >= next_page_id_) {
        return;
    }

    for (page_id_t id : free_pages_) {
        if (id == page_id) {
            return;
        }
    }

    free_pages_.push_back(page_id);
    writeMetadata();
}

size_t DiskManager::getPageSize() const {
    return page_size_;
}

std::string DiskManager::getFilePath() const {
    return db_file_path_;
}

void DiskManager::readMetadata() {
    Page metadata_page(page_size_);

    std::streamoff offset = static_cast<std::streamoff>(METADATA_PAGE_ID) * page_size_;
    db_io_.clear();
    db_io_.seekg(offset);
    db_io_.read(metadata_page.getData(), page_size_);

    const char* data = metadata_page.getData();

    std::memcpy(&next_page_id_, data + METADATA_NEXT_PAGE_ID_OFFSET, sizeof(page_id_t));

    if (next_page_id_ == 0) {
        next_page_id_ = 1;
    }

    page_id_t count = 0;
    std::memcpy(&count, data + METADATA_FREE_PAGES_COUNT_OFFSET, sizeof(page_id_t));

    free_pages_.clear();
    free_pages_.reserve(count);
    const char* list_ptr = data + METADATA_FREE_PAGES_LIST_OFFSET;

    for (page_id_t i = 0; i < count; ++i) {
        page_id_t free_id;
        std::memcpy(&free_id, list_ptr + i * sizeof(page_id_t), sizeof(page_id_t));
        free_pages_.push_back(free_id);
    }
}

void DiskManager::writeMetadata() {
    Page metadata_page(page_size_);
    char* data = metadata_page.getData();

    size_t required_size = METADATA_FREE_PAGES_LIST_OFFSET + free_pages_.size() * sizeof(page_id_t);
    if (required_size > page_size_) {
        throw std::runtime_error("Metadata page size limit exceeded.");
    }

    std::memcpy(data + METADATA_NEXT_PAGE_ID_OFFSET, &next_page_id_, sizeof(page_id_t));

    page_id_t count = static_cast<page_id_t>(free_pages_.size());
    std::memcpy(data + METADATA_FREE_PAGES_COUNT_OFFSET, &count, sizeof(page_id_t));

    char* list_ptr = data + METADATA_FREE_PAGES_LIST_OFFSET;
    for (size_t i = 0; i < free_pages_.size(); ++i) {
        std::memcpy(list_ptr + i * sizeof(page_id_t), &free_pages_[i], sizeof(page_id_t));
    }

    std::streamoff offset = static_cast<std::streamoff>(METADATA_PAGE_ID) * page_size_;
    db_io_.clear();
    db_io_.seekp(offset);
    db_io_.write(metadata_page.getData(), page_size_);
    db_io_.flush();
}

bool DiskManager::isPageAllocated(page_id_t page_id) const {
    if (page_id < 0 || page_id >= next_page_id_) {
        return false;
    }
    if (page_id == METADATA_PAGE_ID) {
        return true;
    }

    return std::find(free_pages_.begin(), free_pages_.end(), page_id) == free_pages_.end();
}

}  // namespace storage