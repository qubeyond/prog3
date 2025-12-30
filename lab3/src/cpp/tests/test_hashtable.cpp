#include <gtest/gtest.h>

#include <algorithm>

#include "hashtable.hpp"

class HashTableTest : public ::testing::Test {
   protected:
    HashTable ht;
};

TEST_F(HashTableTest, BasicOperations) {
    EXPECT_TRUE(ht.empty());
    ht.insert("key1", "value1");
    EXPECT_EQ(ht.size(), 1);
    EXPECT_EQ(ht.get("key1"), "value1");
    EXPECT_TRUE(ht.contains("key1"));
}

TEST_F(HashTableTest, UpdateValue) {
    ht.insert("key", "v1");
    ht.insert("key", "v2");
    EXPECT_EQ(ht.size(), 1);
    EXPECT_EQ(ht.get("key"), "v2");
}

TEST_F(HashTableTest, CollisionAndRemoval) {
    ht.insert("a", "1");
    ht.insert("b", "2");
    ht.insert("c", "3");

    EXPECT_TRUE(ht.remove("b"));
    EXPECT_FALSE(ht.contains("b"));
    EXPECT_TRUE(ht.contains("a"));
    EXPECT_TRUE(ht.contains("c"));
    EXPECT_EQ(ht.size(), 2);

    EXPECT_FALSE(ht.remove("nonexistent"));
}

TEST_F(HashTableTest, PrintCheck) {
    ht.insert("1", "A");
    std::string p = ht.print();
    EXPECT_NE(p, "{}");
    EXPECT_TRUE(p.find("1: A") != std::string::npos);
}

TEST_F(HashTableTest, StressResize) {
    for (int i = 0; i < 200; ++i) {
        ht.insert("key" + std::to_string(i), "val" + std::to_string(i));
    }
    EXPECT_EQ(ht.size(), 200);
    EXPECT_EQ(ht.get("key199"), "val199");
}

TEST_F(HashTableTest, MoveLogic) {
    ht.insert("m", "v");
    HashTable moved = std::move(ht);
    EXPECT_EQ(moved.get("m"), "v");
    EXPECT_EQ(ht.size(), 0);

    HashTable assignMoved;
    assignMoved = std::move(moved);
    EXPECT_EQ(assignMoved.get("m"), "v");
}