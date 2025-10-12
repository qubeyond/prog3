#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include "c/DoublyLinkedList.hpp"

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

void testInit() {
    DLinkedList list;
    initDList(list);
    assert(list.head == nullptr && list.tail == nullptr);
}

void testAddHeadTail() {
    DLinkedList list;
    initDList(list);
    addHead(list, 10);
    addTail(list, 20);
    assert(list.head->value == 10 && list.tail->value == 20);
    clearList(list);
}

void testAddBeforeAfter() {
    DLinkedList list;
    initDList(list);
    addHead(list, 10);
    addTail(list, 30);
    bool added = addBefore(list, 30, 20);
    assert(added && list.head->next->value == 20);
    added = addAfter(list, 10, 5);
    assert(added && list.head->next->value == 5);
    assert(!addBefore(list, 999, 0));
    assert(!addAfter(list, 999, 0));
    clearList(list);
}

void testRemove() {
    DLinkedList list;
    initDList(list);
    addHead(list, 10);
    addTail(list, 20);
    addTail(list, 30);
    assert(removeHead(list));
    assert(list.head->value == 20);
    assert(removeTail(list));
    assert(list.tail->value == 20);
    addTail(list, 30);
    assert(removeValue(list, 30));
    assert(!removeValue(list, 999));
    clearList(list);
}

void testGetFind() {
    DLinkedList list;
    initDList(list);
    addTail(list, 10);
    addTail(list, 20);
    addTail(list, 30);
    DNode* n0 = getNode(list, 0);
    DNode* n1 = getNode(list, 1);
    DNode* n2 = getNode(list, 2);
    DNode* n3 = getNode(list, 3);
    assert(n0 && n0->value == 10);
    assert(n1 && n1->value == 20);
    assert(n2 && n2->value == 30);
    assert(n3 == nullptr);

    DNode* f1 = findValue(list, 20);
    DNode* f2 = findValue(list, 999);
    assert(f1 && f1->value == 20);
    assert(f2 == nullptr);
    clearList(list);
}

void testPrint() {
    DLinkedList list;
    initDList(list);
    addTail(list, 1);
    addTail(list, 2);
    addTail(list, 3);
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printList(list);
    std::cout.rdbuf(old);
    assert(buffer.str() == "1 2 3 \n");
    clearList(list);
}

void testFileIO() {
    DLinkedList list;
    initDList(list);
    addTail(list, 100);
    addTail(list, 200);
    writeToFile(list, "test_dlist.txt");
    clearList(list);
    readFromFile(list, "test_dlist.txt");
    assert(list.head && list.head->value == 100);
    assert(list.tail && list.tail->value == 200);
    clearList(list);
    std::remove("test_dlist.txt");
}

void testStress() {
    DLinkedList list;
    initDList(list);
    const int N = 10000;
    for (int i = 0; i < N; i++) addTail(list, i);
    assert(list.head->value == 0 && list.tail->value == N-1);

    for (int i = 0; i < N/2; i++) removeHead(list);
    assert(list.head->value == N/2);

    clearList(list);
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
    runTest("testInit", testInit);
    runTest("testAddHeadTail", testAddHeadTail);
    runTest("testAddBeforeAfter", testAddBeforeAfter);
    runTest("testRemove", testRemove);
    runTest("testGetFind", testGetFind);
    runTest("testPrint", testPrint);
    runTest("testFileIO", testFileIO);
    runTest("testStress", testStress);

    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;

    return fails ? 1 : 0;
}
