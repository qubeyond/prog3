#include <gtest/gtest.h>

#include "cbtree.hpp"

class CBTTest : public ::testing::Test {
   protected:
    CompleteBinaryTree tree;
};

TEST_F(CBTTest, InsertionOrder) {
    tree.insert("1");
    tree.insert("2");
    tree.insert("3");
    tree.insert("4");

    auto vec = tree.toVector();
    ASSERT_EQ(vec.size(), 4);
    EXPECT_EQ(vec[0], "1");  // Root
    EXPECT_EQ(vec[1], "2");  // Left of 1
    EXPECT_EQ(vec[2], "3");  // Right of 1
    EXPECT_EQ(vec[3], "4");  // Left of 2
}

TEST_F(CBTTest, SearchAndClear) {
    EXPECT_FALSE(tree.search("any"));
    tree.insert("apple");
    tree.insert("banana");
    EXPECT_TRUE(tree.search("apple"));
    EXPECT_TRUE(tree.search("banana"));

    tree.clear();
    EXPECT_EQ(tree.toVector().size(), 0);
    EXPECT_FALSE(tree.search("apple"));
}

TEST_F(CBTTest, LargeTree) {
    for (int i = 0; i < 15; ++i) {
        tree.insert(std::to_string(i));
    }
    EXPECT_EQ(tree.toVector().size(), 15);
    EXPECT_TRUE(tree.search("14"));
}

TEST_F(CBTTest, PrintNotEmpty) {
    tree.insert("root");
    std::string output = tree.print();
    EXPECT_NE(output.find("root"), std::string::npos);
}