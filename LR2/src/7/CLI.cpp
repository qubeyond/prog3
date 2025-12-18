#include <iostream>
#include <string>
#include "7/LruCache.hpp"

int main() {
    size_t cap;
    if (!(std::cin >> cap)) return 0;
    LruCache cache(cap);
    std::string cmd, key, val;
    while (std::cin >> cmd) {
        if (cmd == "PUT") {
            std::cin >> key >> val;
            cache.put(key, val);
        } else if (cmd == "GET") {
            std::cin >> key;
            auto res = cache.get(key);
            if (res) std::cout << *res << std::endl;
            else std::cout << "NULL" << std::endl;
        } else if (cmd == "EXIT") break;
    }
    return 0;
}