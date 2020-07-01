#include <vector> // std::vector

#include <gtest/gtest.h>

#include <minesweeper/random.h>

TEST(MinesweeperRandomTest, ShuffleVectorTest) { // TODO

    minesweeper::Random myRandom;

    // empty vector
    std::vector<int> emptyVec;
    EXPECT_NO_THROW(myRandom.shuffleVector(emptyVec));

    // normal positive vector
    std::vector<int> normalPosVec{18, 51, 561, 1, 16, 156, 65, 864, 1, 186, 1, 186, 61, 68, 11, 68, 81};
    EXPECT_NO_THROW(myRandom.shuffleVector(normalPosVec));

    // normal vector
    std::vector<int> normalVec{981, -681, 81,  -15,  -846, 841, -8, -4984, 6, 861,
                               84,  -684, 684, 8541, -12,  13,  3,  -1,    1, 74};
    EXPECT_NO_THROW(myRandom.shuffleVector(normalVec));

    // uniform vector
    std::vector<int> uniformVec(86, 13);
    auto uniformVecCopy(uniformVec);
    EXPECT_NO_THROW(myRandom.shuffleVector(uniformVec));
    EXPECT_EQ(uniformVec, uniformVecCopy);
}