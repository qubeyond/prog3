#pragma once
#include <string>

// Элемент лог выражения
struct Token {
    enum Type { OPERAND, OPERATOR, PARENTHESIS };

    Type type;
    std::string value;

    // Свойства
    int precedence;
    bool is_unary;

    // Значение
    int logical_value;
};

void initializeTokenProperties(Token& t);