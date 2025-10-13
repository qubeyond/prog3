#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include "a/Array.hpp"

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"

void testInit() {
    Array arr;
    initArray(arr);
    assert(arr.size == 0 && arr.data != nullptr);
    clearArray(arr);
}

void testAddEnd() {
    Array arr;
    initArray(arr);
    addEnd(arr, "A");
    addEnd(arr, "B");
    assert(arr.size == 2);
    assert(arr.data[0] == "A" && arr.data[1] == "B");
    clearArray(arr);
}

void testAddAt() {
    Array arr;
    initArray(arr);
    addEnd(arr, "A");
    addEnd(arr, "C");
    assert(addAt(arr, 1, "B"));
    assert(arr.data[1] == "B" && arr.size == 3);
    assert(!addAt(arr, 5, "Z"));
    clearArray(arr);
}

void testGetSet() {
    Array arr;
    initArray(arr);
    addEnd(arr, "X");
    addEnd(arr, "Y");
    assert(get(arr, 0) == "X");
    assert(get(arr, 1) == "Y");
    assert(set(arr, 0, "Z"));
    assert(arr.data[0] == "Z");
    assert(!set(arr, 3, "Q"));
    clearArray(arr);
}

void testRemoveAt() {
    Array arr;
    initArray(arr);
    addEnd(arr, "A");
    addEnd(arr, "B");
    addEnd(arr, "C");
    assert(removeAt(arr, 1));
    assert(arr.size == 2 && arr.data[1] == "C");
    assert(!removeAt(arr, 5));
    clearArray(arr);
}

void testFileIO() {
    Array arr;
    initArray(arr);
    addEnd(arr, "Hello");
    addEnd(arr, "World");
    writeToFile(arr, "test_array.txt");
    clearArray(arr);
    readFromFile(arr, "test_array.txt");
    assert(arr.size == 2);
    assert(arr.data[0] == "Hello" && arr.data[1] == "World");
    clearArray(arr);
    std::remove("test_array.txt");
}

int fails = 0, total = 0;

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
    runTest("Init", testInit);
    runTest("AddEnd", testAddEnd);
    runTest("AddAt", testAddAt);
    runTest("GetSet", testGetSet);
    runTest("RemoveAt", testRemoveAt);
    runTest("FileIO", testFileIO);
    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;
    return fails ? 1 : 0;
}
