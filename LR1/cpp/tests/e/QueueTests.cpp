#include "e/Queue.hpp"
#include <cassert>
#include <iostream>
#include <cstdio>

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

void testPushPop() {
    Queue q;
    initQueue(q);
    push(q, "A");
    push(q, "B");
    std::string val;
    assert(pop(q, val) && val == "A");
    assert(pop(q, val) && val == "B");
    assert(!pop(q, val));
    clearQueue(q);
}

void testPeekLength() {
    Queue q;
    initQueue(q);
    push(q, "one");
    push(q, "two");
    assert(peek(q) == "one");
    assert(length(q) == 2);
    clearQueue(q);
}

void testFileIO() {
    Queue q;
    initQueue(q);
    push(q, "x");
    push(q, "y");
    writeToFile(q, "test_queue.txt");
    clearQueue(q);
    initQueue(q);
    readFromFile(q, "test_queue.txt");
    assert(length(q) == 2 && peek(q) == "x");
    clearQueue(q);
    std::remove("test_queue.txt");
}

void testClear() {
    Queue q;
    initQueue(q);
    push(q, "z");
    clearQueue(q);
    assert(q.size == 0);
}

int main() {
    int fails = 0;
    int total = 0;
    auto run = [&](const char* name, void(*f)()){
        ++total;
        try { f(); std::cout << GREEN << "." << RESET; }
        catch(...) { ++fails; std::cout << RED << "F" << RESET; std::cerr << "\nTest failed: " << name << std::endl; }
    };
    run("testPushPop", testPushPop);
    run("testPeekLength", testPeekLength);
    run("testFileIO", testFileIO);
    run("testClear", testClear);
    std::cout << std::endl;
    if (fails == 0) std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;
    return fails ? 1 : 0;
}
