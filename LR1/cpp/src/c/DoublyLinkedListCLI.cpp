#include "c/DoublyLinkedList.hpp"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "dlist.txt";

void loadList(DLinkedList &list) {
    clearList(list);
    readFromFile(list, DATA_FILE);
}

void saveList(DLinkedList &list) {
    writeToFile(list, DATA_FILE);
}

void processCommand(DLinkedList &list, const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "PRINT") { printList(list); return; }
    if (cmd == "DCLEAR") { clearList(list); saveList(list); std::cout << "List cleared.\n"; return; }

    if (cmd == "DADDH") {
        std::string val; ss >> val;
        addHead(list, val);
        saveList(list);
        std::cout << "Added " << val << " to head.\n";
        return;
    }

    if (cmd == "DADDT") {
        std::string val; ss >> val;
        addTail(list, val);
        saveList(list);
        std::cout << "Added " << val << " to tail.\n";
        return;
    }

    if (cmd == "DADDB") {
        std::string target, val; ss >> target >> val;
        if (addBefore(list, target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " before " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "DADDA") {
        std::string target, val; ss >> target >> val;
        if (addAfter(list, target, val)) {
            saveList(list);
            std::cout << "Inserted " << val << " after " << target << ".\n";
        } else {
            std::cout << "Target " << target << " not found.\n";
        }
        return;
    }

    if (cmd == "DREMH") {
        if (removeHead(list)) {
            saveList(list);
            std::cout << "Head removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "DREMT") {
        if (removeTail(list)) {
            saveList(list);
            std::cout << "Tail removed.\n";
        } else {
            std::cout << "List empty.\n";
        }
        return;
    }

    if (cmd == "DREMV") {
        std::string val; ss >> val;
        if (removeValue(list, val)) {
            saveList(list);
            std::cout << "Removed value " << val << ".\n";
        } else {
            std::cout << "Value not found.\n";
        }
        return;
    }

    if (cmd == "DGET") {
        int idx; ss >> idx;
        DNode* n = getNode(list, idx);
        if (n) std::cout << "Node[" << idx << "] = " << n->value << "\n";
        else std::cout << "Index out of range.\n";
        return;
    }

    if (cmd == "DFIND") {
        std::string val; ss >> val;
        DNode* n = findValue(list, val);
        if (n) std::cout << "Found value: " << val << "\n";
        else std::cout << "Value not found.\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    DLinkedList list;
    initDList(list);
    loadList(list);

    std::cout << "DoublyLinkedList CLI. Commands:\n"
              << "  DADDH <val>           - add to head\n"
              << "  DADDT <val>           - add to tail\n"
              << "  DADDB <target> <val>  - add before target\n"
              << "  DADDA <target> <val>  - add after target\n"
              << "  DREMH                 - remove head\n"
              << "  DREMT                 - remove tail\n"
              << "  DREMV <val>           - remove by value\n"
              << "  DGET <index>          - get node by index\n"
              << "  DFIND <val>           - find node by value\n"
              << "  DCLEAR                - clear list\n"
              << "  PRINT                 - print list\n"
              << "  EXIT                  - quit\n";

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
