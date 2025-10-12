#include <iostream>
#include <sstream>
#include "d/Stack.hpp"

const std::string DATA_FILE = "stack.txt";

Stack stack;

void loadStack() {
    clearStack(stack);
    initStack(stack);
    readFromFile(stack, DATA_FILE);
}

void saveStack() {
    writeToFile(stack, DATA_FILE);
}

void processCommand(const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") { int val; ss >> val; push(stack, val); saveStack(); std::cout << "Pushed " << val << "\n"; return; }
    if (cmd == "POP") { int val; if (pop(stack, val)) { saveStack(); std::cout << "Popped " << val << "\n"; } else { std::cout << "Stack empty\n"; } return; }
    if (cmd == "TOP") { int val = top(stack); if (stack.size > 0) std::cout << "Top = " << val << "\n"; else std::cout << "Stack empty\n"; return; }
    if (cmd == "LENGTH") { std::cout << "Length = " << length(stack) << "\n"; return; }
    if (cmd == "CLEAR") { clearStack(stack); initStack(stack); saveStack(); std::cout << "Stack cleared\n"; return; }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    initStack(stack);
    loadStack();

    std::cout << "Stack CLI. Commands:\n"
              << "  PUSH <val>   - push value\n"
              << "  POP          - pop value\n"
              << "  TOP          - show top\n"
              << "  LENGTH       - show length\n"
              << "  CLEAR        - clear stack\n"
              << "  EXIT         - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    clearStack(stack);
    return 0;
}
