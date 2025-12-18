#include "1/Stack.hpp"
#include <stdexcept>

Stack::Stack() : data(nullptr), size(0), capacity(0) {}
Stack::~Stack() { if (data) delete[] data; }

void initStack(Stack &stk, int capacity) {
    stk.data = new Token[capacity];
    stk.capacity = capacity;
    stk.size = 0;
}

void clearStack(Stack &stk) {
    stk.size = 0;
}

bool push(Stack &stk, const Token &val) {
    if (stk.size >= stk.capacity) return false;
    stk.data[stk.size++] = val;
    return true;
}

bool pop(Stack &stk, Token &val) {
    if (stk.size == 0) return false;
    val = stk.data[--stk.size];
    return true;
}

const Token &top(const Stack &stk) {
    if (stk.size == 0) throw std::out_of_range("Empty");
    return stk.data[stk.size - 1];
}

bool isEmpty(const Stack &stk) { return stk.size == 0; }

void initializeTokenProperties(Token &token) {
    if (token.type == Token::OPERATOR) {
        if (token.value == "!") { token.precedence = 3; token.is_unary = true; }
        else if (token.value == "&") { token.precedence = 2; token.is_unary = false; }
        else if (token.value == "|" || token.value == "^") { token.precedence = 1; token.is_unary = false; }
    }
}