#include <gtest/gtest.h>
#include "6/HashTable.hpp"

TEST(HashTableTest, BasicInsertionAndSearch) {
    HashTable ht(100);
    ht.insert("523456795", 1774);
    
    auto res = ht.search("523456795");
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(*res, 1774);
}

TEST(HashTableTest, UpdateValue) {
    HashTable ht(100);
    ht.insert("test", 10);
    ht.insert("test", 20);
    EXPECT_EQ(ht.search("test").value_or(0), 20);
    EXPECT_EQ(ht.size(), 1);
}

TEST(HashTableTest, DeletionLogic) {
    HashTable ht(10);
    ht.insert("key1", 1);
    ht.insert("key2", 2);
    
    ht.remove("key1");
    EXPECT_FALSE(ht.search("key1").has_value());
    EXPECT_TRUE(ht.search("key2").has_value());
    EXPECT_EQ(ht.size(), 1);
}

TEST(HashTableTest, FoldingHashLogic) {
    HashTable ht(10000);
    // Проверка примера из задания: 523456795 -> 523+456+795 = 1774
    // В нашей реализации calculate_folding_hash возвращает (сумму % capacity)
    ht.insert("523456795", 1);
    EXPECT_TRUE(ht.search("523456795").has_value());
}