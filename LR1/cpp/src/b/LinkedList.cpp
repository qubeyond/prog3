#include "b/LinkedList.hpp"
#include <iostream>
#include <fstream>

// =======================
// Инициализация списка
// =======================
void initList(LinkedList &list) {
    list.head = nullptr;
}

// =======================
// Добавление элементов
// =======================

void addHead(LinkedList &list, int val) {
    Node* newNode = new Node{val, list.head};
    list.head = newNode;
}

void addTail(LinkedList &list, int val) {
    Node* newNode = new Node{val, nullptr};
    if (list.head == nullptr) {
        list.head = newNode;
    } else {
        Node* cur = list.head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        cur->next = newNode;
    }
}

bool addBefore(LinkedList &list, int target, int val) {
    if (list.head == nullptr) return false;
    if (list.head->value == target) {
        addHead(list, val);
        return true;
    }
    Node* cur = list.head;
    while (cur->next != nullptr && cur->next->value != target) {
        cur = cur->next;
    }
    if (cur->next == nullptr) return false;
    Node* newNode = new Node{val, cur->next};
    cur->next = newNode;
    return true;
}

bool addAfter(LinkedList &list, int target, int val) {
    Node* cur = list.head;
    while (cur != nullptr && cur->value != target) {
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    Node* newNode = new Node{val, cur->next};
    cur->next = newNode;
    return true;
}

// =======================
// Удаление элементов
// =======================

bool removeHead(LinkedList &list) {
    if (list.head == nullptr) return false;
    Node* temp = list.head;
    list.head = list.head->next;
    delete temp;
    return true;
}

bool removeTail(LinkedList &list) {
    if (list.head == nullptr) return false;
    if (list.head->next == nullptr) {
        delete list.head;
        list.head = nullptr;
        return true;
    }
    Node* cur = list.head;
    while (cur->next->next != nullptr) {
        cur = cur->next;
    }
    delete cur->next;
    cur->next = nullptr;
    return true;
}

bool removeValue(LinkedList &list, int val) {
    if (list.head == nullptr) return false;
    if (list.head->value == val) {
        return removeHead(list);
    }
    Node* cur = list.head;
    while (cur->next != nullptr && cur->next->value != val) {
        cur = cur->next;
    }
    if (cur->next == nullptr) return false;
    Node* temp = cur->next;
    cur->next = cur->next->next;
    delete temp;
    return true;
}

// =======================
// Поиск и чтение
// =======================

Node* getNode(LinkedList &list, int index) {
    Node* cur = list.head;
    int i = 0;
    while (cur != nullptr && i < index) {
        cur = cur->next;
        i++;
    }
    return (i == index ? cur : nullptr);
}

Node* findValue(LinkedList &list, int val) {
    Node* cur = list.head;
    while (cur != nullptr) {
        if (cur->value == val) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// =======================
// Вывод
// =======================

void printList(LinkedList &list) {
    Node* cur = list.head;
    while (cur != nullptr) {
        std::cout << cur->value << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

// =======================
// Работа с файлами
// =======================

void readFromFile(LinkedList &list, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) {
        addTail(list, val);
    }
    in.close();
}

void writeToFile(LinkedList &list, const std::string &filename) {
    std::ofstream out(filename);
    Node* cur = list.head;
    while (cur != nullptr) {
        out << cur->value << " ";
        cur = cur->next;
    }
    out.close();
}

// =======================
// Очистка списка
// =======================

void clearList(LinkedList &list) {
    while (list.head != nullptr) {
        removeHead(list);
    }
}
