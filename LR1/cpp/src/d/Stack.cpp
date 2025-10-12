#include "d/Stack.hpp"
#include <fstream>
#include <algorithm>

void initStack(Stack &s, int capacity) {
    s.data = new int[capacity];
    s.size = 0;
    s.capacity = capacity;
}

bool push(Stack &s, int val) {
    if (s.size >= s.capacity) return false;
    s.data[s.size++] = val;
    return true;
}

bool pop(Stack &s, int &val) {
    if (s.size == 0) return false;
    val = s.data[--s.size];
    return true;
}

int top(Stack &s) {
    if (s.size == 0) return -1; // либо throw
    return s.data[s.size - 1];
}

int length(Stack &s) {
    return s.size;
}

void readFromFile(Stack &s, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) push(s, val);
    in.close();
}

void writeToFile(Stack &s, const std::string &filename) {
    std::ofstream out(filename);
    for (int i = 0; i < s.size; i++) out << s.data[i] << " ";
    out.close();
}

void clearStack(Stack &s) {
    delete[] s.data;
    s.data = nullptr;
    s.size = 0;
    s.capacity = 0;
}
