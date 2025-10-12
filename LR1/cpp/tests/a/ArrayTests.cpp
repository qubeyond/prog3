#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include "a/Array.hpp"

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
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
    addEnd(arr, 10);
    addEnd(arr, 20);
    assert(arr.size == 2);
    assert(arr.data[0] == 10 && arr.data[1] == 20);
    clearArray(arr);
}

void testAddAt() {
    Array arr;
    initArray(arr);
    addEnd(arr, 1);
    addEnd(arr, 3);
    assert(addAt(arr, 1, 2));
    assert(arr.data[1] == 2 && arr.size == 3);
    assert(!addAt(arr, 5, 10)); // вне диапазона
    clearArray(arr);
}

void testGetSet() {
    Array arr;
    initArray(arr);
    addEnd(arr, 5);
    addEnd(arr, 10);
    assert(get(arr, 0) == 5);
    assert(get(arr, 1) == 10);
    assert(set(arr, 0, 15));
    assert(arr.data[0] == 15);
    assert(!set(arr, 2, 100));
    clearArray(arr);
}

void testRemoveAt() {
    Array arr;
    initArray(arr);
    addEnd(arr, 1);
    addEnd(arr, 2);
    addEnd(arr, 3);
    assert(removeAt(arr, 1)); // удаляем 2
    assert(arr.size == 2 && arr.data[1] == 3);
    assert(!removeAt(arr, 5)); // вне диапазона
    clearArray(arr);
}

void testLength() {
    Array arr;
    initArray(arr);
    assert(length(arr) == 0);
    addEnd(arr, 1);
    assert(length(arr) == 1);
    clearArray(arr);
}

void testFileIO() {
    Array arr;
    initArray(arr);
    addEnd(arr, 100);
    addEnd(arr, 200);
    writeToFile(arr, "test_array.txt");
    clearArray(arr);
    readFromFile(arr, "test_array.txt");
    assert(arr.size == 2);
    assert(arr.data[0] == 100 && arr.data[1] == 200);
    clearArray(arr);
    std::remove("test_array.txt");
}

void testStress() {
    Array arr;
    initArray(arr);
    const int N = 10000;
    for (int i = 0; i < N; i++) addEnd(arr, i);
    assert(arr.size == N && arr.data[N-1] == N-1);

    for (int i = 0; i < N/2; i++) removeAt(arr, 0);
    assert(arr.size == N/2 && arr.data[0] == N/2);

    clearArray(arr);
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
    runTest("testAddEnd", testAddEnd);
    runTest("testAddAt", testAddAt);
    runTest("testGetSet", testGetSet);
    runTest("testRemoveAt", testRemoveAt);
    runTest("testLength", testLength);
    runTest("testFileIO", testFileIO);
    runTest("testStress", testStress);

    std::cout << std::endl;
    if (fails == 0)
        std::cout << GREEN << "All " << total << " tests passed" << RESET << std::endl;
    else
        std::cout << RED << fails << " of " << total << " tests failed" << RESET << std::endl;

    return fails ? 1 : 0;
}
