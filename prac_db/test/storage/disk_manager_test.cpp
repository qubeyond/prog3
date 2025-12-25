#include "db/storage/disk_manager.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/config.hpp"
#include "common/defs.hpp"
#include "db/storage/page.hpp"

using storage::page_id_t;

namespace {
const std::string TEST_DB_FILE = "test_db.db";
const int NUM_PAGES_STRESS = 1000;
const int NUM_DEALLOCATE_TEST = 100;
}  // namespace

class DiskManagerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        auto& cfg = common::Config::instance();
        cfg.load("../db.conf");

        dm_ = &storage::DiskManager::instance();
        dm_->destroyDbFile(TEST_DB_FILE);
        dm_->createDbFile(TEST_DB_FILE);
        dm_->openDbFile(TEST_DB_FILE);
    }

    void TearDown() override {
        dm_->closeDbFile();
        dm_->destroyDbFile(TEST_DB_FILE);
    }

    storage::DiskManager* dm_ = nullptr;
};

TEST_F(DiskManagerTest, PageInitialization) {
    size_t page_size = dm_->getPageSize();
    storage::Page page(page_size);

    ASSERT_EQ(page.getSize(), page_size);
    ASSERT_NE(page.getData(), nullptr);
    ASSERT_EQ(page.getPageId(), -1);
}

TEST_F(DiskManagerTest, PageWriteRead) {
    size_t page_size = dm_->getPageSize();
    storage::Page page(page_size);

    const std::string test_string = "Page Data Integrity Check.";
    std::memcpy(page.getData(), test_string.c_str(), test_string.length() + 1);

    ASSERT_STREQ(page.getData(), test_string.c_str());

    page.resetData();
    ASSERT_EQ(page.getData()[0], 0);
    ASSERT_EQ(page.getData()[page_size - 1], 0);
}

TEST_F(DiskManagerTest, AllocatePageCorrectness) {
    ASSERT_EQ(dm_->allocatePage(), 1);
    ASSERT_EQ(dm_->allocatePage(), 2);
    ASSERT_EQ(dm_->allocatePage(), 3);

    std::filesystem::path full_path =
        std::filesystem::path(common::Config::instance().getString("db_path", common::defs::DEFAULT_DB_PATH)) /
        TEST_DB_FILE;

    ASSERT_EQ(std::filesystem::file_size(full_path), dm_->getPageSize() * 4);
}

TEST_F(DiskManagerTest, DeallocatePageCorrectness) {
    page_id_t id1 = dm_->allocatePage();
    page_id_t id2 = dm_->allocatePage();
    page_id_t id3 = dm_->allocatePage();

    ASSERT_EQ(id1, 1);
    ASSERT_EQ(id2, 2);
    ASSERT_EQ(id3, 3);

    dm_->deallocatePage(id2);
    dm_->deallocatePage(id1);

    ASSERT_EQ(dm_->allocatePage(), id1);
    ASSERT_EQ(dm_->allocatePage(), id2);
    ASSERT_EQ(dm_->allocatePage(), 4);
}

TEST_F(DiskManagerTest, DeallocateMetadataPage) {
    page_id_t id1 = dm_->allocatePage();

    dm_->deallocatePage(common::defs::METADATA_PAGE_ID);

    ASSERT_EQ(dm_->allocatePage(), id1 + 1);
}

TEST_F(DiskManagerTest, ReallocationOrder) {
    std::vector<page_id_t> allocated_ids;
    for (int i = 0; i < NUM_DEALLOCATE_TEST; ++i) {
        allocated_ids.push_back(dm_->allocatePage());
    }

    std::vector<page_id_t> to_deallocate = {10, 20, 50, 99};

    for (page_id_t id : to_deallocate) {
        dm_->deallocatePage(id);
    }

    ASSERT_EQ(dm_->allocatePage(), 99);
    ASSERT_EQ(dm_->allocatePage(), 50);
    ASSERT_EQ(dm_->allocatePage(), 20);
    ASSERT_EQ(dm_->allocatePage(), 10);
    ASSERT_EQ(dm_->allocatePage(), 101);
}

TEST_F(DiskManagerTest, ReadWritePage) {
    size_t page_size = dm_->getPageSize();
    storage::Page write_page(page_size);

    const std::string test_string = "Hello, DBMS!";
    std::memcpy(write_page.getData(), test_string.c_str(), test_string.length() + 1);

    page_id_t id = dm_->allocatePage();
    dm_->writePage(id, write_page);

    storage::Page read_page(page_size);
    dm_->readPage(id, read_page);

    ASSERT_EQ(read_page.getPageId(), id);
    ASSERT_STREQ(read_page.getData(), test_string.c_str());
}

TEST_F(DiskManagerTest, EdgeCase_ReadNonExistentPage) {
    size_t page_size = dm_->getPageSize();
    storage::Page page(page_size);

    dm_->allocatePage();
    dm_->allocatePage();

    ASSERT_THROW(dm_->readPage(3, page), std::out_of_range);
    ASSERT_THROW(dm_->readPage(-1, page), std::runtime_error);
}

TEST_F(DiskManagerTest, EdgeCase_WritePastEnd) {
    size_t page_size = dm_->getPageSize();
    storage::Page page(page_size);

    dm_->allocatePage();

    page.getData()[0] = static_cast<char>(0xAA);
    dm_->writePage(5, page);

    std::filesystem::path full_path =
        std::filesystem::path(common::Config::instance().getString("db_path", common::defs::DEFAULT_DB_PATH)) /
        TEST_DB_FILE;

    ASSERT_EQ(std::filesystem::file_size(full_path), dm_->getPageSize() * 6);

    storage::Page gap(page_size);
    dm_->readPage(2, gap);
    ASSERT_EQ(gap.getData()[0], 0);

    storage::Page check(page_size);
    dm_->readPage(5, check);
    ASSERT_EQ(check.getData()[0], static_cast<char>(0xAA));
}

TEST_F(DiskManagerTest, StressTest_SequentialReadWrite) {
    size_t page_size = dm_->getPageSize();
    std::vector<char> expected(page_size);
    std::iota(expected.begin(), expected.end(), 1);

    storage::Page write(page_size), read(page_size);

    for (int i = 1; i <= NUM_PAGES_STRESS; ++i) {
        write.getData()[0] = static_cast<char>(i % 256);
        std::memcpy(write.getData() + 1, expected.data() + 1, page_size - 1);

        page_id_t allocated_id = dm_->allocatePage();
        ASSERT_EQ(allocated_id, i);
        dm_->writePage(i, write);
    }

    for (int i = 1; i <= NUM_PAGES_STRESS; ++i) {
        dm_->readPage(i, read);
        ASSERT_EQ(read.getPageId(), i);
        ASSERT_EQ(read.getData()[0], static_cast<char>(i % 256));

        ASSERT_EQ(std::memcmp(read.getData() + 1, expected.data() + 1, page_size - 1), 0);
    }
}

TEST_F(DiskManagerTest, StressTest_RandomAccess) {
    size_t page_size = dm_->getPageSize();
    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < NUM_PAGES_STRESS; ++i) {
        dm_->allocatePage();
    }

    std::vector<page_id_t> ids(NUM_PAGES_STRESS);
    std::iota(ids.begin(), ids.end(), 1);
    std::shuffle(ids.begin(), ids.end(), g);

    storage::Page write(page_size), read(page_size);
    std::unordered_map<page_id_t, uint32_t> checksums;

    for (page_id_t id : ids) {
        std::generate(write.getData(), write.getData() + page_size, [&]() { return static_cast<char>(g() % 256); });

        uint32_t checksum = 0;
        for (size_t i = 0; i < page_size; ++i) {
            checksum ^= static_cast<uint32_t>(write.getData()[i]);
        }
        checksums[id] = checksum;

        dm_->writePage(id, write);
    }

    std::shuffle(ids.begin(), ids.end(), g);

    for (page_id_t id : ids) {
        dm_->readPage(id, read);

        ASSERT_EQ(read.getPageId(), id);

        uint32_t c = 0;
        for (size_t i = 0; i < page_size; ++i) {
            c ^= static_cast<uint32_t>(read.getData()[i]);
        }

        ASSERT_EQ(c, checksums[id]);
    }
}

TEST_F(DiskManagerTest, PersistenceTest_Metadata) {
    for (int i = 0; i < NUM_DEALLOCATE_TEST; ++i) {
        dm_->allocatePage();
    }

    dm_->deallocatePage(5);
    dm_->deallocatePage(50);

    ASSERT_EQ(dm_->allocatePage(), 50);

    dm_->closeDbFile();
    dm_->openDbFile(TEST_DB_FILE);

    ASSERT_EQ(dm_->allocatePage(), 5);
    ASSERT_EQ(dm_->allocatePage(), NUM_DEALLOCATE_TEST + 1);
}

TEST_F(DiskManagerTest, PersistenceTest_Data) {
    size_t page_size = dm_->getPageSize();
    storage::Page write(page_size), read(page_size);
    page_id_t test_id = 10;
    const std::string test_data = "Data to persist across closing.";

    for (int i = 0; i <= test_id; ++i) {
        dm_->allocatePage();
    }

    std::memcpy(write.getData(), test_data.c_str(), test_data.length() + 1);
    dm_->writePage(test_id, write);

    dm_->closeDbFile();

    dm_->openDbFile(TEST_DB_FILE);

    dm_->readPage(test_id, read);
    ASSERT_STREQ(read.getData(), test_data.c_str());
}