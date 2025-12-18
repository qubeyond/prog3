#include <gtest/gtest.h>
#include "3/Genome.hpp"

TEST(GenomeTest, TauCetiExample) {
    EXPECT_EQ(Genome::solve("ABBACAB", "BCABB"), 4);
}

TEST(GenomeTest, NoOverlap) {
    EXPECT_EQ(Genome::solve("AAAAA", "BBBBB"), 0);
}

TEST(GenomeTest, FullOverlap) {
    EXPECT_EQ(Genome::solve("ZZZ", "ZZZ"), 2);
}