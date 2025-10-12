#pragma once
#include <string>

struct Stack {
    int* data;
    int size;
    int capacity;
};

// =======================
// Инициализация
// =======================
void initStack(Stack &s, int capacity = 100);

// =======================
// Операции
// =======================
bool push(Stack &s, int val);
bool pop(Stack &s, int &val);
int top(Stack &s);
int length(Stack &s);

// =======================
// Работа с файлами
// =======================
void readFromFile(Stack &s, const std::string &filename);
void writeToFile(Stack &s, const std::string &filename);

// =======================
// Очистка
// =======================
void clearStack(Stack &s);
