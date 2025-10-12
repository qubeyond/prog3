#include "a/Array.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

// =======================
// Инициализация
// =======================
void initArray(Array &arr, int initialCapacity) {
    arr.data = new int[initialCapacity];
    arr.size = 0;
    arr.capacity = initialCapacity;
}

// =======================
// Вспомогательная функция увеличения
// =======================
void ensureCapacity(Array &arr) {
    if (arr.size >= arr.capacity) {
        arr.capacity *= 2;
        int* newData = new int[arr.capacity];
        std::copy(arr.data, arr.data + arr.size, newData);
        delete[] arr.data;
        arr.data = newData;
    }
}

// =======================
// Добавление
// =======================
void addEnd(Array &arr, int val) {
    ensureCapacity(arr);
    arr.data[arr.size++] = val;
}

bool addAt(Array &arr, int index, int val) {
    if (index < 0 || index > arr.size) return false;
    ensureCapacity(arr);
    for (int i = arr.size; i > index; i--) {
        arr.data[i] = arr.data[i-1];
    }
    arr.data[index] = val;
    arr.size++;
    return true;
}

// =======================
// Получение и замена
// =======================
int get(Array &arr, int index) {
    if (index < 0 || index >= arr.size) throw std::out_of_range("Index out of range");
    return arr.data[index];
}

bool set(Array &arr, int index, int val) {
    if (index < 0 || index >= arr.size) return false;
    arr.data[index] = val;
    return true;
}

// =======================
// Удаление
// =======================
bool removeAt(Array &arr, int index) {
    if (index < 0 || index >= arr.size) return false;
    for (int i = index; i < arr.size - 1; i++) {
        arr.data[i] = arr.data[i+1];
    }
    arr.size--;
    return true;
}

// =======================
// Размер
// =======================
int length(Array &arr) {
    return arr.size;
}

// =======================
// Работа с файлами
// =======================
void readFromFile(Array &arr, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) {
        addEnd(arr, val);
    }
    in.close();
}

void writeToFile(Array &arr, const std::string &filename) {
    std::ofstream out(filename);
    for (int i = 0; i < arr.size; i++) {
        out << arr.data[i] << " ";
    }
    out.close();
}

// =======================
// Очистка массива
// =======================
void clearArray(Array &arr) {
    delete[] arr.data;
    arr.data = nullptr;
    arr.size = 0;
    arr.capacity = 0;
}
