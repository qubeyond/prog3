#include <gtest/gtest.h>

#include "array.hpp"

class ArrayTest : public ::testing::Test {
   protected:
    Array arr;
};

TEST_F(ArrayTest, CapacityGrowth) {
    for (int i = 0; i < 20; ++i) arr.push(std::to_string(i));
    EXPECT_EQ(arr.getSize(), 20);
    EXPECT_GE(arr.getCapacity(), 20);
}

TEST_F(ArrayTest, InsertRemove) {
    arr.push("A");
    arr.push("C");
    arr.insert(1, "B");
    EXPECT_EQ(arr.print(), "[A, B, C]");
    EXPECT_TRUE(arr.remove(1));
    EXPECT_EQ(arr.get(1), "C");
}

TEST_F(ArrayTest, Shrink) {
    for (int i = 0; i < 32; ++i) arr.push("x");
    size_t bigCap = arr.getCapacity();
    for (int i = 0; i < 30; ++i) arr.remove(0);
    EXPECT_LT(arr.getCapacity(), bigCap);
}

TEST_F(ArrayTest, Exceptions) {
    EXPECT_THROW(arr.get(0), std::out_of_range);
    EXPECT_THROW(arr.set(0, "val"), std::out_of_range);
    arr.push("ok");
    arr.set(0, "new");
    EXPECT_EQ(arr.get(0), "new");
}

TEST_F(ArrayTest, Assignments) {
    arr.push("1");
    Array a2 = arr;
    Array a3;
    a3 = arr;
    EXPECT_EQ(a2.get(0), "1");
    EXPECT_EQ(a3.get(0), "1");
}