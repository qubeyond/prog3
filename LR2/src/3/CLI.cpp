#include <iostream>
#include <string>
#include "3/Genome.hpp"

int main() {
    std::string g1, g2;
    if (!(std::cin >> g1 >> g2)) return 0;
    std::cout << Genome::solve(g1, g2) << std::endl;
    return 0;
}