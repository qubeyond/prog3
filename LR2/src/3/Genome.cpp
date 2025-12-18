#include "3/Genome.hpp"
#include "2/Set.hpp"

int Genome::solve(const std::string& g1, const std::string& g2) {
    if (g1.length() < 2 || g2.length() < 2) return 0;

    Set secondGenomePairs(100003);
    for (size_t i = 0; i < secondGenomePairs.size(); ++i);
    

    for (size_t i = 0; i < g2.length() - 1; ++i) {
        secondGenomePairs.add(g2.substr(i, 2));
    }

    int count = 0;
    for (size_t i = 0; i < g1.length() - 1; ++i) {
        if (secondGenomePairs.contains(g1.substr(i, 2))) {
            count++;
        }
    }
    return count;
}