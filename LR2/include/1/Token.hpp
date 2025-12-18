#pragma once
#include <string>

struct Token {
    enum Type { OPERAND, OPERATOR, PARENTHESIS };
    Type type;
    std::string value;
    int precedence;
    bool is_unary;
    int logical_value;
};

void initializeTokenProperties(Token& t);