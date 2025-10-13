#include <iostream>
#include <sstream>
#include <string>
#include "b/LinkedList.hpp"

const std::string DATA_FILE = "list.txt";

void loadList(LinkedList &list) {
    clearList(list);
    readFromFile(list, DATA_FILE);
}

void saveList(LinkedList &list) {
    writeToFile(list, DATA_FILE);
}

void processCommand(LinkedList &list, const std::string &commandLine) {
    std::stringstream ss(commandLine);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") {
        printList(list);
        return;
    }

    if (cmd == "LCLEAR") {
        clearList(list);
        saveList(list);
        std::cout << "List cleared.\n";
        return;
    }

    if (cmd == "LADDH") {
        std::string val; ss >> val;
        addHead(list, val);
        saveList(list);
        std::cout << "Added " << val << " to head.\n";
        return;
    }

    if (cmd == "LADDT") {
        std::string val; ss >> val;
        addTail(list, val);
        saveList(list);
        std::cout << "Added " << val << " to tail.\n";
        return;
    }

    if (cmd == "LADDB") {
        std::string target, val; ss >> target >> val;
        if (addBefore(list, target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " before " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "LADDA") {
        std::string target, val; ss >> target >> val;
        if (addAfter(list, target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " after " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "LREMH") {
        if (removeHead(list)) {
            saveList(list);
            std::cout << "Head removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "LREMT") {
        if (removeTail(list)) {
            saveList(list);
            std::cout << "Tail removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "LREMV") {
        std::string val; ss >> val;
        if (removeValue(list, val)) {
            saveList(list);
            std::cout << "Removed value " << val << ".\n";
        } else {
            std::cout << "Value not found.\n";
        }
        return;
    }

    if (cmd == "LGET") {
        int idx; ss >> idx;
        Node* n = getNode(list, idx);
        if (n) std::cout << "Node[" << idx << "] = " << n->value << "\n";
        else std::cout << "Index out of range.\n";
        return;
    }

    if (cmd == "LFIND") {
        std::string val; ss >> val;
        Node* n = findValue(list, val);
        if (n) std::cout << "Found value: " << val << "\n";
        else std::cout << "Value not found.\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    LinkedList list;
    initList(list);
    loadList(list);

    std::cout << "LinkedList CLI. Commands:\n"
              << "  LADDH <val>               - add to head\n"
              << "  LADDT <val>               - add to tail\n"
              << "  LADDB <target> <val>      - add before target\n"
              << "  LADDA <target> <val>      - add after target\n"
              << "  LREMH                     - remove head\n"
              << "  LREMT                     - remove tail\n"
              << "  LREMV <val>               - remove by value\n"
              << "  LGET <index>              - get node by index\n"
              << "  LFIND <val>               - find node by value\n"
              << "  LCLEAR                    - clear list\n"
              << "  PRINT                     - print list\n"
              << "  EXIT                      - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(list, line);
    }

    clearList(list);
    return 0;
}