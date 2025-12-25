#include "db/storage/slotted_page.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <random>
#include <stdexcept>
#include <vector>

#include "common/config.hpp"
#include "db/buffer/buffer_pool_manager.hpp"
#include "db/storage/disk_manager.hpp"
#include "db/storage/page.hpp"
#include "db/storage/tuple.hpp"

using storage::BufferPoolManager;
using storage::DiskManager;
using storage::Page;
using storage::page_id_t;
using storage::SlottedPage;
using storage::Tuple;

namespace {
const std::string TEST_DB_FILE = "slotted_page_test_db.db";
const size_t POOL_SIZE = 10;
}  // namespace

class SlottedPageTest : public ::testing::Test {
   protected:
    void SetUp() override {
        auto& cfg = common::Config::instance();
        cfg.load("../db.conf");

        dm_ = &DiskManager::instance();
        dm_->destroyDbFile(TEST_DB_FILE);
        dm_->createDbFile(TEST_DB_FILE);
        dm_->openDbFile(TEST_DB_FILE);

        bpm_ = std::make_unique<BufferPoolManager>(POOL_SIZE);

        page_id_t id;
        Page* p = bpm_->newPage(&id);

        *reinterpret_cast<page_id_t*>(p->getData() + SlottedPage::NEXT_PAGE_ID_OFFSET) = Page::INVALID_PAGE_ID;
        *reinterpret_cast<uint16_t*>(p->getData() + SlottedPage::HEADER_START_OFFSET) = p->getSize();
        *reinterpret_cast<uint16_t*>(p->getData() + SlottedPage::HEADER_START_OFFSET + sizeof(uint16_t)) = 0;

        p->setDirty(true);

        bpm_->unpinPage(id, true);
        page_ = bpm_->fetchPage(id);

        EXPECT_NE(page_, nullptr);
        EXPECT_EQ(page_->getPageId(), id);
    }

    void TearDown() override {
        if (page_) {
            bpm_->unpinPage(page_->getPageId(), false);
            bpm_->deletePage(page_->getPageId());
        }
        bpm_.reset();
        dm_->closeDbFile();
        dm_->destroyDbFile(TEST_DB_FILE);
    }

    DiskManager* dm_ = nullptr;
    std::unique_ptr<BufferPoolManager> bpm_ = nullptr;
    Page* page_ = nullptr;
};

TEST_F(SlottedPageTest, BasicInsertAndDelete) {
    std::string data1 = "Tuple Data 1";
    Tuple t1(data1.data(), data1.length() + 1);
    uint16_t s1;

    SlottedPage sp(page_);

    ASSERT_TRUE(sp.InsertTuple(t1, &s1));
    ASSERT_EQ(s1, 0);
    ASSERT_EQ(sp.getNumSlots(), 1);

    Tuple read_t = sp.GetTuple(s1);
    ASSERT_STREQ(read_t.getData(), data1.c_str());

    ASSERT_TRUE(sp.DeleteTuple(s1));
    ASSERT_EQ(sp.getNumSlots(), 0);
    ASSERT_TRUE(sp.isEmpty());

    ASSERT_EQ(sp.GetTuple(s1).getLength(), 0);
}

TEST_F(SlottedPageTest, CompactionAfterDelete) {
    std::string data1 = "A";
    std::string data2 = "BB";
    std::string data3 = "CCC";
    Tuple t1(data1.data(), data1.length() + 1);
    Tuple t2(data2.data(), data2.length() + 1);
    Tuple t3(data3.data(), data3.length() + 1);
    uint16_t s1, s2, s3;

    SlottedPage sp(page_);
    sp.InsertTuple(t1, &s1);
    sp.InsertTuple(t2, &s2);
    sp.InsertTuple(t3, &s3);

    ASSERT_TRUE(sp.DeleteTuple(s2));

    ASSERT_EQ(sp.getNumSlots(), 2);

    ASSERT_STREQ(sp.GetTuple(0).getData(), data1.c_str());
    ASSERT_STREQ(sp.GetTuple(1).getData(), data3.c_str());

    ASSERT_EQ(sp.GetTuple(2).getLength(), 0);
}


TEST_F(SlottedPageTest, UpdateTuple_LargerSize) {
    std::string data_short = "A";
    std::string data_long = "Very Long Tuple Data";
    Tuple t_short(data_short.data(), data_short.length() + 1);
    Tuple t_long(data_long.data(), data_long.length() + 1);
    uint16_t s;

    SlottedPage sp(page_);
    sp.InsertTuple(t_short, &s);
    uint16_t initial_free = sp.getFreeSpace();
    uint16_t required_extra = t_long.getLength() - t_short.getLength();

    ASSERT_TRUE(sp.UpdateTuple(s, t_long));

    Tuple updated = sp.GetTuple(s);
    ASSERT_STREQ(updated.getData(), data_long.c_str());
    ASSERT_EQ(sp.getFreeSpace(), initial_free - required_extra);
}

TEST_F(SlottedPageTest, Critical_UpdateTuple_TooLarge) {
    SlottedPage sp(page_);
    Tuple t_small("S", 2);
    uint16_t s;
    sp.InsertTuple(t_small, &s);

    size_t required_size = sp.getFreeSpace() + t_small.getLength() + 100;
    std::string data(required_size, 'X');
    Tuple t_large(data.data(), data.length() + 1);

    ASSERT_FALSE(sp.UpdateTuple(s, t_large));

    Tuple check = sp.GetTuple(s);
    ASSERT_STREQ(check.getData(), "S");
}

TEST_F(SlottedPageTest, StressTest_InsertUntilFull) {
    std::string data = "Tuple data.";
    Tuple t(data.data(), data.length() + 1);
    uint16_t slot_id;
    int insert_count = 0;
    SlottedPage sp(page_);

    while (sp.InsertTuple(t, &slot_id)) {
        insert_count++;
    }

    ASSERT_GT(insert_count, 1);
    ASSERT_EQ(sp.getNumSlots(), insert_count);

    for (int i = 0; i < insert_count; ++i) {
        Tuple read_t = sp.GetTuple(static_cast<uint16_t>(i));
        ASSERT_STREQ(read_t.getData(), data.c_str());
    }
}

TEST_F(SlottedPageTest, StressTest_DeleteAndReinsert) {
    std::vector<uint16_t> initial_slots;
    Tuple t("T", 2);
    SlottedPage sp(page_);

    for (int i = 0; i < 50; ++i) {
        uint16_t s;
        if (sp.InsertTuple(t, &s)) {
            initial_slots.push_back(s);
        }
    }

    for (size_t i = 0; i < initial_slots.size(); i += 2) {
        sp.DeleteTuple(initial_slots[i]);
    }

    int reinsert_count = 0;
    for (int i = 0; i < 50; ++i) {
        uint16_t s;
        if (sp.InsertTuple(t, &s)) {
            reinsert_count++;
        }
    }

    ASSERT_GT(reinsert_count, 5);
}