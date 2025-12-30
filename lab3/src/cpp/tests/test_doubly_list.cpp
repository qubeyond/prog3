#include <gtest/gtest.h>

#include "doubly_list.hpp"

class DoublyListTest : public ::testing::Test {
   protected:
    DoublyList list;
};

TEST_F(DoublyListTest, BasicAndSearch) {
    EXPECT_EQ(list.getSize(), 0);
    EXPECT_EQ(list.print(), "[]");
    list.push_head("10");
    list.push_tail("20");
    EXPECT_TRUE(list.find("10"));
    EXPECT_FALSE(list.find("99"));
    EXPECT_EQ(list.get(0), "10");
    EXPECT_EQ(list.get(1), "20");
    EXPECT_EQ(list.get(100), "");
}

TEST_F(DoublyListTest, GetSetOptimization) {
    for (int i = 0; i < 10; ++i) list.push_tail(std::to_string(i));
    EXPECT_EQ(list.get(2), "2");
    EXPECT_EQ(list.get(8), "8");
    list.set(1, "one");
    list.set(9, "nine");
    EXPECT_EQ(list.get(1), "one");
    EXPECT_EQ(list.get(9), "nine");
    EXPECT_THROW(list.set(100, "err"), std::out_of_range);
}

TEST_F(DoublyListTest, InsertionsComprehensive) {
    list.push_head("B");
    EXPECT_TRUE(list.insert_before_value("B", "A"));
    EXPECT_TRUE(list.insert_after_value("B", "C"));
    list.push_head("X");
    EXPECT_TRUE(list.insert_before_value("B", "mid1"));
    EXPECT_TRUE(list.insert_after_value("B", "mid2"));
    EXPECT_TRUE(list.insert_before_index(0, "START"));
    EXPECT_TRUE(list.insert_after_index(list.getSize() - 1, "END"));
    EXPECT_TRUE(list.insert_before_index(2, "MID_IDX"));
    EXPECT_TRUE(list.insert_after_index(2, "MID_IDX2"));
    EXPECT_FALSE(list.insert_before_value("non-existent", "err"));
    EXPECT_FALSE(list.insert_after_value("non-existent", "err"));
    EXPECT_FALSE(list.insert_before_index(99, "err"));
    EXPECT_FALSE(list.insert_after_index(99, "err"));
}

TEST_F(DoublyListTest, DeletionsDetailed) {
    for (int i = 0; i < 5; ++i) list.push_tail(std::to_string(i));
    EXPECT_TRUE(list.del_value("0"));
    EXPECT_TRUE(list.del_value("4"));
    EXPECT_TRUE(list.del_value("2"));
    list.push_head("A");
    EXPECT_TRUE(list.del_before_value("1"));
    EXPECT_TRUE(list.del_after_value("1"));
    list.push_head("X");
    list.push_tail("Y");
    EXPECT_FALSE(list.del_before_value("X"));
    EXPECT_FALSE(list.del_after_value("Y"));
    list.push_tail("Z");
    EXPECT_TRUE(list.del_before_index(1));
    EXPECT_TRUE(list.del_after_index(list.getSize() - 2));
    list.push_head("1");
    list.push_head("2");
    list.push_head("3");
    EXPECT_TRUE(list.del_before_index(2));
    EXPECT_TRUE(list.del_after_index(1));
    EXPECT_FALSE(list.del_before_index(0));
    EXPECT_FALSE(list.del_after_index(list.getSize() - 1));
}

TEST_F(DoublyListTest, RuleOfFiveAndSelf) {
    list.push_tail("data");
    DoublyList copy;
    copy = list;
    copy = *&copy;
    EXPECT_EQ(copy.getSize(), 1);
    DoublyList moved;
    moved = std::move(list);
    moved = std::move(*&moved);
    EXPECT_EQ(moved.getSize(), 1);
    EXPECT_EQ(list.getSize(), 0);
    DoublyList empty_list;
    moved = empty_list;
    EXPECT_EQ(moved.getSize(), 0);
}

TEST_F(DoublyListTest, EmptyStateFailures) {
    EXPECT_FALSE(list.del_head());
    EXPECT_FALSE(list.del_tail());
    EXPECT_FALSE(list.del_value("any"));
    EXPECT_FALSE(list.del_before_index(1));
    EXPECT_FALSE(list.del_after_index(1));
}