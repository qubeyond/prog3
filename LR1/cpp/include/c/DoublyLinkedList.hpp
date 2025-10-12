#pragma once
#include <string>

struct DNode {
    int value;
    DNode* prev;
    DNode* next;
};

struct DLinkedList {
    DNode* head;
    DNode* tail;
};

// =======================
// Инициализация
// =======================
void initDList(DLinkedList &list);

// =======================
// Добавление элементов
// =======================
void addHead(DLinkedList &list, int val);
void addTail(DLinkedList &list, int val);
bool addBefore(DLinkedList &list, int target, int val);
bool addAfter(DLinkedList &list, int target, int val);

// =======================
// Удаление элементов
// =======================
bool removeHead(DLinkedList &list);
bool removeTail(DLinkedList &list);
bool removeValue(DLinkedList &list, int val);

// =======================
// Поиск и чтение
// =======================
DNode* getNode(DLinkedList &list, int index);
DNode* findValue(DLinkedList &list, int val);

// =======================
// Вывод
// =======================
void printList(DLinkedList &list);

// =======================
// Работа с файлами
// =======================
void readFromFile(DLinkedList &list, const std::string &filename);
void writeToFile(DLinkedList &list, const std::string &filename);

// =======================
// Очистка списка
// =======================
void clearList(DLinkedList &list);
