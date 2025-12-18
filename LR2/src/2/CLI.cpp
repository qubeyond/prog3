#include <iostream>
#include <string>
#include <vector>
#include "2/Set.hpp"

int main(int argc, char* argv[]) {
    Set s;
    std::string query, value;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--query" && i + 1 < argc) query = argv[++i];
        if (arg == "--value" && i + 1 < argc) value = argv[++i];
    }

    if (query == "SETADD") {
        s.add(value);
    } else if (query == "SETDEL") {
        s.del(value);
    } else if (query == "SET_AT") {
        std::cout << (s.contains(value) ? "YES" : "NO") << std::endl;
    }

    return 0;
}