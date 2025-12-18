#include <iostream>
#include <string>
#include "4/Accents.hpp"
#include "4/Array.hpp"

int main() {
    int n;
    if (!(std::cin >> n)) return 0;

    Array<std::string> dict(n);
    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        dict.push_back(s);
    }

    Array<std::string> text;
    std::string word;
    while (std::cin >> word) {
        text.push_back(word);
    }

    std::cout << AccentChecker::countErrors(dict, text) << std::endl;
    return 0;
}