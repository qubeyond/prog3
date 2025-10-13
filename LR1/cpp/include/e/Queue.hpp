#pragma once
#include <string>

struct Queue {
    std::string* data;
    int size;
    int capacity;
    int front;
    int back;
};

void initQueue(Queue &q, int capacity = 100);

bool push(Queue &q, const std::string &val);
bool pop(Queue &q, std::string &val);

std::string peek(Queue &q);
int length(Queue &q);

void readFromFile(Queue &q, const std::string &filename);
void writeToFile(Queue &q, const std::string &filename);

void clearQueue(Queue &q);
