#pragma once

#include <string>

struct Node {
    int value;      // значение элемента
    Node* next;     // указатель на следующий элемент
};

struct LinkedList {
    Node* head;     // указатель на голову списка
};

// =======================
// Инициализация списка
// =======================
void initList(LinkedList &list);

// =======================
// Добавление элементов
// =======================

// В голову
void addHead(LinkedList &list, int val);

// В хвост
void addTail(LinkedList &list, int val);

// Перед заданным значением
bool addBefore(LinkedList &list, int target, int val);

// После заданного значения
bool addAfter(LinkedList &list, int target, int val);

// =======================
// Удаление элементов
// =======================

// Удаление головы
bool removeHead(LinkedList &list);

// Удаление хвоста
bool removeTail(LinkedList &list);

// Удаление по значению
bool removeValue(LinkedList &list, int val);

// =======================
// Поиск и чтение
// =======================

// Получение узла по индексу
Node* getNode(LinkedList &list, int index);

// Поиск узла по значению
Node* findValue(LinkedList &list, int val);

// =======================
// Вывод
// =======================

// Печать списка на экран
void printList(LinkedList &list);

// =======================
// Работа с файлами
// =======================

// Чтение из файла (по одному числу через пробел)
void readFromFile(LinkedList &list, const std::string &filename);

// Запись в файл
void writeToFile(LinkedList &list, const std::string &filename);

// =======================
// Очистка списка
// =======================

// Удаление всех элементов
void clearList(LinkedList &list);
