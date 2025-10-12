#include "e/Queue.hpp"
#include <fstream>

void initQueue(Queue &q, int capacity) {
    q.data = new int[capacity];
    q.size = 0;
    q.capacity = capacity;
    q.front = 0;
    q.back = 0;
}

bool push(Queue &q, int val) {
    if (q.size >= q.capacity) return false;
    q.data[q.back] = val;
    q.back = (q.back + 1) % q.capacity;
    q.size++;
    return true;
}

bool pop(Queue &q, int &val) {
    if (q.size == 0) return false;
    val = q.data[q.front];
    q.front = (q.front + 1) % q.capacity;
    q.size--;
    return true;
}

int peek(Queue &q) {
    if (q.size == 0) return -1;
    return q.data[q.front];
}

int length(Queue &q) {
    return q.size;
}

void readFromFile(Queue &q, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;
    int val;
    while (in >> val) push(q, val);
    in.close();
}

void writeToFile(Queue &q, const std::string &filename) {
    std::ofstream out(filename);
    for (int i = 0, idx = q.front; i < q.size; i++) {
        out << q.data[idx] << " ";
        idx = (idx + 1) % q.capacity;
    }
    out.close();
}

void clearQueue(Queue &q) {
    delete[] q.data;
    q.data = nullptr;
    q.size = 0;
    q.front = 0;
    q.back = 0;
    q.capacity = 0;
}