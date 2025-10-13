#pragma once
#include <string>

struct Stack {
    std::string* data;
    int size;
    int capacity;
};

void initStack(Stack &s, int capacity = 100);

bool push(Stack &s, const std::string &val);
bool pop(Stack &s, std::string &val);

std::string top(Stack &s);
int length(Stack &s);

void readFromFile(Stack &s, const std::string &filename);
void writeToFile(Stack &s, const std::string &filename);

void clearStack(Stack &s);
