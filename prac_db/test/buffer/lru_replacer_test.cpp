#include "db/buffer/lru_replacer.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

#include "db/storage/page.hpp"

using storage::frame_id_t;


TEST(LRUReplacerTest, HitAndReorder) {
    storage::LRUReplacer lru(5);

    lru.unpin(1);
    lru.unpin(2);
    lru.unpin(3);
    lru.unpin(4);

    lru.pin(2);
    lru.unpin(2);

    lru.pin(1);
    lru.unpin(1);

    frame_id_t victim;

    ASSERT_TRUE(lru.victim(&victim));
    ASSERT_EQ(victim, 3);

    ASSERT_TRUE(lru.victim(&victim));
    ASSERT_EQ(victim, 4);

    lru.pin(2);

    ASSERT_TRUE(lru.victim(&victim));
    ASSERT_EQ(victim, 1);

    ASSERT_FALSE(lru.victim(&victim));
}

TEST(LRUReplacerTest, PinUnpinNonExistent) {
    storage::LRUReplacer lru(5);
    frame_id_t victim;

    lru.pin(10);
    ASSERT_EQ(lru.size(), 0);

    lru.unpin(10);
    ASSERT_EQ(lru.size(), 1);

    lru.pin(10);
    ASSERT_EQ(lru.size(), 0);

    lru.pin(10);
    ASSERT_EQ(lru.size(), 0);

    ASSERT_FALSE(lru.victim(&victim));
}

TEST(LRUReplacerTest, Boundary_SizeLimit) {
    storage::LRUReplacer lru(3);
    frame_id_t victim;

    lru.unpin(1);
    lru.unpin(2);
    lru.unpin(3);
    lru.unpin(4);
    lru.unpin(5);

    ASSERT_EQ(lru.size(), 5);

    ASSERT_TRUE(lru.victim(&victim));
    ASSERT_EQ(victim, 1);
    ASSERT_EQ(lru.size(), 4);

    ASSERT_TRUE(lru.victim(&victim));
    ASSERT_EQ(victim, 2);
    ASSERT_EQ(lru.size(), 3);

    lru.pin(3);
    lru.pin(4);
    lru.pin(5);
    ASSERT_EQ(lru.size(), 0);

    ASSERT_FALSE(lru.victim(&victim));
}

TEST(LRUReplacerTest, Concurrency_Stress) {
    const int num_threads = 8;
    const int num_ops_per_thread = 1000;
    const int total_frames = num_threads * num_ops_per_thread;
    storage::LRUReplacer lru(total_frames);

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&lru, i]() {
            for (int j = 0; j < num_ops_per_thread; ++j) {
                frame_id_t fid = i * num_ops_per_thread + j;
                lru.unpin(fid);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
    threads.clear();

    ASSERT_EQ(lru.size(), total_frames);

    std::vector<frame_id_t> fids(total_frames);
    std::iota(fids.begin(), fids.end(), 0);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&lru, &fids, i]() {
            std::mt19937 gen(i);
            std::uniform_int_distribution<> distrib(0, fids.size() - 1);

            for (int k = 0; k < num_ops_per_thread; ++k) {
                frame_id_t fid = fids[distrib(gen)];

                lru.pin(fid);
                lru.unpin(fid);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    ASSERT_EQ(lru.size(), total_frames);

    frame_id_t v;
    int count = 0;
    while (lru.victim(&v)) {
        count++;
    }
    ASSERT_EQ(count, total_frames);
}