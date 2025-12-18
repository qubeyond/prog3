#pragma once
#include "Token.hpp"

struct Stack {
    Token *data;
    int size;
    int capacity;

    Stack();
    ~Stack();
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
};

void initStack(Stack &stk, int capacity = 100);
void clearStack(Stack &s);
bool push(Stack &s, const Token &val);
bool pop(Stack &s, Token &val);
const Token &top(const Stack &s);
bool isEmpty(const Stack &s);