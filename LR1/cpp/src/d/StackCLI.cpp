#include "d/Stack.hpp"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "stack.txt";

static Stack stackInstance;

void loadStack() {
    clearStack(stackInstance);
    initStack(stackInstance);
    readFromFile(stackInstance, DATA_FILE);
}

void saveStack() {
    writeToFile(stackInstance, DATA_FILE);
}

void printStack(Stack &s) {
    for (int i = 0; i < s.size; ++i) {
        std::cout << s.data[i];
        if (i + 1 < s.size) std::cout << " ";
    }
    std::cout << std::endl;
}

void processCommand(const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") {
        std::string val; ss >> val;
        push(stackInstance, val);
        saveStack();
        std::cout << "Pushed " << val << "\n";
        return;
    }

    if (cmd == "POP") {
        std::string val;
        if (pop(stackInstance, val)) {
            saveStack();
            std::cout << "Popped " << val << "\n";
        } else {
            std::cout << "Stack empty\n";
        }
        return;
    }

    if (cmd == "TOP") {
        if (length(stackInstance) > 0) std::cout << "Top = " << top(stackInstance) << "\n";
        else std::cout << "Stack empty\n";
        return;
    }

    if (cmd == "LENGTH") {
        std::cout << "Length = " << length(stackInstance) << "\n";
        return;
    }

    if (cmd == "CLEAR") {
        clearStack(stackInstance);
        initStack(stackInstance);
        saveStack();
        std::cout << "Stack cleared\n";
        return;
    }

    if (cmd == "PRINT") {
        printStack(stackInstance);
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    initStack(stackInstance);
    loadStack();

    std::cout << "Stack CLI. Commands:\n"
              << "  PUSH <val>   - push value\n"
              << "  POP          - pop value\n"
              << "  TOP          - show top\n"
              << "  LENGTH       - show length\n"
              << "  PRINT        - print stack (bottom..top)\n"
              << "  CLEAR        - clear stack\n"
              << "  EXIT         - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    clearStack(stackInstance);
    return 0;
}
