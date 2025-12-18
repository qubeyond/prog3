#include <iostream>
#include <string>
#include "6/HashTable.hpp"

int main() {
    HashTable ht;
    std::string cmd, key;
    int val;
    
    while (std::cin >> cmd) {
        if (cmd == "PUT") {
            if (std::cin >> key >> val) ht.insert(key, val);
        } else if (cmd == "GET") {
            if (std::cin >> key) {
                auto res = ht.search(key);
                if (res) std::cout << *res << std::endl;
                else std::cout << "-1" << std::endl;
            }
        } else if (cmd == "DEL") {
            if (std::cin >> key) ht.remove(key);
        } else if (cmd == "EXIT") {
            break;
        }
    }
    return 0;
}