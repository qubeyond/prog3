#pragma once
#include <string>

struct DNode {
    std::string value;
    DNode* prev;
    DNode* next;
};

struct DLinkedList {
    DNode* head;
    DNode* tail;
};

void initDList(DLinkedList &list);

void addHead(DLinkedList &list, const std::string &val);
void addTail(DLinkedList &list, const std::string &val);
bool addBefore(DLinkedList &list, const std::string &target, const std::string &val);
bool addAfter(DLinkedList &list, const std::string &target, const std::string &val);

bool removeHead(DLinkedList &list);
bool removeTail(DLinkedList &list);
bool removeValue(DLinkedList &list, const std::string &val);

DNode* getNode(DLinkedList &list, int index);
DNode* findValue(DLinkedList &list, const std::string &val);

void printList(DLinkedList &list);

void readFromFile(DLinkedList &list, const std::string &filename);
void writeToFile(DLinkedList &list, const std::string &filename);

void clearList(DLinkedList &list);
