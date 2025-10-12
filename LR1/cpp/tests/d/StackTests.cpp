#include <cassert>
#include <iostream>
#include <fstream>
#include "d/Stack.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void testPushPop() {
    Stack s;
    initStack(s);
    push(s, 10); push(s, 20);
    int val;
    assert(pop(s, val) && val == 20);
    assert(pop(s, val) && val == 10);
    assert(!pop(s, val));
    clearStack(s);
}

void testTopLength() {
    Stack s;
    initStack(s);
    push(s, 1); push(s, 2);
    assert(top(s) == 2);
    assert(length(s) == 2);
    clearStack(s);
}

void testFileIO() {
    Stack s;
    initStack(s);
    push(s, 100); push(s, 200);
    writeToFile(s, "test_stack.txt");
    clearStack(s);

    initStack(s);
    readFromFile(s, "test_stack.txt");
    assert(length(s) == 2 && top(s) == 200);
    clearStack(s);
    std::remove("test_stack.txt");
}

void testClear() {
    Stack s;
    initStack(s);
    push(s, 1);
    clearStack(s);
    assert(s.size == 0);
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
    runTest("testTopLength", testTopLength);
    runTest("testFileIO", testFileIO);
    runTest("testClear", testClear);

    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;

    return fails ? 1 : 0;
}
