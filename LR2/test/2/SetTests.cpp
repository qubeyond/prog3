#include <gtest/gtest.h>
#include "2/Set.hpp"

TEST(SetTest, BasicOperations) {
    Set s(100);
    s.add("element1");
    EXPECT_TRUE(s.contains("element1"));
    EXPECT_EQ(s.size(), 1);

    s.del("element1");
    EXPECT_FALSE(s.contains("element1"));
    EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, DuplicateAddition) {
    Set s(100);
    s.add("item");
    s.add("item");
    EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, NonExistentDeletion) {
    Set s(10);
    s.del("nothing");
    EXPECT_EQ(s.size(), 0);
}