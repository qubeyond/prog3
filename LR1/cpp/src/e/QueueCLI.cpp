#include "e/Queue.hpp"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "queue.txt";
static Queue queueInstance;

void loadQueue() {
    clearQueue(queueInstance);
    initQueue(queueInstance);
    readFromFile(queueInstance, DATA_FILE);
}

void saveQueue() {
    writeToFile(queueInstance, DATA_FILE);
}

void printQueue(Queue &q) {
    for (int i = 0; i < q.size; ++i) {
        std::cout << q.data[(q.front + i) % q.capacity];
        if (i + 1 < q.size) std::cout << " ";
    }
    std::cout << std::endl;
}

void processCommand(const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PUSH") {
        std::string val; ss >> val;
        push(queueInstance, val);
        saveQueue();
        std::cout << "Pushed " << val << "\n";
        return;
    }

    if (cmd == "POP") {
        std::string val;
        if (pop(queueInstance, val)) {
            saveQueue();
            std::cout << "Popped " << val << "\n";
        } else {
            std::cout << "Queue empty\n";
        }
        return;
    }

    if (cmd == "PEEK") {
        if (length(queueInstance) > 0) std::cout << "Front = " << peek(queueInstance) << "\n";
        else std::cout << "Queue empty\n";
        return;
    }

    if (cmd == "LENGTH") {
        std::cout << "Length = " << length(queueInstance) << "\n";
        return;
    }

    if (cmd == "CLEAR") {
        clearQueue(queueInstance);
        initQueue(queueInstance);
        saveQueue();
        std::cout << "Queue cleared\n";
        return;
    }

    if (cmd == "PRINT") {
        printQueue(queueInstance);
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    initQueue(queueInstance);
    loadQueue();

    std::cout << "Queue CLI. Commands:\n"
              << "  PUSH <val>   - enqueue\n"
              << "  POP          - dequeue\n"
              << "  PEEK         - show front\n"
              << "  LENGTH       - show length\n"
              << "  PRINT        - print queue (front..back)\n"
              << "  CLEAR        - clear queue\n"
              << "  EXIT         - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    clearQueue(queueInstance);
    return 0;
}
