#include <gtest/gtest.h>
#include "5/Huffman.hpp"
#include <sstream>

TEST(HuffmanTest, BasicString) {
    testing::internal::CaptureStdout();
    Huffman::compress("aaaaabbbcccc");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("a:") != std::string::npos);
    EXPECT_TRUE(output.find("b:") != std::string::npos);
    EXPECT_TRUE(output.find("c:") != std::string::npos);
}