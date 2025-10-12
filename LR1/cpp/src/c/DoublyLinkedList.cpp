#include "c/DoublyLinkedList.hpp"
#include <iostream>
#include <fstream>

// =======================
// Инициализация
// =======================
void initDList(DLinkedList &list) {
    list.head = nullptr;
    list.tail = nullptr;
}

// =======================
// Добавление элементов
// =======================
void addHead(DLinkedList &list, int val) {
    DNode* node = new DNode{val, nullptr, list.head};
    if (list.head) list.head->prev = node;
    list.head = node;
    if (!list.tail) list.tail = node;
}

void addTail(DLinkedList &list, int val) {
    DNode* node = new DNode{val, list.tail, nullptr};
    if (list.tail) list.tail->next = node;
    list.tail = node;
    if (!list.head) list.head = node;
}

bool addBefore(DLinkedList &list, int target, int val) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur) return false;

    DNode* node = new DNode{val, cur->prev, cur};
    if (cur->prev) cur->prev->next = node;
    cur->prev = node;
    if (cur == list.head) list.head = node;
    return true;
}

bool addAfter(DLinkedList &list, int target, int val) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur) return false;

    DNode* node = new DNode{val, cur, cur->next};
    if (cur->next) cur->next->prev = node;
    cur->next = node;
    if (cur == list.tail) list.tail = node;
    return true;
}

// =======================
// Удаление
// =======================
bool removeHead(DLinkedList &list) {
    if (!list.head) return false;
    DNode* temp = list.head;
    list.head = list.head->next;
    if (list.head) list.head->prev = nullptr;
    else list.tail = nullptr;
    delete temp;
    return true;
}

bool removeTail(DLinkedList &list) {
    if (!list.tail) return false;
    DNode* temp = list.tail;
    list.tail = list.tail->prev;
    if (list.tail) list.tail->next = nullptr;
    else list.head = nullptr;
    delete temp;
    return true;
}

bool removeValue(DLinkedList &list, int val) {
    DNode* cur = list.head;
    while (cur && cur->value != val) cur = cur->next;
    if (!cur) return false;

    if (cur->prev) cur->prev->next = cur->next;
    else list.head = cur->next;

    if (cur->next) cur->next->prev = cur->prev;
    else list.tail = cur->prev;

    delete cur;
    return true;
}

// =======================
// Поиск и чтение
// =======================
DNode* getNode(DLinkedList &list, int index) {
    DNode* cur = list.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; i++; }
    return (i == index ? cur : nullptr);
}

DNode* findValue(DLinkedList &list, int val) {
    DNode* cur = list.head;
    while (cur) {
        if (cur->value == val) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// =======================
// Вывод
// =======================
void printList(DLinkedList &list) {
    DNode* cur = list.head;
    while (cur) {
        std::cout << cur->value << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

// =======================
// Работа с файлами
// =======================
void readFromFile(DLinkedList &list, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) addTail(list, val);
    in.close();
}

void writeToFile(DLinkedList &list, const std::string &filename) {
    std::ofstream out(filename);
    DNode* cur = list.head;
    while (cur) {
        out << cur->value << " ";
        cur = cur->next;
    }
    out.close();
}

// =======================
// Очистка списка
// =======================
void clearList(DLinkedList &list) {
    while (list.head) removeHead(list);
}
