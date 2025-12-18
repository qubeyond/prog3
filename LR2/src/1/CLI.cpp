#include <iostream>
#include <string>
#include "1/Evaluator.hpp"

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "EXIT") break;
        if (line.substr(0, 4) == "EVAL") {
            try {
                std::cout << evaluateExpression(line.substr(5)) << std::endl;
            } catch (...) {
                std::cout << "Error" << std::endl;
            }
        }
    }
    return 0;
}