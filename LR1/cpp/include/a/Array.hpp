#pragma once
#include <string>

struct Array {
    int* data;
    int size;
    int capacity;
};

// =======================
// Инициализация массива
// =======================
void initArray(Array &arr, int initialCapacity = 10);

// =======================
// Добавление элементов
// =======================

// В конец массива
void addEnd(Array &arr, int val);

// По индексу (сдвиг элементов вправо)
bool addAt(Array &arr, int index, int val);

// =======================
// Получение и замена
// =======================
int get(Array &arr, int index);      // получить элемент по индексу
bool set(Array &arr, int index, int val); // заменить элемент по индексу

// =======================
// Удаление
// =======================
bool removeAt(Array &arr, int index); // удалить элемент по индексу

// =======================
// Размер
// =======================
int length(Array &arr);

// =======================
// Работа с файлами
// =======================
void readFromFile(Array &arr, const std::string &filename);
void writeToFile(Array &arr, const std::string &filename);

// =======================
// Очистка массива
// =======================
void clearArray(Array &arr);
