#pragma once
#include "Token.hpp"

struct Stack {
    Token *data;
    int size;
    int capacity;

    ~Stack()
    {
        if (data != nullptr) {
            delete[] data;
        }
    }

    Stack() : data(nullptr), size(0), capacity(0)
    {
    }
};

void initStack(Stack &stk, int capacity = 100);
void clearStack(Stack &s);

bool push(Stack &s, const Token &val);
bool pop(Stack &s, Token &val);
const Token &top(const Stack &s);
bool isEmpty(const Stack &s);