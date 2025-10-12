#pragma once
#include <string>

struct Queue {
    int* data;
    int size;
    int capacity;
    int front;
    int back;
};

// =======================
// Инициализация
// =======================
void initQueue(Queue &q, int capacity = 100);

// =======================
// Операции
// =======================
bool push(Queue &q, int val);   // добавление в конец
bool pop(Queue &q, int &val);   // удаление с начала
int peek(Queue &q);             // чтение первого элемента
int length(Queue &q);

// =======================
// Работа с файлами
// =======================
void readFromFile(Queue &q, const std::string &filename);
void writeToFile(Queue &q, const std::string &filename);

// =======================
// Очистка
// =======================
void clearQueue(Queue &q);
