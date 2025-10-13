#include "d/Stack.hpp"
#include <fstream>

static const int MIN_CAPACITY = 4;

void initStack(Stack &s, int capacity) {
    if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;
    s.data = new std::string[capacity];
    s.size = 0;
    s.capacity = capacity;
}

static void growIfNeeded(Stack &s) {
    if (s.size < s.capacity) return;
    int newCap = s.capacity * 2;
    std::string* newData = new std::string[newCap];
    for (int i = 0; i < s.size; ++i) newData[i] = s.data[i];
    delete[] s.data;
    s.data = newData;
    s.capacity = newCap;
}

static void shrinkIfNeeded(Stack &s) {
    if (s.capacity <= MIN_CAPACITY) return;
    if (s.size * 4 >= s.capacity) return;
    int newCap = s.capacity / 2;
    if (newCap < MIN_CAPACITY) newCap = MIN_CAPACITY;
    std::string* newData = new std::string[newCap];
    for (int i = 0; i < s.size; ++i) newData[i] = s.data[i];
    delete[] s.data;
    s.data = newData;
    s.capacity = newCap;
}

bool push(Stack &s, const std::string &val) {
    growIfNeeded(s);
    s.data[s.size++] = val;
    return true;
}

bool pop(Stack &s, std::string &val) {
    if (s.size == 0) return false;
    val = s.data[--s.size];
    shrinkIfNeeded(s);
    return true;
}

std::string top(Stack &s) {
    if (s.size == 0) return std::string();
    return s.data[s.size - 1];
}

int length(Stack &s) {
    return s.size;
}

void readFromFile(Stack &s, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    std::string val;
    while (in >> val) push(s, val);
    in.close();
}

void writeToFile(Stack &s, const std::string &filename) {
    std::ofstream out(filename);
    for (int i = 0; i < s.size; ++i) {
        out << s.data[i];
        if (i + 1 < s.size) out << " ";
    }
    out.close();
}

void clearStack(Stack &s) {
    delete[] s.data;
    s.data = nullptr;
    s.size = 0;
    s.capacity = 0;
}
