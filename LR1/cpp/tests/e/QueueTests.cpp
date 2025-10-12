#include <cassert>
#include <iostream>
#include "e/Queue.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void testPushPop() {
    Queue q;
    initQueue(q);
    push(q, 10); push(q, 20);
    int val;
    assert(pop(q, val) && val == 10);
    assert(pop(q, val) && val == 20);
    assert(!pop(q, val));
    clearQueue(q);
}

void testPeekLength() {
    Queue q;
    initQueue(q);
    push(q, 1); push(q, 2);
    assert(peek(q) == 1);
    assert(length(q) == 2);
    clearQueue(q);
}

void testFileIO() {
    Queue q;
    initQueue(q);
    push(q, 100); push(q, 200);
    writeToFile(q, "test_queue.txt");
    clearQueue(q);

    initQueue(q);
    readFromFile(q, "test_queue.txt");
    assert(length(q) == 2 && peek(q) == 100);
    clearQueue(q);
    std::remove("test_queue.txt");
}

void testClear() {
    Queue q;
    initQueue(q);
    push(q, 1);
    clearQueue(q);
    assert(q.size == 0);
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
    runTest("testPushPop", testPushPop);
    runTest("testPeekLength", testPeekLength);
    runTest("testFileIO", testFileIO);
    runTest("testClear", testClear);

    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;

    return fails ? 1 : 0;
}
