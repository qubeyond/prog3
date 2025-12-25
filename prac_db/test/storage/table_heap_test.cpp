#include "db/storage/table_heap.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <vector>

#include "common/config.hpp"
#include "common/defs.hpp"
#include "db/buffer/buffer_pool_manager.hpp"
#include "db/storage/disk_manager.hpp"
#include "db/storage/page.hpp"
#include "db/storage/slotted_page.hpp"
#include "db/storage/table_iterator.hpp"
#include "db/storage/tuple.hpp"

using storage::BufferPoolManager;
using storage::DiskManager;
using storage::Page;
using storage::page_id_t;
using storage::RID;
using storage::SlottedPage;
using storage::TableHeap;
using storage::TableIterator;
using storage::Tuple;

namespace {
const std::string TEST_DB_FILE = "test_heap.db";
constexpr size_t TEST_PAGE_SIZE = common::defs::PAGE_SIZE;
constexpr size_t TEST_POOL_SIZE = 10;  
}  // namespace

class TableHeapTestFixture : public ::testing::Test {
   protected:
    void SetUp() override {
        auto& cfg = common::Config::instance();
        cfg.load("../db.conf");

        dm_ = &DiskManager::instance();
        dm_->destroyDbFile(TEST_DB_FILE);
        dm_->createDbFile(TEST_DB_FILE);
        dm_->openDbFile(TEST_DB_FILE);

        bpm_ = std::make_unique<BufferPoolManager>(TEST_POOL_SIZE);

        table_heap_ = std::make_unique<TableHeap>(bpm_.get());
        first_page_id_ = table_heap_->getFirstPageId();
    }

    void TearDown() override {
        bpm_.reset();

        dm_->closeDbFile();
        dm_->destroyDbFile(TEST_DB_FILE);
    }

    Tuple createTestTuple(size_t size, int seed) {
        std::vector<char> data(size);
        std::generate(data.begin(), data.end(), [n = seed]() mutable { return (char)(n++ % 256); });
        uint8_t expected_seed_byte = static_cast<uint8_t>(seed % 256);

        uint8_t actual_seed_byte = static_cast<uint8_t>(data[0]);

        if (actual_seed_byte != expected_seed_byte) {
            std::cout << "CRITICAL ERROR in createTestTuple: Seed mismatch! Expected " << (int)expected_seed_byte
                      << ", Got " << (int)actual_seed_byte << std::endl;
        }

        return Tuple(data.data(), size);
    }

    void assertTupleContent(const Tuple& expected, const Tuple& actual) {
        ASSERT_EQ(expected.getLength(), actual.getLength()) << "Tuple length mismatch.";
        bool content_match = std::memcmp(expected.getData(), actual.getData(), expected.getLength()) == 0;
        ASSERT_TRUE(content_match) << "Tuple content mismatch (memcmp failed). "
                                   << "Expected first byte (seed): " << (int)static_cast<uint8_t>(expected.getData()[0])
                                   << ", Actual first byte (seed): " << (int)static_cast<uint8_t>(actual.getData()[0]);
    }

    DiskManager* dm_ = nullptr;
    std::unique_ptr<BufferPoolManager> bpm_ = nullptr;
    std::unique_ptr<TableHeap> table_heap_ = nullptr;
    page_id_t first_page_id_;
};

TEST_F(TableHeapTestFixture, InsertTuple_Basic) {
    RID rid1, rid2;
    Tuple t1 = createTestTuple(20, 10);
    Tuple t2 = createTestTuple(30, 20);

    ASSERT_TRUE(table_heap_->insertTuple(t1, &rid1));
    ASSERT_TRUE(table_heap_->insertTuple(t2, &rid2));

    ASSERT_EQ(rid1.page_id, first_page_id_);
    ASSERT_EQ(rid1.slot_id, 0);
    ASSERT_EQ(rid2.page_id, first_page_id_);
    ASSERT_EQ(rid2.slot_id, 1);

    Tuple retrieved1 = table_heap_->getTuple(rid1);
    assertTupleContent(t1, retrieved1);
}

TEST_F(TableHeapTestFixture, InsertTuple_PageBoundaryCrossing) {
    size_t tuple_size = TEST_PAGE_SIZE / 4;
    std::vector<RID> rids;
    RID current_rid;
    int num_inserted = 0;

    while (true) {
        Tuple t = createTestTuple(tuple_size, num_inserted);
        if (!table_heap_->insertTuple(t, &current_rid)) {
            FAIL() << "Failed to insert tuple during boundary crossing setup.";
        }
        if (current_rid.page_id != first_page_id_) {
            break;  
        }
        rids.push_back(current_rid);
        num_inserted++;
    }

    ASSERT_NE(current_rid.page_id, first_page_id_);
    ASSERT_EQ(current_rid.slot_id, 0);

    page_id_t second_page_id = current_rid.page_id;

    Page* p1 = bpm_->fetchPage(first_page_id_);
    ASSERT_NE(p1, nullptr);
    SlottedPage sp1(p1);
    ASSERT_EQ(sp1.getNextPageId(), second_page_id);
    bpm_->unpinPage(first_page_id_, false);

    Tuple retrieved = table_heap_->getTuple(current_rid);
    assertTupleContent(createTestTuple(tuple_size, num_inserted), retrieved);
}

TEST_F(TableHeapTestFixture, InsertTuple_MultiplePageBoundary) {
    size_t tuple_size = TEST_PAGE_SIZE / 10;
    std::vector<RID> rids;
    int num_inserted = 0;
    page_id_t last_page_id = first_page_id_;

    for (int i = 0; i < TEST_PAGE_SIZE * 3 / tuple_size + 10; ++i) {
        RID current_rid;
        Tuple t = createTestTuple(tuple_size, num_inserted);
        if (!table_heap_->insertTuple(t, &current_rid)) {
            break;  
        }
        rids.push_back(current_rid);
        last_page_id = current_rid.page_id;
        num_inserted++;
    }

    ASSERT_GT(last_page_id, first_page_id_);

    Tuple retrieved_last = table_heap_->getTuple(rids.back());
    assertTupleContent(createTestTuple(tuple_size, num_inserted - 1), retrieved_last);

    Tuple retrieved_first = table_heap_->getTuple(rids.front());
    assertTupleContent(createTestTuple(tuple_size, 0), retrieved_first);
}

TEST_F(TableHeapTestFixture, GetTuple_InvalidRID) {
    RID invalid_rid_page = {Page::INVALID_PAGE_ID, 0};
    ASSERT_EQ(table_heap_->getTuple(invalid_rid_page).getLength(), 0);

    RID rid;
    Tuple t = createTestTuple(10, 1);
    table_heap_->insertTuple(t, &rid);

    RID invalid_rid_slot = {rid.page_id, 999};  
    ASSERT_EQ(table_heap_->getTuple(invalid_rid_slot).getLength(), 0);
}

TEST_F(TableHeapTestFixture, MarkDelete_Basic) {
    RID rid;
    Tuple t = createTestTuple(100, 5);
    ASSERT_TRUE(table_heap_->insertTuple(t, &rid));

    ASSERT_TRUE(table_heap_->markDelete(rid));

    ASSERT_EQ(table_heap_->getTuple(rid).getLength(), 0);
}

TEST_F(TableHeapTestFixture, UpdateTuple_SameSize) {
    RID rid;
    Tuple t_original = createTestTuple(50, 10);
    table_heap_->insertTuple(t_original, &rid);

    Tuple t_updated = createTestTuple(50, 11);  

    ASSERT_TRUE(table_heap_->updateTuple(rid, t_updated));
    assertTupleContent(t_updated, table_heap_->getTuple(rid));
}

TEST_F(TableHeapTestFixture, UpdateTuple_SmallerThenLarger) {
    RID rid;
    Tuple t_original = createTestTuple(100, 1);
    table_heap_->insertTuple(t_original, &rid);

    Tuple t_smaller = createTestTuple(50, 2);
    ASSERT_TRUE(table_heap_->updateTuple(rid, t_smaller));
    assertTupleContent(t_smaller, table_heap_->getTuple(rid));

    Tuple t_larger = createTestTuple(150, 3);
    ASSERT_TRUE(table_heap_->updateTuple(rid, t_larger));
    assertTupleContent(t_larger, table_heap_->getTuple(rid));
}

TEST_F(TableHeapTestFixture, UpdateTuple_NoSpaceGrowth) {
    RID rid1;
    size_t tuple_size = TEST_PAGE_SIZE / 2;
    Tuple t1 = createTestTuple(tuple_size, 1);
    ASSERT_TRUE(table_heap_->insertTuple(t1, &rid1));

    RID rid2;
    Tuple t_small = createTestTuple(10, 2);
    ASSERT_TRUE(table_heap_->insertTuple(t_small, &rid2));

    Tuple t_too_large = createTestTuple(TEST_PAGE_SIZE / 2, 3);

    ASSERT_FALSE(table_heap_->updateTuple(rid2, t_too_large));

    assertTupleContent(t_small, table_heap_->getTuple(rid2));
}


TEST_F(TableHeapTestFixture, TableIterator_Scan_MultiPage_Boundary) {
    size_t tuple_size = TEST_PAGE_SIZE / 4;
    int num_inserted = 0;
    std::vector<Tuple> inserted_tuples;

    while (num_inserted < 10) {  
        Tuple t = createTestTuple(tuple_size, num_inserted);
        RID current_rid;
        if (!table_heap_->insertTuple(t, &current_rid)) {
            break;
        }
        inserted_tuples.push_back(t);
        num_inserted++;
    }

    ASSERT_GT(num_inserted, 4); 

    int scan_count = 0;
    for (auto it = table_heap_->begin(); it != table_heap_->end(); ++it) {
        assertTupleContent(inserted_tuples[scan_count], *it);
        scan_count++;
    }

    ASSERT_EQ(scan_count, num_inserted);
}
