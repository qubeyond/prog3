#include <gtest/gtest.h>
#include "1/Evaluator.hpp"

TEST(Task1, Basic) {
    EXPECT_EQ(evaluateExpression("1&0"), 0);
    EXPECT_EQ(evaluateExpression("1|0"), 1);
    EXPECT_EQ(evaluateExpression("1^1"), 0);
    EXPECT_EQ(evaluateExpression("!0"), 1);
}

TEST(Task1, Precedence) {
    EXPECT_EQ(evaluateExpression("1|0&0"), 1); 
    EXPECT_EQ(evaluateExpression("!0|0"), 1);
}

TEST(Task1, Parentheses) {
    EXPECT_EQ(evaluateExpression("(1|0)&0"), 0);
    EXPECT_EQ(evaluateExpression("!(1&0)"), 1);
}

TEST(Task1, NoSpaces) {
    EXPECT_EQ(evaluateExpression("(1&!0)^1"), 0);
}