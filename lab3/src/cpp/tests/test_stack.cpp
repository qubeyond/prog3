#include <gtest/gtest.h>

#include "stack.hpp"

class StackTest : public ::testing::Test {
   protected:
    Stack s;
};

TEST_F(StackTest, LIFOOrder) {
    s.push("A");
    s.push("B");
    s.push("C");
    EXPECT_EQ(s.pop(), "C");
    EXPECT_EQ(s.pop(), "B");
    EXPECT_EQ(s.pop(), "A");
    EXPECT_TRUE(s.empty());
}

TEST_F(StackTest, CopyConsistency) {
    s.push("base");
    Stack copy = s;
    copy.push("extra");
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(copy.size(), 2);
    EXPECT_EQ(s.top(), "base");
    EXPECT_EQ(copy.top(), "extra");
}

TEST_F(StackTest, EmptyStateOperations) {
    EXPECT_EQ(s.pop(), "");
    EXPECT_EQ(s.top(), "");
    EXPECT_TRUE(s.empty());
    s.push("val");
    s.pop();
    EXPECT_TRUE(s.empty());
}

TEST_F(StackTest, PrintFormat) {
    s.push("bottom");
    s.push("top");
    EXPECT_TRUE(s.print().find("top") != std::string::npos);
    EXPECT_TRUE(s.print().find("top->bottom") != std::string::npos);
}