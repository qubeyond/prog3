#include "d/Stack.hpp"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <fstream>

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

void testPushPop() {
    Stack s;
    initStack(s);
    push(s, "A");
    push(s, "B");
    std::string val;
    assert(pop(s, val) && val == "B");
    assert(pop(s, val) && val == "A");
    assert(!pop(s, val));
    clearStack(s);
}

void testTopLength() {
    Stack s;
    initStack(s);
    push(s, "X");
    push(s, "Y");
    assert(top(s) == "Y");
    assert(length(s) == 2);
    clearStack(s);
}

void testFileIO() {
    Stack s;
    initStack(s);
    push(s, "one");
    push(s, "two");
    writeToFile(s, "test_stack.txt");
    clearStack(s);
    initStack(s);
    readFromFile(s, "test_stack.txt");
    assert(length(s) == 2 && top(s) == "two");
    clearStack(s);
    std::remove("test_stack.txt");
}

void testClear() {
    Stack s;
    initStack(s);
    push(s, "z");
    clearStack(s);
    assert(s.size == 0);
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
    run("testTopLength", testTopLength);
    run("testFileIO", testFileIO);
    run("testClear", testClear);
    std::cout << std::endl;
    if (fails == 0) std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;
    return fails ? 1 : 0;
}
