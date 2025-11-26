#include "1/Stack.hpp"

#include <iostream>
#include <stdexcept>

void initializeTokenProperties(Token &token)
{
    if (token.type == Token::OPERATOR) {
        if (token.value == "!") {
            token.precedence = 3;
            token.is_unary = true;
        } else if (token.value == "&") {
            token.precedence = 2;
            token.is_unary = false;
        } else if (token.value == "|" || token.value == "^") {
            token.precedence = 1;
            token.is_unary = false;
        } else {
            token.precedence = 0;
        }
    } else {
        token.precedence = 0;
        token.is_unary = false;
    }
}

void initStack(Stack &stk, int capacity)
{
    if (stk.data != nullptr) {
        clearStack(stk);
    }
    stk.data = new Token[capacity];
    stk.size = 0;
    stk.capacity = capacity;
}

void clearStack(Stack &stk)
{
    if (stk.data != nullptr) {
        delete[] stk.data;
        stk.data = nullptr;
    }
    stk.size = 0;
    stk.capacity = 0;
}

bool push(Stack &stk, const Token &val)
{
    if (stk.size >= stk.capacity) {
        std::cerr << "ERROR: Stack overflow. Cannot push.\n";
        return false;
    }
    stk.data[stk.size++] = val;
    return true;
}

bool pop(Stack &stk, Token &val)
{
    if (stk.size == 0) {
        return false;
    }
    val = stk.data[--stk.size];
    return true;
}

const Token &top(const Stack &stk)
{
    if (stk.size == 0) {
        throw std::out_of_range("ERROR: Stack is empty on top() call.");
    }
    return stk.data[stk.size - 1];
}

bool isEmpty(const Stack &stk)
{
    return stk.size == 0;
}