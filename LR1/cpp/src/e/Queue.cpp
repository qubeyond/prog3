#include "e/Queue.hpp"
#include <fstream>

static const int MIN_CAPACITY_Q = 4;

void initQueue(Queue &q, int capacity) {
    if (capacity < MIN_CAPACITY_Q) capacity = MIN_CAPACITY_Q;
    q.data = new std::string[capacity];
    q.size = 0;
    q.capacity = capacity;
    q.front = 0;
    q.back = 0;
}

static void growIfNeeded(Queue &q) {
    if (q.size < q.capacity) return;
    int newCap = q.capacity * 2;
    std::string* newData = new std::string[newCap];
    for (int i = 0; i < q.size; ++i) {
        newData[i] = q.data[(q.front + i) % q.capacity];
    }
    delete[] q.data;
    q.data = newData;
    q.capacity = newCap;
    q.front = 0;
    q.back = q.size % q.capacity;
}

static void shrinkIfNeeded(Queue &q) {
    if (q.capacity <= MIN_CAPACITY_Q) return;
    if (q.size * 4 >= q.capacity) return;
    int newCap = q.capacity / 2;
    if (newCap < MIN_CAPACITY_Q) newCap = MIN_CAPACITY_Q;
    std::string* newData = new std::string[newCap];
    for (int i = 0; i < q.size; ++i) {
        newData[i] = q.data[(q.front + i) % q.capacity];
    }
    delete[] q.data;
    q.data = newData;
    q.capacity = newCap;
    q.front = 0;
    q.back = q.size % q.capacity;
}

bool push(Queue &q, const std::string &val) {
    growIfNeeded(q);
    q.data[q.back] = val;
    q.back = (q.back + 1) % q.capacity;
    q.size++;
    return true;
}

bool pop(Queue &q, std::string &val) {
    if (q.size == 0) return false;
    val = q.data[q.front];
    q.front = (q.front + 1) % q.capacity;
    q.size--;
    shrinkIfNeeded(q);
    return true;
}

std::string peek(Queue &q) {
    if (q.size == 0) return std::string();
    return q.data[q.front];
}

int length(Queue &q) {
    return q.size;
}

void readFromFile(Queue &q, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    clearQueue(q);
    initQueue(q);
    std::string v;
    while (in >> v) push(q, v);
    in.close();
}

void writeToFile(Queue &q, const std::string &filename) {
    std::ofstream out(filename);
    for (int i = 0; i < q.size; ++i) {
        out << q.data[(q.front + i) % q.capacity];
        if (i + 1 < q.size) out << " ";
    }
    out.close();
}

void clearQueue(Queue &q) {
    delete[] q.data;
    q.data = nullptr;
    q.size = 0;
    q.capacity = 0;
    q.front = 0;
    q.back = 0;
}
