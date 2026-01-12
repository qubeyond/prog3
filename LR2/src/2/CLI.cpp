#include <iostream>
#include <string>
#include "2/Set.hpp"

int main() {
    Set s; 
    std::string cmd, value;

    std::cout << "Введите команду (SETADD, SETDEL, SET_AT) и значение, или EXIT для выхода:" << std::endl;

    while (std::cin >> cmd) {
        if (cmd == "EXIT") {
            break;
        }

        if (!(std::cin >> value)) {
            break;
        }

        if (cmd == "SETADD") {
            s.add(value);
        } else if (cmd == "SETDEL") {
            s.del(value);
        } else if (cmd == "SET_AT") {
            std::cout << (s.contains(value) ? "YES" : "NO") << std::endl;
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}