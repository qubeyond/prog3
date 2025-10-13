#include "f/RedBlackTree.hpp"
#include <iostream>
#include <sstream>

const std::string DATA_FILE = "rbtree.txt";
static RBTree tree;

void loadTree() {
    clearTree(tree);
    initTree(tree);
    readFromFile(tree, DATA_FILE);
}

void saveTree() {
    writeToFile(tree, DATA_FILE);
}

void processCommand(const std::string &line) {
    std::stringstream ss(line);
    std::string cmd;
    ss >> cmd;

    if (cmd == "INSERT") {
        int val; ss >> val;
        insert(tree, val);
        saveTree();
        std::cout << "Inserted " << val << "\n";
        return;
    }

    if (cmd == "REMOVE") {
        int val; ss >> val;
        if (remove(tree, val)) { saveTree(); std::cout << "Removed " << val << "\n"; }
        else std::cout << "Value not found\n";
        return;
    }

    if (cmd == "FIND") {
        int val; ss >> val;
        if (find(tree, val)) std::cout << "Found " << val << "\n";
        else std::cout << "Not found\n";
        return;
    }

    if (cmd == "PRINT") {
        std::cout << "Tree structure (R:red, B:black):\n";
        printTree(tree); // визуально
        return;
    }

    if (cmd == "INORDER") {
        inorderPrint(tree); // сортированный вывод
        return;
    }

    if (cmd == "CLEAR") {
        clearTree(tree);
        saveTree();
        std::cout << "Tree cleared\n";
        return;
    }

    std::cout << "Unknown command: " << cmd << "\n";
}

int main() {
    initTree(tree);
    loadTree();

    std::cout << "Red-Black Tree CLI. Commands:\n"
              << "  INSERT <val>  - add element\n"
              << "  REMOVE <val>  - remove element\n"
              << "  FIND <val>    - search element\n"
              << "  PRINT         - show tree structure\n"
              << "  INORDER       - show sorted values\n"
              << "  CLEAR         - clear tree\n"
              << "  EXIT          - quit\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT") break;
        if (!line.empty()) processCommand(line);
    }

    clearTree(tree);
    return 0;
}
