#include "db/buffer/buffer_pool_manager.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "db/storage/disk_manager.hpp"
#include "db/storage/page.hpp"

using storage::BufferPoolManager;
using storage::DiskManager;
using storage::frame_id_t;
using storage::Page;
using storage::page_id_t;

namespace {
const std::string TEST_BPM_DB_FILE = "bpm_test_db.db";
const size_t POOL_SIZE = 10;
const int NUM_PAGES_SMALL = 5;
}  // namespace

class BufferPoolManagerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        dm_ = &DiskManager::instance();
        dm_->destroyDbFile(TEST_BPM_DB_FILE);
        dm_->createDbFile(TEST_BPM_DB_FILE);
        dm_->openDbFile(TEST_BPM_DB_FILE);

        bpm_ = std::make_unique<BufferPoolManager>(POOL_SIZE);
    }

    void TearDown() override {
        bpm_.reset();
        dm_->closeDbFile();
        dm_->destroyDbFile(TEST_BPM_DB_FILE);
    }

    DiskManager* dm_ = nullptr;
    std::unique_ptr<BufferPoolManager> bpm_ = nullptr;
};

TEST_F(BufferPoolManagerTest, NewFetchUnpin) {
    page_id_t page_id1;
    Page* page1 = bpm_->newPage(&page_id1);

    ASSERT_NE(page1, nullptr);
    ASSERT_EQ(page1->getPageId(), page_id1);
    ASSERT_EQ(page1->getPinCount(), 1);
    ASSERT_TRUE(page1->isDirty());

    const std::string test_data = "Hello BPM 1";
    std::memcpy(page1->getData(), test_data.c_str(), test_data.length() + 1);

    ASSERT_TRUE(bpm_->unpinPage(page_id1, true));

    Page* page_fetch = bpm_->fetchPage(page_id1);
    ASSERT_NE(page_fetch, nullptr);
    ASSERT_EQ(page_fetch->getPageId(), page_id1);
    ASSERT_EQ(page_fetch->getPinCount(), 1);
    ASSERT_STREQ(page_fetch->getData(), test_data.c_str());

    ASSERT_TRUE(bpm_->unpinPage(page_id1, false));
}

TEST_F(BufferPoolManagerTest, SimpleFlushAndDiskRead) {
    page_id_t page_id;
    Page* page = bpm_->newPage(&page_id);
    const std::string test_data = "Flush check";
    std::memcpy(page->getData(), test_data.c_str(), test_data.length() + 1);

    ASSERT_TRUE(bpm_->unpinPage(page_id, true));

    ASSERT_TRUE(bpm_->flushPage(page_id));

    Page* fetched = bpm_->fetchPage(page_id);
    ASSERT_NE(fetched, nullptr);
    ASSERT_FALSE(fetched->isDirty());
    bpm_->unpinPage(page_id, false);

    bpm_.reset();

    Page disk_read_page(dm_->getPageSize());
    dm_->readPage(page_id, disk_read_page);
    ASSERT_STREQ(disk_read_page.getData(), test_data.c_str());
}

TEST_F(BufferPoolManagerTest, LRUVictimSelection) {
    std::vector<page_id_t> ids;
    std::string base_data = "Data for page ";

    for (size_t i = 0; i < POOL_SIZE; ++i) {
        page_id_t id;
        Page* page = bpm_->newPage(&id);
        std::memcpy(page->getData(), (base_data + std::to_string(i + 1)).c_str(), base_data.length() + 2);
        ASSERT_TRUE(bpm_->unpinPage(id, false));
        ids.push_back(id);
    }

    Page* p1 = bpm_->fetchPage(ids[0]);
    ASSERT_NE(p1, nullptr);
    ASSERT_TRUE(bpm_->unpinPage(ids[0], false));

    page_id_t id_evicted;
    Page* page_new = bpm_->newPage(&id_evicted);
    ASSERT_NE(page_new, nullptr);

    Page* p2_check = bpm_->fetchPage(ids[1]);
    ASSERT_NE(p2_check, nullptr);

    ASSERT_EQ(p2_check->getPageId(), ids[1]);

    std::string expected_data_p1 = base_data + "1";
    ASSERT_STREQ(p1->getData(), expected_data_p1.c_str());

    bpm_->unpinPage(id_evicted, false);
    bpm_->unpinPage(ids[1], false);
}

TEST_F(BufferPoolManagerTest, PinningPreventsEviction) {
    std::vector<page_id_t> pinned_ids;

    for (int i = 0; i < NUM_PAGES_SMALL; ++i) {
        page_id_t id;
        bpm_->newPage(&id);
        pinned_ids.push_back(id);
    }

    std::vector<page_id_t> unpinned_ids;
    for (int i = 0; i < NUM_PAGES_SMALL; ++i) {
        page_id_t id;
        bpm_->newPage(&id);
        unpinned_ids.push_back(id);
        ASSERT_TRUE(bpm_->unpinPage(id, false));
    }

    page_id_t id_new1;
    Page* page_new1 = bpm_->newPage(&id_new1);
    ASSERT_NE(page_new1, nullptr);
    ASSERT_EQ(page_new1->getPageId(), id_new1);

    Page* p_pinned = bpm_->fetchPage(pinned_ids[0]);
    ASSERT_NE(p_pinned, nullptr);

    ASSERT_TRUE(bpm_->unpinPage(pinned_ids[0], false));

    page_id_t id_new2;
    Page* page_new2 = bpm_->newPage(&id_new2);
    ASSERT_NE(page_new2, nullptr);

    Page* p_check = bpm_->fetchPage(pinned_ids[0]);
    ASSERT_NE(p_check, nullptr);

    for (page_id_t id : pinned_ids)
        bpm_->deletePage(id);
    for (page_id_t id : unpinned_ids)
        bpm_->deletePage(id);
    bpm_->deletePage(id_new1);
    bpm_->deletePage(id_new2);
}

TEST_F(BufferPoolManagerTest, DeletePageLogic) {
    page_id_t page_id;
    Page* page = bpm_->newPage(&page_id);

    std::memcpy(page->getData(), "To be deleted", 14);

    ASSERT_FALSE(bpm_->deletePage(page_id));

    ASSERT_TRUE(bpm_->unpinPage(page_id, true));

    ASSERT_TRUE(bpm_->deletePage(page_id));

    ASSERT_EQ(bpm_->fetchPage(page_id), nullptr);

    page_id_t reallocated_id = dm_->allocatePage();
    ASSERT_EQ(reallocated_id, page_id);
    dm_->deallocatePage(reallocated_id);
}

TEST_F(BufferPoolManagerTest, FlushAllDirtyCheck) {
    std::vector<page_id_t> ids;

    for (int i = 0; i < NUM_PAGES_SMALL; ++i) {
        page_id_t id;
        Page* page = bpm_->newPage(&id);
        std::memcpy(page->getData(), ("Data " + std::to_string(i)).c_str(), 6);
        ids.push_back(id);
        ASSERT_TRUE(bpm_->unpinPage(id, true));
    }

    bpm_->flushAllPages();

    for (page_id_t id : ids) {
        Page* page = bpm_->fetchPage(id);
        ASSERT_FALSE(page->isDirty());
        ASSERT_TRUE(bpm_->unpinPage(id, false));
    }

    bpm_.reset();

    bpm_ = std::make_unique<BufferPoolManager>(POOL_SIZE);

    for (int i = 0; i < NUM_PAGES_SMALL; ++i) {
        Page* page = bpm_->fetchPage(ids[i]);
        ASSERT_NE(page, nullptr);
        ASSERT_STREQ(page->getData(), ("Data " + std::to_string(i)).c_str());
        bpm_->unpinPage(ids[i], false);
        bpm_->deletePage(ids[i]);
    }
}