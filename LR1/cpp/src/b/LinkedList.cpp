#include "b/LinkedList.hpp"
#include <iostream>
#include <fstream>

void initList(LinkedList &list) {
    list.head = nullptr;
}

void addHead(LinkedList &list, const std::string &val) {
    Node* node = new Node{val, list.head};
    list.head = node;
}

void addTail(LinkedList &list, const std::string &val) {
    Node* node = new Node{val, nullptr};
    if (!list.head) list.head = node;
    else {
        Node* cur = list.head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
}

bool addBefore(LinkedList &list, const std::string &target, const std::string &val) {
    if (!list.head) return false;
    if (list.head->value == target) {
        addHead(list, val);
        return true;
    }
    Node* cur = list.head;
    while (cur->next && cur->next->value != target) cur = cur->next;
    if (!cur->next) return false;
    Node* node = new Node{val, cur->next};
    cur->next = node;
    return true;
}

bool addAfter(LinkedList &list, const std::string &target, const std::string &val) {
    Node* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur) return false;
    Node* node = new Node{val, cur->next};
    cur->next = node;
    return true;
}

bool removeHead(LinkedList &list) {
    if (!list.head) return false;
    Node* tmp = list.head;
    list.head = list.head->next;
    delete tmp;
    return true;
}

bool removeTail(LinkedList &list) {
    if (!list.head) return false;
    if (!list.head->next) {
        delete list.head;
        list.head = nullptr;
        return true;
    }
    Node* cur = list.head;
    while (cur->next->next) cur = cur->next;
    delete cur->next;
    cur->next = nullptr;
    return true;
}

bool removeValue(LinkedList &list, const std::string &val) {
    if (!list.head) return false;
    if (list.head->value == val) return removeHead(list);
    Node* cur = list.head;
    while (cur->next && cur->next->value != val) cur = cur->next;
    if (!cur->next) return false;
    Node* tmp = cur->next;
    cur->next = cur->next->next;
    delete tmp;
    return true;
}

bool removeBefore(LinkedList &list, const std::string &target) {
    if (!list.head || list.head->value == target) return false;
    if (list.head->next && list.head->next->value == target) return removeHead(list);
    Node* cur = list.head;
    while (cur->next && cur->next->next && cur->next->next->value != target) cur = cur->next;
    if (!cur->next || !cur->next->next) return false;
    Node* tmp = cur->next;
    cur->next = cur->next->next;
    delete tmp;
    return true;
}

bool removeAfter(LinkedList &list, const std::string &target) {
    Node* cur = list.head;
    while (cur && cur->value != target) cur = cur->next;
    if (!cur || !cur->next) return false;
    Node* tmp = cur->next;
    cur->next = cur->next->next;
    delete tmp;
    return true;
}

Node* getNode(LinkedList &list, int index) {
    Node* cur = list.head;
    int i = 0;
    while (cur && i < index) {
        cur = cur->next;
        i++;
    }
    return (i == index ? cur : nullptr);
}

Node* findValue(LinkedList &list, const std::string &val) {
    Node* cur = list.head;
    while (cur) {
        if (cur->value == val) return cur;
        cur = cur->next;
    }
    return nullptr;
}

void printList(LinkedList &list) {
    Node* cur = list.head;
    while (cur) {
        std::cout << cur->value;
        if (cur->next) std::cout << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

void readFromFile(LinkedList &list, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    std::string val;
    while (in >> val) addTail(list, val);
    in.close();
}

void writeToFile(LinkedList &list, const std::string &filename) {
    std::ofstream out(filename);
    Node* cur = list.head;
    while (cur) {
        out << cur->value;
        if (cur->next) out << " ";
        cur = cur->next;
    }
    out.close();
}

void clearList(LinkedList &list) {
    while (list.head) removeHead(list);
}
