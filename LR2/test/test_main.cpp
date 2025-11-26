#include "gtest/gtest.h"

// Этот тест будет обнаружен командой gtest_discover_tests
TEST(SmokeTest, BasicAssertion)
{
    // Простейшая проверка, чтобы CTest обнаружил хотя бы один тест
    ASSERT_EQ(1 + 1, 2);
}
