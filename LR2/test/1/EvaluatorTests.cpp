#include <stdexcept>

#include "1/Evaluator.hpp"
#include "1/Stack.hpp"
#include "gtest/gtest.h"

// Базовые операции стека
TEST(StackTest, BasicOperations)
{
    Stack s;
    initStack(s, 5);
    Token t1 = {Token::OPERAND, "1", 0, false, 1};
    Token t2 = {Token::OPERATOR, "&", 2, false, 0};
    Token t3;

    EXPECT_TRUE(isEmpty(s));

    // Push t1
    EXPECT_TRUE(push(s, t1));
    EXPECT_FALSE(isEmpty(s));
    EXPECT_EQ(top(s).value, "1");

    // Push t2
    EXPECT_TRUE(push(s, t2));
    EXPECT_EQ(s.size, 2);
    EXPECT_EQ(top(s).value, "&");

    // Pop t2
    EXPECT_TRUE(pop(s, t3));
    EXPECT_EQ(t3.value, "&");
    EXPECT_EQ(s.size, 1);

    // Pop t1
    EXPECT_TRUE(pop(s, t3));
    EXPECT_EQ(t3.value, "1");
    EXPECT_TRUE(isEmpty(s));

    EXPECT_THROW({ top(s); }, std::out_of_range);

    clearStack(s);
}

// Основные бинарные операции
TEST(EvaluatorTest, BasicBinaryOperators)
{
    // 1 & 0 = 0
    EXPECT_EQ(evaluateExpression("1 & 0"), 0);
    // 1 | 0 = 1
    EXPECT_EQ(evaluateExpression("1 | 0"), 1);
    // 1 ^ 0 = 1
    EXPECT_EQ(evaluateExpression("1 ^ 0"), 1);
    // 0 & 0 = 0
    EXPECT_EQ(evaluateExpression("0 & 0"), 0);
}

// Унарные операции
TEST(EvaluatorTest, UnaryNOT)
{
    // ! 1 = 0
    EXPECT_EQ(evaluateExpression("! 1"), 0);
    // ! 0 = 1
    EXPECT_EQ(evaluateExpression("! 0"), 1);
    // ! ( ! 1 ) = 1
    EXPECT_EQ(evaluateExpression("! ( ! 1 )"), 1);
}

// Приоритет
TEST(EvaluatorTest, OperatorPrecedence)
{
    // (1 & 0) | 1 = 0 | 1 = 1
    EXPECT_EQ(evaluateExpression("1 & 0 | 1"), 1);
    // (! 0) & 0 = 1 & 0 = 0
    EXPECT_EQ(evaluateExpression("! 0 & 0"), 0);
}

// Скобки
TEST(EvaluatorTest, ParenthesesGrouping)
{
    // 1 & ( 0 | 1 ) = 1 & 1 = 1
    EXPECT_EQ(evaluateExpression("1 & ( 0 | 1 )"), 1);
    // ( 1 & 0 ) | 1 = 0 | 1 = 1
    EXPECT_EQ(evaluateExpression("( 1 & 0 ) | 1"), 1);
    // 1 ^ ( 1 & 0 ) = 1 ^ 0 = 1
    EXPECT_EQ(evaluateExpression("1 ^ ( 1 & 0 )"), 1);
}

// Сложные выражения
TEST(EvaluatorTest, ComplexExpression)
{
    // ( ! 1 & 0 ) | ( 1 ^ ! 0 ) = ( 0 & 0 ) | ( 1 ^ 1 ) = 0 | 0 = 0
    EXPECT_EQ(evaluateExpression("( ! 1 & 0 ) | ( 1 ^ ! 0 )"), 0);
}

// Некорректные выражения
TEST(EvaluatorTest, InvalidExpressions)
{
    EXPECT_EQ(evaluateExpression("( 1 & 0 "), -1);
    EXPECT_EQ(evaluateExpression("1 &"), -1);
}