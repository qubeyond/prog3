#include <iostream>
#include "6/HashTableOpen.hpp"
#include "6/HashTableChains.hpp"

template <typename T>
void run_cli(T& ht) {
    std::string cmd, key;
    int val;
    while (std::cin >> cmd && cmd != "EXIT") {
        if (cmd == "PUT") {
            if (std::cin >> key >> val) ht.insert(key, val);
        } else if (cmd == "GET") {
            if (std::cin >> key) {
                auto res = ht.search(key);
                std::cout << (res ? std::to_string(*res) : "-1") << std::endl;
            }
        } else if (cmd == "DEL") {
            if (std::cin >> key) ht.remove(key);
        }
    }
}

int main() {
    std::cout << "Select Hash Table Type:\n1 - Open Addressing\n2 - Separate Chaining\nChoice: ";
    int choice;
    if (!(std::cin >> choice)) return 0;

    if (choice == 1) {
        HashTableOpen ht;
        std::cout << "Mode: Open Addressing. Commands: PUT, GET, DEL, EXIT" << std::endl;
        run_cli(ht);
    } else if (choice == 2) {
        HashTableChains ht;
        std::cout << "Mode: Separate Chaining. Commands: PUT, GET, DEL, EXIT" << std::endl;
        run_cli(ht);
    }

    return 0;
}