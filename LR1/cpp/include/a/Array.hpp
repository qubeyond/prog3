#pragma once
#include <string>

struct Array {
    std::string* data;
    int size;
    int capacity;
};

void initArray(Array &arr, int initialCapacity = 10);

void addEnd(Array &arr, const std::string &val);
bool addAt(Array &arr, int index, const std::string &val);

std::string get(Array &arr, int index);
bool set(Array &arr, int index, const std::string &val);

bool removeAt(Array &arr, int index);

int length(Array &arr);

void readFromFile(Array &arr, const std::string &filename);
void writeToFile(Array &arr, const std::string &filename);

void clearArray(Array &arr);
