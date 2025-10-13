#include "c/DoublyLinkedList.hpp"
#include <fstream>
#include <iostream>

void initDList(DLinkedList &list) {
    list.head = nullptr;
    list.tail = nullptr;
}

void addHead(DLinkedList &list, const std::string &val) {
    DNode* node = new DNode{val, nullptr, list.head};
    if (list.head) list.head->prev = node;
    list.head = node;
    if (!list.tail) list.tail = node;
}

void addTail(DLinkedList &list, const std::string &val) {
    DNode* node = new DNode{val, list.tail, nullptr};
    if (list.tail) list.tail->next = node;
    list.tail = node;
    if (!list.head) list.head = node;
}

bool addBefore(DLinkedList &list, const std::string &target, const std::string &val) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur) return false;
    DNode* node = new DNode{val, cur->prev, cur};
    if (cur->prev) cur->prev->next = node; else list.head = node;
    cur->prev = node;
    return true;
}

bool addAfter(DLinkedList &list, const std::string &target, const std::string &val) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur) return false;
    DNode* node = new DNode{val, cur, cur->next};
    if (cur->next) cur->next->prev = node; else list.tail = node;
    cur->next = node;
    return true;
}

bool removeHead(DLinkedList &list) {
    if (!list.head) return false;
    DNode* t = list.head;
    list.head = list.head->next;
    if (list.head) list.head->prev = nullptr; else list.tail = nullptr;
    delete t;
    return true;
}

bool removeTail(DLinkedList &list) {
    if (!list.tail) return false;
    DNode* t = list.tail;
    list.tail = list.tail->prev;
    if (list.tail) list.tail->next = nullptr; else list.head = nullptr;
    delete t;
    return true;
}

bool removeValue(DLinkedList &list, const std::string &val) {
    DNode* cur = list.head;
    while (cur && cur->value != val) cur = cur->next;
    if (!cur) return false;
    if (cur->prev) cur->prev->next = cur->next; else list.head = cur->next;
    if (cur->next) cur->next->prev = cur->prev; else list.tail = cur->prev;
    delete cur;
    return true;
}

bool removeBefore(DLinkedList &list, const std::string &target) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur || !cur->prev) return false;
    DNode* toDelete = cur->prev;
    if (toDelete->prev) toDelete->prev->next = cur; else list.head = cur;
    cur->prev = toDelete->prev;
    delete toDelete;
    return true;
}

bool removeAfter(DLinkedList &list, const std::string &target) {
    DNode* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur || !cur->next) return false;
    DNode* toDelete = cur->next;
    if (toDelete->next) toDelete->next->prev = cur; else list.tail = cur;
    cur->next = toDelete->next;
    delete toDelete;
    return true;
}

DNode* getNode(DLinkedList &list, int index) {
    DNode* cur = list.head;
    for (int i = 0; cur && i < index; ++i) cur = cur->next;
    return cur;
}

DNode* findValue(DLinkedList &list, const std::string &val) {
    DNode* cur = list.head;
    while (cur && cur->value != val) cur = cur->next;
    return cur;
}

void printList(DLinkedList &list) {
    DNode* cur = list.head;
    bool first = true;
    while (cur) {
        if (!first) std::cout << " ";
        std::cout << cur->value;
        first = false;
        cur = cur->next;
    }
    std::cout << std::endl;
}

void readFromFile(DLinkedList &list, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    clearList(list);
    std::string val;
    while (in >> val) addTail(list, val);
    in.close();
}

void writeToFile(DLinkedList &list, const std::string &filename) {
    std::ofstream out(filename);
    for (DNode* cur = list.head; cur; cur = cur->next) {
        out << cur->value;
        if (cur->next) out << " ";
    }
    out.close();
}

void clearList(DLinkedList &list) {
    while (list.head) removeHead(list);
}
