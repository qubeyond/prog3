#include <gtest/gtest.h>
#include "singly_list.hpp"

class SinglyListTest : public ::testing::Test {
protected:
    SinglyList list;

    void clear() {
        while(list.getSize() > 0) {
            list.del_head();
        }
    }
};

TEST_F(SinglyListTest, BasicOps) {
    EXPECT_EQ(list.getSize(), 0);
    list.push_head("2");
    list.push_tail("3");
    list.push_head("1");
    EXPECT_EQ(list.print(), "[1 -> 2 -> 3]");
    EXPECT_TRUE(list.find("2"));
    EXPECT_EQ(list.get(1), "2");
    EXPECT_EQ(list.get(100), "");
}



TEST_F(SinglyListTest, Insertions) {
    list.push_head("B");
    EXPECT_TRUE(list.insert_before_value("B", "A"));
    EXPECT_TRUE(list.insert_after_value("B", "C"));
    EXPECT_TRUE(list.insert_after_value("C", "D"));
    EXPECT_FALSE(list.insert_before_value("Z", "err"));
    EXPECT_FALSE(list.insert_after_value("Z", "err"));

    EXPECT_TRUE(list.insert_before_index(0, "0"));
    EXPECT_TRUE(list.insert_after_index(list.getSize() - 1, "E"));
    EXPECT_TRUE(list.insert_before_index(2, "mid"));
    EXPECT_FALSE(list.insert_before_index(99, "err"));
    EXPECT_FALSE(list.insert_after_index(99, "err"));
}

TEST_F(SinglyListTest, Deletions) {
    EXPECT_FALSE(list.del_head());
    EXPECT_FALSE(list.del_tail());
    
    list.push_head("only");
    EXPECT_TRUE(list.del_tail());
    EXPECT_EQ(list.getSize(), 0);
    
    for (int i = 0; i < 5; ++i) list.push_tail(std::to_string(i));
    EXPECT_TRUE(list.del_value("2"));
    EXPECT_TRUE(list.del_value("0"));
    EXPECT_TRUE(list.del_value("4"));
    EXPECT_FALSE(list.del_value("99"));
}



TEST_F(SinglyListTest, RelativeDeletions) {
    clear();
    list.push_tail("0"); list.push_tail("1"); list.push_tail("2");
    EXPECT_TRUE(list.del_before_value("1")); 
    EXPECT_EQ(list.get(0), "1");
    
    list.push_tail("3");
    EXPECT_TRUE(list.del_before_value("3")); 
    EXPECT_FALSE(list.del_before_value("1")); 
    
    clear();
    list.push_tail("A"); list.push_tail("B"); list.push_tail("C");
    EXPECT_TRUE(list.del_after_value("A")); 
    EXPECT_TRUE(list.del_after_value("A")); 
    EXPECT_FALSE(list.del_after_value("A")); 
    EXPECT_FALSE(list.del_after_value("Z")); 

    clear();
    for(int i=0; i<4; ++i) list.push_tail(std::to_string(i));
    EXPECT_TRUE(list.del_before_index(1)); 
    EXPECT_TRUE(list.del_before_index(2)); 
    EXPECT_FALSE(list.del_before_index(0));

    clear();
    for(int i=0; i<4; ++i) list.push_tail(std::to_string(i));
    EXPECT_TRUE(list.del_after_index(0)); 
    EXPECT_TRUE(list.del_after_index(list.getSize() - 2)); 
    EXPECT_FALSE(list.del_after_index(list.getSize() - 1));
}

TEST_F(SinglyListTest, RuleOfFive) {
    clear();
    list.push_tail("A");
    list.push_tail("B");

    SinglyList copy = list;
    EXPECT_EQ(copy.getSize(), 2);
    
    SinglyList assign;
    assign = list;
    assign = *&assign;
    EXPECT_EQ(assign.getSize(), 2);

    SinglyList moved(std::move(list));
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(list.getSize(), 0);

    SinglyList move_assign;
    move_assign = std::move(moved);
    move_assign = std::move(*&move_assign);
    EXPECT_EQ(move_assign.getSize(), 2);
}