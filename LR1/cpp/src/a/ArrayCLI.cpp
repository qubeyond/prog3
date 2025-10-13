#include <iostream>
#include <sstream>
#include "a/Array.hpp"

const std::string DATA_FILE = "array.txt";

void loadArray(Array &arr) {
    clearArray(arr);
    initArray(arr);
    readFromFile(arr, DATA_FILE);
}

void saveArray(Array &arr) {
    writeToFile(arr, DATA_FILE);
}

void printArray(Array &arr) {
    for (int i = 0; i < arr.size; i++) std::cout << arr.data[i] << (i < arr.size - 1 ? " " : "");
    std::cout << std::endl;
}

void processCommand(Array &arr, const std::string &commandLine) {
    std::stringstream ss(commandLine);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        printArray(arr);
        return;
    }

    if (cmd == "ACLEAR") {
        clearArray(arr);
        initArray(arr);
        saveArray(arr);
        std::cout << "Array cleared.\n";
        return;
    }

    if (cmd == "AADDEND") {
        std::string val; ss >> val;
        addEnd(arr, val);
        saveArray(arr);
        std::cout << "Added " << val << " to end.\n";
        return;
    }

    if (cmd == "AADD") {
        int index; std::string val; ss >> index >> val;
        if (addAt(arr, index, val)) {
            saveArray(arr);
            std::cout << "Inserted " << val << " at index " << index << ".\n";
        } else std::cout << "Index out of range.\n";
        return;
    }

    if (cmd == "AGET") {
        int index; ss >> index;
        try {
            std::cout << "Array[" << index << "] = " << get(arr, index) << "\n";
        } catch (...) {
            std::cout << "Index out of range.\n";
        }
        return;
    }

    if (cmd == "ASET") {
        int index; std::string val; ss >> index >> val;
        if (set(arr, index, val)) {
            saveArray(arr);
            std::cout << "Set index " << index << " = " << val << "\n";
        } else std::cout << "Index out of range.\n";
        return;
    }

    if (cmd == "AREM") {
        int index; ss >> index;
        if (removeAt(arr, index)) {
            saveArray(arr);
            std::cout << "Removed element at index " << index << ".\n";
        } else std::cout << "Index out of range.\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    Array arr;
    initArray(arr);
    loadArray(arr);

    std::cout << "Array CLI. Commands:\n"
              << "  AADDEND <val>       - add to end\n"
              << "  AADD <index> <val>  - add at index\n"
              << "  AGET <index>        - get value at index\n"
              << "  ASET <index> <val>  - set value at index\n"
              << "  AREM <index>        - remove element at index\n"
              << "  ACLEAR              - clear array\n"
              << "  PRINT               - print array\n"
              << "  EXIT                - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(arr, line);
    }

    clearArray(arr);
    return 0;
}