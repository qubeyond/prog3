#include <cassert>
#include <iostream>
#include "f/RedBlackTree.hpp"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

void testInsertFind() {
    RBTree t;
    initTree(t);
    insert(t, 10);
    insert(t, 5);
    insert(t, 20);
    assert(find(t, 10));
    assert(find(t, 5));
    assert(find(t, 20));
    assert(!find(t, 15));
    clearTree(t);
}

void testRemove() {
    RBTree t;
    initTree(t);
    insert(t, 10); insert(t, 5); insert(t, 20);
    assert(remove(t, 5));
    assert(!find(t, 5));
    assert(remove(t, 20));
    assert(!find(t, 20));
    assert(!remove(t, 999));
    clearTree(t);
}

void testFileIO() {
    RBTree t;
    initTree(t);
    insert(t, 1); insert(t, 2); insert(t, 3);
    writeToFile(t, "test_rbtree.txt");
    clearTree(t);
    initTree(t);
    readFromFile(t, "test_rbtree.txt");
    assert(find(t, 1) && find(t, 2) && find(t, 3));
    clearTree(t);
    std::remove("test_rbtree.txt");
}

void testClear() {
    RBTree t;
    initTree(t);
    insert(t, 10);
    clearTree(t);
    assert(!t.root);
}

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
    runTest("testInsertFind", testInsertFind);
    runTest("testRemove", testRemove);
    runTest("testFileIO", testFileIO);
    runTest("testClear", testClear);

    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;

    return fails ? 1 : 0;
}
