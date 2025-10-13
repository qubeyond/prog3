#pragma once
#include <string>

struct Node {
    std::string value;
    Node* next;
};

struct LinkedList {
    Node* head;
};

void initList(LinkedList &list);

void addHead(LinkedList &list, const std::string &val);
void addTail(LinkedList &list, const std::string &val);
bool addBefore(LinkedList &list, const std::string &target, const std::string &val);
bool addAfter(LinkedList &list, const std::string &target, const std::string &val);

bool removeHead(LinkedList &list);
bool removeTail(LinkedList &list);
bool removeValue(LinkedList &list, const std::string &val);

Node* getNode(LinkedList &list, int index);
Node* findValue(LinkedList &list, const std::string &val);

void printList(LinkedList &list);

void readFromFile(LinkedList &list, const std::string &filename);
void writeToFile(LinkedList &list, const std::string &filename);

void clearList(LinkedList &list);
