#include <gtest/gtest.h>
#include "4/Accents.hpp"
#include "4/Array.hpp"

TEST(AccentTest, MainLogic) {
    Array<std::string> dict;
    dict.push_back("cAnnot");
    dict.push_back("cannOt");
    dict.push_back("fOund");
    dict.push_back("pAge");

    Array<std::string> text;
    text.push_back("thE");   
    text.push_back("pAge");  
    text.push_back("cAnnot"); 
    text.push_back("be");     
    text.push_back("found"); 

    EXPECT_EQ(AccentChecker::countErrors(dict, text), 2);
}