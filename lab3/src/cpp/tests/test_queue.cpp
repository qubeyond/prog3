#include <gtest/gtest.h>

#include "queue.hpp"

class QueueTest : public ::testing::Test {
   protected:
    Queue q;
};

TEST_F(QueueTest, SequenceOrder) {
    for (int i = 0; i < 100; ++i) {
        q.push(std::to_string(i));
    }
    EXPECT_EQ(q.size(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(q.pop(), std::to_string(i));
    }
    EXPECT_TRUE(q.empty());
}

TEST_F(QueueTest, CopyAssignment) {
    q.push("1");
    q.push("2");

    Queue q_copy;
    q_copy = q;

    EXPECT_EQ(q_copy.size(), 2);
    EXPECT_EQ(q_copy.pop(), "1");
    EXPECT_EQ(q.front(), "1");
}

TEST_F(QueueTest, PrintFormat) {
    q.push("A");
    q.push("B");
    std::string output = q.print();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("A"), std::string::npos);
    EXPECT_NE(output.find("front->back"), std::string::npos);
}

TEST_F(QueueTest, InterleavedPushPop) {
    q.push("1");
    EXPECT_EQ(q.pop(), "1");
    q.push("2");
    q.push("3");
    EXPECT_EQ(q.pop(), "2");
    q.push("4");
    EXPECT_EQ(q.pop(), "3");
    EXPECT_EQ(q.pop(), "4");
    EXPECT_TRUE(q.empty());
}

TEST_F(QueueTest, EmptyHandlingCheck) {
    EXPECT_EQ(q.pop(), "");
    EXPECT_EQ(q.front(), "");
    EXPECT_EQ(q.size(), 0);
}