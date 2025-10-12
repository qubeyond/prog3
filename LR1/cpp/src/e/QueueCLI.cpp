#include <iostream>
#include <sstream>
#include "e/Queue.hpp"

const std::string DATA_FILE = "queue.txt";
Queue queue;

void loadQueue() {
    clearQueue(queue);
    initQueue(queue);
    readFromFile(queue, DATA_FILE);
}

void saveQueue() {
    writeToFile(queue, DATA_FILE);
}

void processCommand(const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") { int val; ss >> val; push(queue, val); saveQueue(); std::cout << "Pushed " << val << "\n"; return; }
    if (cmd == "POP") { int val; if (pop(queue, val)) { saveQueue(); std::cout << "Popped " << val << "\n"; } else std::cout << "Queue empty\n"; return; }
    if (cmd == "PEEK") { int val = peek(queue); if (length(queue) > 0) std::cout << "Front = " << val << "\n"; else std::cout << "Queue empty\n"; return; }
    if (cmd == "LENGTH") { std::cout << "Length = " << length(queue) << "\n"; return; }
    if (cmd == "CLEAR") { clearQueue(queue); initQueue(queue); saveQueue(); std::cout << "Queue cleared\n"; return; }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    initQueue(queue);
    loadQueue();

    std::cout << "Queue CLI. Commands:\n"
              << "  PUSH <val>   - enqueue\n"
              << "  POP          - dequeue\n"
              << "  PEEK         - show front\n"
              << "  LENGTH       - show length\n"
              << "  CLEAR        - clear queue\n"
              << "  EXIT         - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    clearQueue(queue);
    return 0;
}
