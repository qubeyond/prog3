#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include "b/LinkedList.hpp"

// -------------------- Цвета для консоли --------------------
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

// -------------------- Отладочный вывод списка --------------------
void printListDebug(LinkedList &list, const std::string &label) {
    std::cout << YELLOW << label << RESET << ": [";
    Node* cur = list.head;
    while (cur) {
        std::cout << cur->value;
        if (cur->next) std::cout << ", ";
        cur = cur->next;
    }
    std::cout << "]\n";
}

// -------------------- Базовые тесты --------------------
void testInit() {
    LinkedList list;
    initList(list);
    assert(list.head == nullptr);
}

void testAddHead() {
    LinkedList list;
    initList(list);
    addHead(list, 10);
    assert(list.head->value == 10);
    addHead(list, 20);
    assert(list.head->value == 20 && list.head->next->value == 10);
}

void testAddTail() {
    LinkedList list;
    initList(list);
    addTail(list, 5);
    assert(list.head->value == 5);
    addTail(list, 15);
    assert(list.head->next->value == 15);
}

void testAddBefore() {
    LinkedList list;
    initList(list);
    addHead(list, 10);
    addHead(list, 20); // [20,10]
    bool added = addBefore(list, 10, 15); // [20,15,10]
    assert(added && list.head->next->value == 15);
    added = addBefore(list, 20, 5); // [5,20,15,10]
    assert(added && list.head->value == 5);
    added = addBefore(list, 999, 50);
    assert(!added);
}

void testAddAfter() {
    LinkedList list;
    initList(list);
    addHead(list, 10);
    addHead(list, 20); // [20,10]
    bool added = addAfter(list, 10, 15); // [20,10,15]
    assert(added && list.head->next->next->value == 15);
    added = addAfter(list, 999, 50);
    assert(!added);
}

void testRemoveHead() {
    LinkedList list;
    initList(list);
    assert(!removeHead(list));
    addHead(list, 10);
    assert(removeHead(list) && list.head == nullptr);
    addHead(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    assert(removeHead(list) && list.head->value == 2);
    clearList(list);
}

void testRemoveTail() {
    LinkedList list;
    initList(list);
    assert(!removeTail(list));
    addHead(list, 10);
    assert(removeTail(list) && list.head == nullptr);
    addHead(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    assert(removeTail(list));
    Node* cur = list.head;
    while (cur->next) cur = cur->next;
    assert(cur->value == 2);
    clearList(list);
}

void testRemoveValue() {
    LinkedList list;
    initList(list);
    assert(!removeValue(list, 10));
    addHead(list, 10);
    assert(removeValue(list, 10) && list.head == nullptr);
    addHead(list, 20);
    assert(!removeValue(list, 999));
    addHead(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    assert(removeValue(list, 2));
    assert(removeValue(list, 1));
    assert(removeValue(list, 20));
    assert(!removeValue(list, 999));
    clearList(list);
}

// -------------------- Расширенные тесты --------------------
void testGetNode() {
    LinkedList list;
    initList(list);
    addTail(list, 10);
    addTail(list, 20);
    addTail(list, 30); // [10,20,30]
    Node* n0 = getNode(list, 0);
    Node* n1 = getNode(list, 1);
    Node* n2 = getNode(list, 2);
    Node* n3 = getNode(list, 3); // вне диапазона
    assert(n0 && n0->value == 10);
    assert(n1 && n1->value == 20);
    assert(n2 && n2->value == 30);
    assert(n3 == nullptr);
    clearList(list);
}

void testFindValue() {
    LinkedList list;
    initList(list);
    addTail(list, 5);
    addTail(list, 15);
    addTail(list, 25);
    Node* n1 = findValue(list, 15);
    Node* n2 = findValue(list, 999);
    assert(n1 && n1->value == 15);
    assert(n2 == nullptr);
    clearList(list);
}

void testPrintList() {
    LinkedList list;
    initList(list);
    addTail(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printList(list); // должен вывести "1 2 3\n"
    std::cout.rdbuf(old);
    std::string output = buffer.str();
    assert(output == "1 2 3 \n");
    clearList(list);
}

void testFileIO() {
    LinkedList list;
    initList(list);
    addTail(list, 100);
    addTail(list, 200);
    addTail(list, 300);
    writeToFile(list, "test.txt");
    clearList(list);
    readFromFile(list, "test.txt");
    Node* cur = list.head;
    assert(cur && cur->value == 100);
    assert(cur->next && cur->next->value == 200);
    assert(cur->next->next && cur->next->next->value == 300);
    clearList(list);
    std::remove("test.txt");
}

void testClearList() {
    LinkedList list;
    initList(list);
    addTail(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    clearList(list);
    assert(list.head == nullptr);
}

void testStress() {
    LinkedList list;
    initList(list);

    const int N = 10000;
    for (int i = 0; i < N; i++) {
        addTail(list, i);
    }
    assert(list.head->value == 0);
    Node* cur = list.head;
    int count = 1;
    while (cur->next) {
        cur = cur->next;
        count++;
    }
    assert(cur->value == N - 1);
    assert(count == N);

    Node* middle = getNode(list, N/2);
    assert(middle && middle->value == N/2);

    for (int i = 0; i < N/2; i++) {
        bool removed = removeValue(list, i);
        assert(removed);
    }

    cur = list.head;
    int expected = N/2;
    while (cur) {
        assert(cur->value == expected);
        expected++;
        cur = cur->next;
    }
    assert(expected == N);

    clearList(list);
    assert(list.head == nullptr);
}

// -------------------- Раннер --------------------
int fails = 0;
int total = 0;

void runTest(const std::string &name, void(*func)()) {
    total++;
    try {
        func();
        std::cout << GREEN << "." << RESET;
    } catch (...) {
        fails++;
        std::cout << RED << "F" << RESET;
        std::cerr << "\nTest failed: " << name << std::endl;
    }
}

int main() {
    // Базовые тесты
    runTest("testInit", testInit);
    runTest("testAddHead", testAddHead);
    runTest("testAddTail", testAddTail);
    runTest("testAddBefore", testAddBefore);
    runTest("testAddAfter", testAddAfter);
    runTest("testRemoveHead", testRemoveHead);
    runTest("testRemoveTail", testRemoveTail);
    runTest("testRemoveValue", testRemoveValue);

    // Расширенные тесты
    runTest("testGetNode", testGetNode);
    runTest("testFindValue", testFindValue);
    runTest("testPrintList", testPrintList);
    runTest("testFileIO", testFileIO);
    runTest("testClearList", testClearList);

    // Стресс-тест
    runTest("testStress", testStress);

    std::cout << std::endl;
    if (fails == 0) {
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    } else {
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;
    }

    return fails ? 1 : 0;
}
