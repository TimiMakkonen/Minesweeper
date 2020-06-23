#include <fstream> // std::ifstream
#include <sstream> // std::stringstream

#include <gtest/gtest.h>

#include <test_config.h>

#include <minesweeper/game.h>
#include <minesweeper/random.h>

class MinesweeperTest : public ::testing::Test {
  protected:
    // used for setting up fields for testing
    void SetUp() override { setSerialisationStrings(); }

    void setSerialisationStrings() {

        startedFiveBySixGameStr =
            fileContentsToString(minesweeper::TEST_DATA[minesweeper::startedGame_fiveBySix_serialisation____json]);

        startedSixByEightGameStr =
            fileContentsToString(minesweeper::TEST_DATA[minesweeper::startedGame_sixByEight_serialisation____json]);

        markedWinFiveByFourGameStr =
            fileContentsToString(minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json]);

        lossFiveByThreeGameStr =
            fileContentsToString(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json]);
    }

    // declarations:
    minesweeper::Random myRandom;
    std::string startedFiveBySixGameStr;
    std::string startedSixByEightGameStr;
    std::string markedWinFiveByFourGameStr;
    std::string lossFiveByThreeGameStr;

    // utility methods
    std::string fileContentsToString(const std::string& filePath) const {

        std::ifstream file(filePath);
        std::ostringstream ss;
        ss << file.rdbuf();

        return ss.str();
    }

    void deserialiseFromFile(const std::string& filePath, minesweeper::Game& game) const {

        std::ifstream fileStream(filePath);
        game.deserialise(fileStream);
    }

    std::string serialiseToString(minesweeper::Game& game) const {

        std::ostringstream oss;
        game.serialise(oss);

        return oss.str();
    }

    // print functionality just for
    std::string printGameToString(minesweeper::Game& gameToPrint) const {

        std::string gamePrint = "Here is the current visibility of the grid:\n\n";
        const char MINESYMBOL = '*';
        const char NO_MINES_AROUND_SYMBOL = '.';
        const char NOT_VISIBLE_SYMBOL = ' ';
        const char MARKED_SYMBOL = '#';

        for (int y = 0; y < gameToPrint.getGridHeight(); ++y) {
            for (int x = 0; x < gameToPrint.getGridWidth(); ++x) {
                if (gameToPrint.isCellMarked(x, y)) {
                    gamePrint += MARKED_SYMBOL;
                } else if (!gameToPrint.isCellVisible(x, y)) {
                    gamePrint += NOT_VISIBLE_SYMBOL;
                } else if (gameToPrint.doesCellHaveMine(x, y)) {
                    gamePrint += MINESYMBOL;
                } else if (gameToPrint.numOfMinesAroundCell(x, y) == 0) {
                    gamePrint += NO_MINES_AROUND_SYMBOL;
                } else {
                    gamePrint += std::to_string(gameToPrint.numOfMinesAroundCell(x, y));
                }
            }
            gamePrint += '\n';
        }

        gamePrint += "\nAnd here is the solution for the grid:\n\n";

        for (int y = 0; y < gameToPrint.getGridHeight(); ++y) {
            for (int x = 0; x < gameToPrint.getGridWidth(); ++x) {
                if (gameToPrint.doesCellHaveMine(x, y)) {
                    gamePrint += MINESYMBOL;
                } else if (gameToPrint.numOfMinesAroundCell(x, y) == 0) {
                    gamePrint += NO_MINES_AROUND_SYMBOL;
                } else {
                    gamePrint += std::to_string(gameToPrint.numOfMinesAroundCell(x, y));
                }
            }
            gamePrint += '\n';
        }

        return gamePrint;
    }

    // predetermined mines random
    class PreSetMinesRandom : public minesweeper::IRandom {
      public:
        // predetermined shuffle
        // puts class member ints to front of vector parameter
        // (at this moment first numbers of vector are the mines)
        void shuffleVector(std::vector<int>& vec) {
            auto toIt = vec.begin();
            for (auto& num : minesToSet) {

                auto fromIt = std::find(vec.begin(), vec.end(), num);

                std::iter_swap(toIt, fromIt);
                ++toIt;
            }
        }

        PreSetMinesRandom(std::vector<int> minesToSet) : minesToSet(minesToSet) {}

      private:
        const std::vector<int> minesToSet;
    };
};

using MinesweeperDeathTest = MinesweeperTest;

TEST_F(MinesweeperTest, ConstructorTest) {

    // default constructor
    minesweeper::Game myDefaultGame;
    EXPECT_EQ(myDefaultGame.getGridHeight(), 0);
    EXPECT_EQ(myDefaultGame.getGridWidth(), 0);
    EXPECT_EQ(myDefaultGame.getNumOfMines(), 0);

    // most basic/expected constructor
    minesweeper::Game myBasicGame(10, 11, 20, &myRandom);
    EXPECT_EQ(myBasicGame.getGridHeight(), 10);
    EXPECT_EQ(myBasicGame.getGridWidth(), 11);
    EXPECT_EQ(myBasicGame.getNumOfMines(), 20);

    // square grid size constructor
    minesweeper::Game mySquareGame(23, 14, &myRandom);
    EXPECT_EQ(mySquareGame.getGridHeight(), 23);
    EXPECT_EQ(mySquareGame.getGridWidth(), 23);
    EXPECT_EQ(mySquareGame.getNumOfMines(), 14);

    // basic constructor with random statically set or not set at all
    minesweeper::Game myUnsetRandomGame(8, 5, 4);
    EXPECT_EQ(myUnsetRandomGame.getGridHeight(), 8);
    EXPECT_EQ(myUnsetRandomGame.getGridWidth(), 5);
    EXPECT_EQ(myUnsetRandomGame.getNumOfMines(), 4);

    // square grid size constructor with random statically set or not set at all
    minesweeper::Game mySquareUnsetRandomGame(6, 7);
    EXPECT_EQ(mySquareUnsetRandomGame.getGridHeight(), 6);
    EXPECT_EQ(mySquareUnsetRandomGame.getGridWidth(), 6);
    EXPECT_EQ(mySquareUnsetRandomGame.getNumOfMines(), 7);
}

TEST_F(MinesweeperTest, ConstructorGridSizeTest) {

    // negative grid
    EXPECT_THROW(minesweeper::Game myNegativeGridGame(-5, -3, 4, &myRandom), std::out_of_range);

    // negative square grid
    EXPECT_THROW(minesweeper::Game myNegativeSquareGridGame(-5, 8, &myRandom), std::out_of_range);

    // zero size grid
    minesweeper::Game myZeroSizeGame(0, 0, &myRandom);
    EXPECT_EQ(myZeroSizeGame.getGridHeight(), 0);
    EXPECT_EQ(myZeroSizeGame.getGridWidth(), 0);
    EXPECT_EQ(myZeroSizeGame.getNumOfMines(), 0);

    // single cell grid
    minesweeper::Game mySingleCellGame(1, 0, &myRandom);
    EXPECT_EQ(mySingleCellGame.getGridHeight(), 1);
    EXPECT_EQ(mySingleCellGame.getGridWidth(), 1);
    EXPECT_EQ(mySingleCellGame.getNumOfMines(), 0);

    // 2x2 grid
    minesweeper::Game myTwoByTwoGame(2, 0, &myRandom);
    EXPECT_EQ(myTwoByTwoGame.getGridHeight(), 2);
    EXPECT_EQ(myTwoByTwoGame.getGridWidth(), 2);
    EXPECT_EQ(myTwoByTwoGame.getNumOfMines(), 0);

    // normal grid
    minesweeper::Game myNormalGame(6, 7, 14, &myRandom);
    EXPECT_EQ(myNormalGame.getGridHeight(), 6);
    EXPECT_EQ(myNormalGame.getGridWidth(), 7);
    EXPECT_EQ(myNormalGame.getNumOfMines(), 14);

    // large grid
    minesweeper::Game myLargeGame(100, 110, &myRandom);
    EXPECT_EQ(myLargeGame.getGridHeight(), 100);
    EXPECT_EQ(myLargeGame.getGridWidth(), 100);
    EXPECT_EQ(myLargeGame.getNumOfMines(), 110);

    // very large grid
    minesweeper::Game myVeryLargeGame(1'000, 1'500, 200'000, &myRandom);
    EXPECT_EQ(myVeryLargeGame.getGridHeight(), 1'000);
    EXPECT_EQ(myVeryLargeGame.getGridWidth(), 1'500);
    EXPECT_EQ(myVeryLargeGame.getNumOfMines(), 200'000);
}

TEST_F(MinesweeperTest, ConstructorMineTest) {

    // negative number of mines
    EXPECT_THROW(minesweeper::Game myNegativeMinesGame(5, -5, &myRandom), std::out_of_range);

    // zero mines
    minesweeper::Game myZeroMinesGame(6, 0, &myRandom);
    EXPECT_EQ(myZeroMinesGame.getNumOfMines(), 0);

    // max number of mines
    minesweeper::Game myMaxMineGame(5, 16, &myRandom);
    EXPECT_EQ(myMaxMineGame.getNumOfMines(), 16);

    // slightly too many mines
    EXPECT_THROW(minesweeper::Game mySlightTooManyMinesGame(7, 41, &myRandom), std::out_of_range);

    // too many mines
    EXPECT_THROW(minesweeper::Game myTooManyMinesGame(6, 40, &myRandom), std::out_of_range);
}

TEST_F(MinesweeperTest, CreateMinesAndNumsTest) {

    // negative chosen x and y for 'createMinesAndNums'
    minesweeper::Game negativeXandYCreateMinesAndNumsGame(7, 20, &myRandom);
    EXPECT_THROW(negativeXandYCreateMinesAndNumsGame.createMinesAndNums(-5, -1), std::out_of_range);

    // negative chosen x for 'createMinesAndNums'
    minesweeper::Game negativeXCreateMinesAndNumsGame(5, 6, 18, &myRandom);
    EXPECT_THROW(negativeXCreateMinesAndNumsGame.createMinesAndNums(-2, 3), std::out_of_range);

    // negative chosen y for 'createMinesAndNums'
    minesweeper::Game negativeYCreateMinesAndNumsGame(8, 30, &myRandom);
    EXPECT_THROW(negativeYCreateMinesAndNumsGame.createMinesAndNums(0, -11), std::out_of_range);

    // too large chosen x and y for 'createMinesAndNums'
    minesweeper::Game tooLargeXandYCreateMinesAndNumsGame(13, 60, &myRandom);
    EXPECT_THROW(tooLargeXandYCreateMinesAndNumsGame.createMinesAndNums(13, 20), std::out_of_range);

    // too large chosen x for 'createMinesAndNums'
    minesweeper::Game tooLargeXCreateMinesAndNumsGame(9, 14, 48, &myRandom);
    EXPECT_THROW(tooLargeXCreateMinesAndNumsGame.createMinesAndNums(16, 5), std::out_of_range);

    // too large chosen y for 'createMinesAndNums'
    minesweeper::Game tooLargeYCreateMinesAndNumsGame(10, 8, 30, &myRandom);
    EXPECT_THROW(tooLargeYCreateMinesAndNumsGame.createMinesAndNums(0, 10), std::out_of_range);

    // too large chosen x and negative y for 'createMinesAndNums'
    minesweeper::Game tooLargeXandNegativeYCreateMinesAndNumsGame(10, 23, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYCreateMinesAndNumsGame.createMinesAndNums(13, -2), std::out_of_range);

    // valid chosen x and y for 'createMinesAndNums'
    minesweeper::Game validCreateMinesAndNumsGame(8, 9, 18, &myRandom);
    EXPECT_NO_THROW(validCreateMinesAndNumsGame.createMinesAndNums(5, 3));

    // borderline valid chosen x and y for 'createMinesAndNums'
    minesweeper::Game borderlineValidOnCreateMinesAndNumsGame(7, 10, 18, &myRandom);
    EXPECT_NO_THROW(borderlineValidOnCreateMinesAndNumsGame.createMinesAndNums(9, 0));

    // calling already set 'createMinesAndNums'
    minesweeper::Game alreadySetCreateMinesAndNumsGame(11, 25, &myRandom);
    EXPECT_NO_THROW(alreadySetCreateMinesAndNumsGame.createMinesAndNums(8, 2));
    EXPECT_THROW(alreadySetCreateMinesAndNumsGame.createMinesAndNums(2, 10), std::invalid_argument);

    // calling already set (by check) 'createMinesAndNums'
    minesweeper::Game alreadySetByCheckCreateMinesAndNumsGame(11, 16, 25, &myRandom);
    EXPECT_NO_THROW(alreadySetByCheckCreateMinesAndNumsGame.checkInputCoordinates(9, 5));
    EXPECT_THROW(alreadySetByCheckCreateMinesAndNumsGame.createMinesAndNums(12, 6), std::invalid_argument);
}

TEST_F(MinesweeperTest, ResetTest) {

    // unstarted game should not change after reset
    minesweeper::Game unstartedGame(12, 9, 40, &myRandom);
    std::string unstartedGameSerialisationStr = serialiseToString(unstartedGame);
    unstartedGame.reset(true);
    std::string unstartedGameAfterResetSerialisationStr = serialiseToString(unstartedGame);
    unstartedGame.reset(false);
    std::string unstartedGameAfterKeepMinesResetSerialisationStr = serialiseToString(unstartedGame);
    EXPECT_EQ(unstartedGameSerialisationStr, unstartedGameAfterResetSerialisationStr);
    EXPECT_EQ(unstartedGameSerialisationStr, unstartedGameAfterKeepMinesResetSerialisationStr);

    // +---------------------------------+
    // | reset after simple started game |
    // +---------------------------------+

    // premade started 6x8 game, remove mines reset
    minesweeper::Game premadeRemoveMinesGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_sixByEight_serialisation____json], premadeRemoveMinesGame));
    EXPECT_NE(startedSixByEightGameStr, "");
    // does cell have mine
    EXPECT_TRUE(premadeRemoveMinesGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeRemoveMinesGame.doesCellHaveMine(3, 5));
    // is cell visible
    EXPECT_FALSE(premadeRemoveMinesGame.isCellVisible(1, 5));
    EXPECT_TRUE(premadeRemoveMinesGame.isCellVisible(5, 3));
    // num of mines around cell
    EXPECT_EQ(premadeRemoveMinesGame.numOfMinesAroundCell(3, 2), 2);
    EXPECT_EQ(premadeRemoveMinesGame.numOfMinesAroundCell(7, 3), 1);
    // game fields
    EXPECT_EQ(premadeRemoveMinesGame.getGridHeight(), 6);
    EXPECT_EQ(premadeRemoveMinesGame.getGridWidth(), 8);
    EXPECT_EQ(premadeRemoveMinesGame.getNumOfMines(), 15);
    // RESET
    premadeRemoveMinesGame.reset(false);
    // serialisation comparisons
    std::string premadeRemoveMinesGameStr = serialiseToString(premadeRemoveMinesGame);
    EXPECT_NE(premadeRemoveMinesGameStr, "");
    EXPECT_NE(premadeRemoveMinesGameStr, startedSixByEightGameStr);
    // does cell have mine
    EXPECT_FALSE(premadeRemoveMinesGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeRemoveMinesGame.doesCellHaveMine(3, 5));
    // is cell visible
    EXPECT_FALSE(premadeRemoveMinesGame.isCellVisible(1, 5));
    EXPECT_FALSE(premadeRemoveMinesGame.isCellVisible(5, 3));
    // num of mines around cell
    EXPECT_EQ(premadeRemoveMinesGame.numOfMinesAroundCell(3, 2), 0);
    EXPECT_EQ(premadeRemoveMinesGame.numOfMinesAroundCell(7, 3), 0);
    // game fields
    EXPECT_EQ(premadeRemoveMinesGame.getGridHeight(), 6);
    EXPECT_EQ(premadeRemoveMinesGame.getGridWidth(), 8);
    EXPECT_EQ(premadeRemoveMinesGame.getNumOfMines(), 15);

    // same premade started 6x8 game, keep mines reset
    minesweeper::Game premadeKeepMinesGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_sixByEight_serialisation____json], premadeKeepMinesGame));
    // RESET
    premadeKeepMinesGame.reset(true);
    // serialisation comparisons
    std::string premadeKeepMinesGameStr = serialiseToString(premadeKeepMinesGame);
    EXPECT_NE(premadeKeepMinesGameStr, "");
    EXPECT_NE(premadeKeepMinesGameStr, startedSixByEightGameStr);
    EXPECT_NE(premadeKeepMinesGameStr, premadeRemoveMinesGameStr);
    // does cell have mine
    EXPECT_TRUE(premadeKeepMinesGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeKeepMinesGame.doesCellHaveMine(3, 5));
    // is cell visible
    EXPECT_FALSE(premadeKeepMinesGame.isCellVisible(1, 5));
    EXPECT_FALSE(premadeKeepMinesGame.isCellVisible(5, 3));
    // num of mines around cell
    EXPECT_EQ(premadeKeepMinesGame.numOfMinesAroundCell(3, 2), 2);
    EXPECT_EQ(premadeKeepMinesGame.numOfMinesAroundCell(7, 3), 1);
    EXPECT_EQ(premadeKeepMinesGame.getGridHeight(), 6);
    EXPECT_EQ(premadeKeepMinesGame.getGridWidth(), 8);
    EXPECT_EQ(premadeKeepMinesGame.getNumOfMines(), 15);

    // +------------------------------+
    // | reset after already won game |
    // +------------------------------+

    // premade 5x4 win game, remove mines reset
    minesweeper::Game premadeRemoveMinesAfterWinGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json],
                            premadeRemoveMinesAfterWinGame));
    EXPECT_NE(markedWinFiveByFourGameStr, "");
    // does cell have mine
    EXPECT_TRUE(premadeRemoveMinesAfterWinGame.doesCellHaveMine(1, 1));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.doesCellHaveMine(2, 4));
    // is cell visible
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellVisible(2, 0));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellVisible(0, 4));
    EXPECT_TRUE(premadeRemoveMinesAfterWinGame.isCellVisible(1, 3));
    // is cell marked
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellMarked(1, 2));
    EXPECT_TRUE(premadeRemoveMinesAfterWinGame.isCellMarked(0, 0));
    // num of mines around cell
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.numOfMinesAroundCell(2, 1), 3);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.numOfMinesAroundCell(0, 2), 1);
    // game fields
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getGridHeight(), 5);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getGridWidth(), 4);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_TRUE(premadeRemoveMinesAfterWinGame.playerHasWon());
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.playerHasLost());
    // RESET
    premadeRemoveMinesAfterWinGame.reset(false);
    // serialisation comparisons
    std::string premadeRemoveMinesAfterWinGameStr = serialiseToString(premadeRemoveMinesAfterWinGame);
    EXPECT_NE(premadeRemoveMinesAfterWinGameStr, "");
    EXPECT_NE(premadeRemoveMinesAfterWinGameStr, markedWinFiveByFourGameStr);
    // does cell have mine
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.doesCellHaveMine(1, 1));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.doesCellHaveMine(2, 4));
    // is cell visible
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellVisible(2, 0));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellVisible(0, 4));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellVisible(1, 3));
    // is cell marked
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellMarked(1, 2));
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.isCellMarked(0, 0));
    // num of mines around cell
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.numOfMinesAroundCell(2, 1), 0);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.numOfMinesAroundCell(0, 2), 0);
    // game fields
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getGridHeight(), 5);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getGridWidth(), 4);
    EXPECT_EQ(premadeRemoveMinesAfterWinGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.playerHasWon());
    EXPECT_FALSE(premadeRemoveMinesAfterWinGame.playerHasLost());

    // premade 5x4 win game, keep mines reset
    minesweeper::Game premadeKeepMinesAfterWinGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json],
                            premadeKeepMinesAfterWinGame));
    // RESET
    premadeKeepMinesAfterWinGame.reset(true);
    // serialisation comparisons
    std::string premadeKeepMinesAfterWinGameStr = serialiseToString(premadeKeepMinesAfterWinGame);
    EXPECT_NE(premadeKeepMinesAfterWinGameStr, "");
    EXPECT_NE(premadeKeepMinesAfterWinGameStr, markedWinFiveByFourGameStr);
    EXPECT_NE(premadeKeepMinesAfterWinGameStr, premadeRemoveMinesAfterWinGameStr);
    // does cell have mine
    EXPECT_TRUE(premadeKeepMinesAfterWinGame.doesCellHaveMine(1, 1));
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.doesCellHaveMine(2, 4));
    // is cell visible
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.isCellVisible(2, 0));
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.isCellVisible(0, 4));
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.isCellVisible(1, 3));
    // is cell marked
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.isCellMarked(1, 2));
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.isCellMarked(0, 0));
    // num of mines around cell
    EXPECT_EQ(premadeKeepMinesAfterWinGame.numOfMinesAroundCell(2, 1), 3);
    EXPECT_EQ(premadeKeepMinesAfterWinGame.numOfMinesAroundCell(0, 2), 1);
    // game fields
    EXPECT_EQ(premadeKeepMinesAfterWinGame.getGridHeight(), 5);
    EXPECT_EQ(premadeKeepMinesAfterWinGame.getGridWidth(), 4);
    EXPECT_EQ(premadeKeepMinesAfterWinGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.playerHasWon());
    EXPECT_FALSE(premadeKeepMinesAfterWinGame.playerHasLost());

    // +-----------------------+
    // | reset after lost game |
    // +-----------------------+

    // premade 5x3 loss game, keep mines reset
    minesweeper::Game premadeKeepMinesAfterLossGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeKeepMinesAfterLossGame));
    EXPECT_NE(lossFiveByThreeGameStr, "");
    // does cell have mine
    EXPECT_TRUE(premadeKeepMinesAfterLossGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.doesCellHaveMine(1, 2));
    // is cell visible
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellVisible(1, 4));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellVisible(0, 1));
    EXPECT_TRUE(premadeKeepMinesAfterLossGame.isCellVisible(2, 3));
    // is cell marked
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellMarked(1, 0));
    EXPECT_TRUE(premadeKeepMinesAfterLossGame.isCellMarked(2, 2));
    // num of mines around cell
    EXPECT_EQ(premadeKeepMinesAfterLossGame.numOfMinesAroundCell(2, 3), 3);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.numOfMinesAroundCell(0, 0), 1);
    // game fields
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getGridHeight(), 5);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getGridWidth(), 3);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.playerHasWon());
    EXPECT_TRUE(premadeKeepMinesAfterLossGame.playerHasLost());
    // RESET
    premadeKeepMinesAfterLossGame.reset(true);
    // serialisation comparisons
    std::string premadeKeepMinesAfterLossGameStr = serialiseToString(premadeKeepMinesAfterLossGame);
    EXPECT_NE(premadeKeepMinesAfterLossGameStr, "");
    EXPECT_NE(premadeKeepMinesAfterLossGameStr, lossFiveByThreeGameStr);
    // does cell have mine
    EXPECT_TRUE(premadeKeepMinesAfterLossGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.doesCellHaveMine(1, 2));
    // is cell visible
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellVisible(1, 4));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellVisible(0, 1));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellVisible(2, 3));
    // is cell marked
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellMarked(1, 0));
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.isCellMarked(2, 2));
    // num of mines around cell
    EXPECT_EQ(premadeKeepMinesAfterLossGame.numOfMinesAroundCell(2, 3), 3);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.numOfMinesAroundCell(0, 0), 1);
    // game fields
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getGridHeight(), 5);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getGridWidth(), 3);
    EXPECT_EQ(premadeKeepMinesAfterLossGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.playerHasWon());
    EXPECT_FALSE(premadeKeepMinesAfterLossGame.playerHasLost());

    // premade 5x3 loss game, remove mines reset
    minesweeper::Game premadeRemoveMinesAfterLossGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeRemoveMinesAfterLossGame));
    // RESET
    premadeRemoveMinesAfterLossGame.reset(false);
    // serialisation comparisons
    std::string premadeRemoveMinesAfterLossGameStr = serialiseToString(premadeRemoveMinesAfterLossGame);
    EXPECT_NE(premadeRemoveMinesAfterLossGameStr, "");
    EXPECT_NE(premadeRemoveMinesAfterLossGameStr, lossFiveByThreeGameStr);
    EXPECT_NE(premadeRemoveMinesAfterLossGameStr, premadeKeepMinesAfterLossGameStr);
    // does cell have mine
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.doesCellHaveMine(1, 2));
    // is cell visible
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.isCellVisible(1, 4));
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.isCellVisible(0, 1));
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.isCellVisible(2, 3));
    // is cell marked
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.isCellMarked(1, 0));
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.isCellMarked(2, 2));
    // num of mines around cell
    EXPECT_EQ(premadeRemoveMinesAfterLossGame.numOfMinesAroundCell(2, 3), 0);
    EXPECT_EQ(premadeRemoveMinesAfterLossGame.numOfMinesAroundCell(0, 0), 0);
    // game fields
    EXPECT_EQ(premadeRemoveMinesAfterLossGame.getGridHeight(), 5);
    EXPECT_EQ(premadeRemoveMinesAfterLossGame.getGridWidth(), 3);
    EXPECT_EQ(premadeRemoveMinesAfterLossGame.getNumOfMines(), 6);
    // has player lost or won
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.playerHasWon());
    EXPECT_FALSE(premadeRemoveMinesAfterLossGame.playerHasLost());
}

TEST_F(MinesweeperTest, NewGameTest) { // TODO

    minesweeper::Game myGame(10, 20, &myRandom);
}

TEST_F(MinesweeperTest, CheckInputCoordinatesTest) {

    // negative x and y
    minesweeper::Game negativeXandYCheckInputCoordsGame(11, 40, &myRandom);
    EXPECT_THROW(negativeXandYCheckInputCoordsGame.checkInputCoordinates(-5, -8), std::out_of_range);

    // negative x
    minesweeper::Game negativeXCheckInputCoordsGame(15, 10, 39, &myRandom);
    EXPECT_THROW(negativeXCheckInputCoordsGame.checkInputCoordinates(-2, 8), std::out_of_range);

    // negative y
    minesweeper::Game negativeYCheckInputCoordsGame(18, 40, &myRandom);
    EXPECT_THROW(negativeYCheckInputCoordsGame.checkInputCoordinates(9, -10), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYCheckInputCoordsGame(150, 6'000, &myRandom);
    EXPECT_THROW(tooLargeXandYCheckInputCoordsGame.checkInputCoordinates(160, 200), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXCheckInputCoordsGame(45, 16, 711, &myRandom);
    EXPECT_THROW(tooLargeXCheckInputCoordsGame.checkInputCoordinates(16, 0), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYCheckInputCoordsGame(35, 358, &myRandom);
    EXPECT_THROW(tooLargeYCheckInputCoordsGame.checkInputCoordinates(12, 182), std::out_of_range);

    // too large x and negative y
    minesweeper::Game tooLargeXandNegativeYCheckInputCoordsGame(18, 24, 94, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYCheckInputCoordsGame.checkInputCoordinates(30, -9), std::out_of_range);

    // valid checks
    minesweeper::Game validCheckInputCoordsGame(28, 14, 380, &myRandom);
    EXPECT_FALSE(validCheckInputCoordsGame.isCellVisible(5, 11));
    EXPECT_NO_THROW(validCheckInputCoordsGame.checkInputCoordinates(5, 11));
    EXPECT_TRUE(validCheckInputCoordsGame.isCellVisible(5, 11));
    EXPECT_FALSE(validCheckInputCoordsGame.isCellVisible(9, 20));
    EXPECT_NO_THROW(validCheckInputCoordsGame.checkInputCoordinates(9, 20));
    EXPECT_TRUE(validCheckInputCoordsGame.isCellVisible(9, 20));
}

TEST_F(MinesweeperTest, MarkInputCoordinatesTest) {

    // negative x and y
    minesweeper::Game negativeXandYMarkInputCoordsGame(10, 23, &myRandom);
    negativeXandYMarkInputCoordsGame.checkInputCoordinates(1, 8);
    EXPECT_THROW(negativeXandYMarkInputCoordsGame.markInputCoordinates(-2, -3), std::out_of_range);

    // negative x
    minesweeper::Game negativeXMarkInputCoordsGame(9, 6, 15, &myRandom);
    negativeXMarkInputCoordsGame.checkInputCoordinates(1, 8);
    EXPECT_THROW(negativeXMarkInputCoordsGame.markInputCoordinates(-4, 3), std::out_of_range);

    // negative y
    minesweeper::Game negativeYMarkInputCoordsGame(5, 8, 14, &myRandom);
    negativeYMarkInputCoordsGame.checkInputCoordinates(1, 0);
    EXPECT_THROW(negativeYMarkInputCoordsGame.markInputCoordinates(3, -1), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYMarkInputCoordsGame(9, 30, &myRandom);
    tooLargeXandYMarkInputCoordsGame.checkInputCoordinates(8, 7);
    EXPECT_THROW(tooLargeXandYMarkInputCoordsGame.markInputCoordinates(10, 15), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXMarkInputCoordsGame(16, 70, &myRandom);
    tooLargeXMarkInputCoordsGame.checkInputCoordinates(14, 10);
    EXPECT_THROW(tooLargeXMarkInputCoordsGame.markInputCoordinates(35, 0), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYMarkInputCoordsGame(50, 500, &myRandom);
    tooLargeYMarkInputCoordsGame.checkInputCoordinates(35, 41);
    EXPECT_THROW(tooLargeYMarkInputCoordsGame.markInputCoordinates(16, 50), std::out_of_range);

    // negative x and too large y
    minesweeper::Game negativeXandTooLargeYMarkInputCoordsGame(45, 80, 600, &myRandom);
    negativeXandTooLargeYMarkInputCoordsGame.checkInputCoordinates(50, 30);
    EXPECT_THROW(negativeXandTooLargeYMarkInputCoordsGame.markInputCoordinates(-14, 90), std::out_of_range);

    // valid check set mark
    minesweeper::Game validMarkInputCoordsGame(11, 13, 30, &myRandom);
    validMarkInputCoordsGame.checkInputCoordinates(5, 7);
    EXPECT_FALSE(validMarkInputCoordsGame.isCellMarked(8, 2));
    EXPECT_NO_THROW(validMarkInputCoordsGame.markInputCoordinates(8, 2));
    EXPECT_TRUE(validMarkInputCoordsGame.isCellMarked(8, 2));

    // valid 'createMinesAndNums' set mark
    minesweeper::Game validCreateMinesAndNumsYMarkInputCoordsGame(47, 35, 700, &myRandom);
    validCreateMinesAndNumsYMarkInputCoordsGame.createMinesAndNums(25, 35);
    EXPECT_FALSE(validCreateMinesAndNumsYMarkInputCoordsGame.isCellMarked(30, 42));
    EXPECT_NO_THROW(validCreateMinesAndNumsYMarkInputCoordsGame.markInputCoordinates(30, 42));
    EXPECT_TRUE(validCreateMinesAndNumsYMarkInputCoordsGame.isCellMarked(30, 42));

    // not set mark
    minesweeper::Game notSetMinesMarkInputCoordsGame(110, 3000, &myRandom);
    EXPECT_THROW(notSetMinesMarkInputCoordsGame.markInputCoordinates(50, 68), std::invalid_argument);

    // unmark
    minesweeper::Game UnmarkMarkInputCoordsGame(15, 32, 105, &myRandom);
    UnmarkMarkInputCoordsGame.checkInputCoordinates(20, 7);
    EXPECT_FALSE(UnmarkMarkInputCoordsGame.isCellMarked(18, 10));
    EXPECT_NO_THROW(UnmarkMarkInputCoordsGame.markInputCoordinates(18, 10));
    EXPECT_TRUE(UnmarkMarkInputCoordsGame.isCellMarked(18, 10));
    EXPECT_NO_THROW(UnmarkMarkInputCoordsGame.markInputCoordinates(18, 10));
    EXPECT_FALSE(UnmarkMarkInputCoordsGame.isCellMarked(18, 10));
}

TEST_F(MinesweeperTest, PlayerHasWonTest) {

    // empty win
    minesweeper::Game emptyWinGame(0, 0, &myRandom);
    EXPECT_TRUE(emptyWinGame.playerHasWon());

    // mineless win
    minesweeper::Game minelessWinGame(10, 0, &myRandom);
    EXPECT_TRUE(minelessWinGame.playerHasWon());

    // mineless checked win
    minesweeper::Game minelessCheckedWinGame(9, 0, &myRandom);
    minelessCheckedWinGame.checkInputCoordinates(4, 5);
    EXPECT_TRUE(minelessCheckedWinGame.playerHasWon());

    // easy win
    minesweeper::Game easyWinGame(3, 4, 3, &myRandom);
    EXPECT_FALSE(easyWinGame.playerHasWon());
    easyWinGame.checkInputCoordinates(1, 1);
    EXPECT_TRUE(easyWinGame.playerHasWon());

    // repeated non-trivial win
    // (consider replacing this relatively slow random method with premade grids when deserialization is available)
    bool allNonTrivialCasesExplored = false;
    do {
        int countOfNonTrivialEasyWins = 0;
        int countOfNonTrivialCase1Wins = 0;
        int countOfNonTrivialCase2Wins = 0;
        int countOfNonTrivialCase3Wins = 0;
        int countOfNonTrivialCase4Wins = 0;
        for (int i = 0; i < 100; ++i) {
            minesweeper::Game nonTrivialWinGame(4, 4, 1, &myRandom);
            EXPECT_FALSE(nonTrivialWinGame.playerHasWon());
            nonTrivialWinGame.checkInputCoordinates(2, 1);

            // at this point we have already won or mine is in: (x, y)
            // (0, 1), (0, 2), (1, 3), or (2, 3)

            // also if we have not won, we have one invisible number '1' in: (x, y)
            // (0, 0), (0, 3), or (3, 3); depending on where the mine is located

            // with this information and overall studying of all possible outcomes,
            // we can come up with this minesweeper solving algorithm:
            // (this algorithm uses checking or marking to make the final move depending if 'i' is even or not)
            if (nonTrivialWinGame.playerHasWon()) {
                ++countOfNonTrivialEasyWins;
                continue;
            } else {
                if (nonTrivialWinGame.numOfMinesAroundCell(1, 0) == 1) {
                    ++countOfNonTrivialCase1Wins;

                    if (i % 2 == 0) {
                        nonTrivialWinGame.checkInputCoordinates(0, 0);
                    } else {
                        nonTrivialWinGame.markInputCoordinates(0, 1);
                    }
                    EXPECT_TRUE(nonTrivialWinGame.playerHasWon());

                } else if (nonTrivialWinGame.numOfMinesAroundCell(1, 1) == 1) {
                    ++countOfNonTrivialCase2Wins;

                    if (i % 2 == 0) {
                        nonTrivialWinGame.checkInputCoordinates(0, 3);
                    } else {
                        nonTrivialWinGame.markInputCoordinates(0, 2);
                    }
                    EXPECT_TRUE(nonTrivialWinGame.playerHasWon());

                } else if (nonTrivialWinGame.numOfMinesAroundCell(3, 2) == 1) {
                    ++countOfNonTrivialCase3Wins;

                    if (i % 2 == 0) {
                        nonTrivialWinGame.checkInputCoordinates(3, 3);
                    } else {
                        nonTrivialWinGame.markInputCoordinates(2, 3);
                    }
                    EXPECT_TRUE(nonTrivialWinGame.playerHasWon());

                } else {
                    ++countOfNonTrivialCase4Wins;

                    if (i % 2 == 0) {
                        nonTrivialWinGame.checkInputCoordinates(0, 3);
                    } else {
                        nonTrivialWinGame.markInputCoordinates(1, 3);
                    }
                    EXPECT_TRUE(nonTrivialWinGame.playerHasWon());
                }
            }
        }

        // let us check we went through each main scenario at least twice:
        // (probability of this failing because of randomness is = 0.000'188'520'2% when i == 100)
        if (countOfNonTrivialEasyWins > 1 && countOfNonTrivialCase1Wins > 1 && countOfNonTrivialCase2Wins > 1 &&
            countOfNonTrivialCase3Wins > 1 && countOfNonTrivialCase4Wins > 1) {
            allNonTrivialCasesExplored = true;
        }

        // if cases were not explored at least twice, repeat test
    } while (!allNonTrivialCasesExplored);

    // premade marked win
    minesweeper::Game premadeMarkedWinGame;
    std::ifstream premadeMarkedWinGameStream(
        minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json]);
    premadeMarkedWinGame.deserialise(premadeMarkedWinGameStream);
    EXPECT_TRUE(premadeMarkedWinGame.playerHasWon()) << printGameToString(premadeMarkedWinGame);

    // premade checked win
    minesweeper::Game premadeCheckedWinGame;
    std::ifstream premadeCheckedWinGameStream(
        minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json]);
    premadeCheckedWinGame.deserialise(premadeCheckedWinGameStream);
    EXPECT_TRUE(premadeCheckedWinGame.playerHasWon()) << printGameToString(premadeCheckedWinGame);

    // fully checked non-win
    minesweeper::Game fullyCheckedNonWinGame(9, 20, &myRandom);
    fullyCheckedNonWinGame.checkInputCoordinates(5, 7);
    for (int y = 0; y < 9; ++y) {
        for (int x = 0; x < 9; ++x) {
            fullyCheckedNonWinGame.checkInputCoordinates(x, y);
        }
    }
    EXPECT_FALSE(fullyCheckedNonWinGame.playerHasWon()) << printGameToString(fullyCheckedNonWinGame);

    // marked win from preset grid
    minesweeper::Game presetMarkToWinGame;
    std::stringstream presetMarkToWinGameStream(startedFiveBySixGameStr);
    presetMarkToWinGame.deserialise(presetMarkToWinGameStream);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(0, 1);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(4, 1);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(4, 2);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(5, 2);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(4, 4);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(3, 4);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(1, 3);
    EXPECT_FALSE(presetMarkToWinGame.playerHasWon());
    presetMarkToWinGame.markInputCoordinates(0, 4);
    EXPECT_TRUE(presetMarkToWinGame.playerHasWon());

    // checked win from preset grid
    minesweeper::Game presetCheckToWinGame;
    std::stringstream presetCheckToWinGameStream(startedFiveBySixGameStr);
    presetCheckToWinGame.deserialise(presetCheckToWinGameStream);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(0, 0);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(0, 2);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(0, 3);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(1, 4);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(2, 3);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(2, 4);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(3, 3);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(4, 0);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(4, 3);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(5, 0);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(5, 1);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(5, 3);
    EXPECT_FALSE(presetCheckToWinGame.playerHasWon());
    presetCheckToWinGame.checkInputCoordinates(5, 4);
    EXPECT_TRUE(presetCheckToWinGame.playerHasWon());
}

TEST_F(MinesweeperTest, PlayerHasLostTest) {

    // unstarted game
    minesweeper::Game unstartedGame(7, 6, 10, &myRandom);
    EXPECT_FALSE(unstartedGame.playerHasLost());

    // not a first move loss tested few times
    // (losing should be impossible after only one check)
    struct gameParameters {
        int gridHeight;
        int gridWidth;
        int numOfMines;
        int initialX;
        int initialY;
    };
    gameParameters gameParameterSet[] = {
        {1, 11, 2, 7, 0},  {48, 50, 47, 28, 39}, {13, 9, 10, 8, 7},    {45, 46, 38, 24, 32}, {6, 19, 20, 15, 4},
        {8, 16, 25, 4, 3}, {2, 22, 25, 20, 1},   {45, 37, 33, 30, 27}, {44, 31, 26, 23, 2},  {4, 7, 11, 3, 1}};

    for (auto gameParas : gameParameterSet) {

        minesweeper::Game notFirstMoveLossGame(gameParas.gridHeight, gameParas.gridWidth, gameParas.numOfMines,
                                               &myRandom);

        EXPECT_FALSE(notFirstMoveLossGame.playerHasLost());
        notFirstMoveLossGame.checkInputCoordinates(gameParas.initialX, gameParas.initialY);
        EXPECT_FALSE(notFirstMoveLossGame.playerHasLost());
    }

    // simple loss game
    // (repeated 10 times to cover most random possibilities)
    for (int i = 0; i < 10; ++i) {
        minesweeper::Game simpleLossGame(4, 6, &myRandom);
        EXPECT_FALSE(simpleLossGame.playerHasLost());
        simpleLossGame.checkInputCoordinates(1, 2);
        EXPECT_FALSE(simpleLossGame.playerHasLost());
        if (simpleLossGame.numOfMinesAroundCell(1, 1) == 3) {
            simpleLossGame.checkInputCoordinates(0, 0);
        } else {
            simpleLossGame.checkInputCoordinates(3, 3);
        }
        EXPECT_TRUE(simpleLossGame.playerHasLost());
    }

    // premade loss game
    minesweeper::Game premadeLossGame;
    std::ifstream premadeLossGameStream(
        minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json]);
    premadeLossGame.deserialise(premadeLossGameStream);
    EXPECT_TRUE(premadeLossGame.playerHasLost()) << printGameToString(premadeLossGame);

    // loss game from preset
    minesweeper::Game presetLossGame;
    std::stringstream presetLossGameStream(startedFiveBySixGameStr);
    presetLossGame.deserialise(presetLossGameStream);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.checkInputCoordinates(0, 2);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.markInputCoordinates(4, 2);
    presetLossGame.checkInputCoordinates(2, 3);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.checkInputCoordinates(3, 3);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.markInputCoordinates(1, 3);
    presetLossGame.checkInputCoordinates(2, 4);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.checkInputCoordinates(1, 4);
    EXPECT_FALSE(presetLossGame.playerHasLost());
    presetLossGame.markInputCoordinates(3, 4);
    presetLossGame.checkInputCoordinates(0, 4);
    EXPECT_TRUE(presetLossGame.playerHasLost());
    presetLossGame.checkInputCoordinates(0, 3); // move after loss is allowed for now
    EXPECT_TRUE(presetLossGame.playerHasLost());
}

TEST_F(MinesweeperTest, IsCellVisibleTest) { // TODO

    // unstarted game should have no visible cells
    minesweeper::Game unstartedGame(5, 9, 20, &myRandom);
    EXPECT_FALSE(unstartedGame.isCellVisible(5, 4));
    EXPECT_FALSE(unstartedGame.isCellVisible(8, 3));
    EXPECT_FALSE(unstartedGame.isCellVisible(3, 1));
    EXPECT_FALSE(unstartedGame.isCellVisible(1, 2));
    EXPECT_FALSE(unstartedGame.isCellVisible(1, 0));
}

TEST_F(MinesweeperTest, DoesCellHaveMineTest) { // TODO

    // unstarted game should have no mines
    minesweeper::Game unstartedGame(14, 7, 28, &myRandom);
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(6, 11));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(5, 2));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(3, 10));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(4, 7));
}

TEST_F(MinesweeperTest, IsCellMarkedTest) { // TODO

    // unstarted game should have no marked cells
    minesweeper::Game unstartedGame(11, 16, 40, &myRandom);
    EXPECT_FALSE(unstartedGame.isCellMarked(9, 6));
    EXPECT_FALSE(unstartedGame.isCellMarked(13, 5));
    EXPECT_FALSE(unstartedGame.isCellMarked(11, 10));
    EXPECT_FALSE(unstartedGame.isCellMarked(2, 8));
    EXPECT_FALSE(unstartedGame.isCellMarked(0, 1));
}

TEST_F(MinesweeperTest, NumOfMinesAroundCellTest) { // TODO

    // unstarted game should have no mines around
    minesweeper::Game unstartedGame(5, 10, 13, &myRandom);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(9, 1), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(2, 2), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(3, 0), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(7, 1), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(2, 3), 0);
}

TEST_F(MinesweeperTest, SerialiseTest) {

    // empty game serialisation
    minesweeper::Game emptyGame(0, 0, &myRandom);
    std::string emptyGameStr = serialiseToString(emptyGame);
    std::string emptyGameFileStr =
        fileContentsToString(minesweeper::TEST_DATA[minesweeper::emptyGame_serialisation____json]);
    EXPECT_EQ(emptyGameStr, emptyGameFileStr);

    // simple game serialisation
    minesweeper::Game simpleGame(7, 4, 13, &myRandom);
    std::string simpleGameStr = serialiseToString(simpleGame);
    std::string simpleGameFileStr =
        fileContentsToString(minesweeper::TEST_DATA[minesweeper::unstartedGame_sevenByFour_serialisation____json]);
    EXPECT_NE(simpleGameStr, "");
    EXPECT_EQ(simpleGameStr, simpleGameFileStr);

    // another game with different constructor arguments should have the following results:
    // different height
    minesweeper::Game simpleGameWithDiffHeight(9, 4, 13, &myRandom);
    std::string simpleGameWithDiffHeightStr = serialiseToString(simpleGameWithDiffHeight);
    EXPECT_NE(simpleGameWithDiffHeightStr, "");
    EXPECT_NE(simpleGameStr, simpleGameWithDiffHeightStr);

    // different width
    minesweeper::Game simpleGameWithDiffWidth(7, 8, 13, &myRandom);
    std::string simpleGameWithDiffWidthStr = serialiseToString(simpleGameWithDiffWidth);
    EXPECT_NE(simpleGameWithDiffWidthStr, "");
    EXPECT_NE(simpleGameStr, simpleGameWithDiffWidthStr);

    // different number of mines
    minesweeper::Game simpleGameWithDiffNumOfMines(7, 4, 11, &myRandom);
    std::string simpleGameWithDiffNumOfMinesStr = serialiseToString(simpleGameWithDiffNumOfMines);
    EXPECT_NE(simpleGameWithDiffNumOfMinesStr, "");
    EXPECT_NE(simpleGameStr, simpleGameWithDiffNumOfMinesStr);

    // different random
    PreSetMinesRandom randomForSimpleGameWithDiffRandom({4, 8});
    minesweeper::Game simpleGameWithDiffRandom(7, 4, 13, &randomForSimpleGameWithDiffRandom);
    std::string simpleGameWithDiffRandomStr = serialiseToString(simpleGameWithDiffRandom);
    EXPECT_NE(simpleGameWithDiffRandomStr, "");
    EXPECT_EQ(simpleGameStr, simpleGameWithDiffRandomStr);

    // simple started game serialisation
    PreSetMinesRandom presetSimpleStartedGameRandom({6, 10, 16, 17, 19, 24, 27, 28});
    minesweeper::Game simpleStartedGame(5, 6, 8, &presetSimpleStartedGameRandom);
    simpleStartedGame.checkInputCoordinates(2, 1);
    std::string simpleStartedGameStr = serialiseToString(simpleStartedGame);
    EXPECT_EQ(simpleStartedGameStr, startedFiveBySixGameStr);
}

TEST_F(MinesweeperTest, DeserialiseTest) { // TODO: update once deserialise is more advanced

    // empty string input
    minesweeper::Game emptyInputStringGame;
    std::istringstream emptyInputStringStream("");
    EXPECT_THROW(emptyInputStringGame.deserialise(emptyInputStringStream), std::invalid_argument);

    // simple invalid json
    minesweeper::Game simpleInvalidJsonGame;
    std::istringstream simpleInvalidJsonStream("{");
    EXPECT_THROW(simpleInvalidJsonGame.deserialise(simpleInvalidJsonStream), std::invalid_argument);

    // simple invalid json 2
    minesweeper::Game simpleInvalidJsonGame2;
    std::istringstream simpleInvalidJsonStream2("\"IAmInvalid\"");
    EXPECT_THROW(simpleInvalidJsonGame2.deserialise(simpleInvalidJsonStream2), std::invalid_argument);

    // invalid json missing cell tag
    minesweeper::Game missingCellTagJsonGame;
    std::ifstream missingCellTagJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidJsonMissingCellTag____json]);
    EXPECT_THROW(missingCellTagJsonGame.deserialise(missingCellTagJsonGameStream), std::invalid_argument);

    // invalid json missing comma
    minesweeper::Game missingCommaJsonGame;
    std::ifstream missingCommaJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidJsonMissingCommaInCell____json]);
    EXPECT_THROW(missingCommaJsonGame.deserialise(missingCommaJsonGameStream), std::invalid_argument);

    // invalid json missing brace
    minesweeper::Game missingBraceJsonGame;
    std::ifstream missingBraceJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidJsonMissingBrace____json]);
    EXPECT_THROW(missingBraceJsonGame.deserialise(missingBraceJsonGameStream), std::invalid_argument);

    // invalid gridsize
    minesweeper::Game invalidGridsizeJsonGame;
    std::ifstream invalidGridsizeJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidGridsize____json]);
    EXPECT_THROW(invalidGridsizeJsonGame.deserialise(invalidGridsizeJsonGameStream), std::invalid_argument);

    // invalid magic
    minesweeper::Game invalidMagicJsonGame;
    std::ifstream invalidMagicJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidMagic____json]);
    EXPECT_THROW(invalidMagicJsonGame.deserialise(invalidMagicJsonGameStream), std::invalid_argument);

    // invalid version
    minesweeper::Game invalidVersionJsonGame;
    std::ifstream invalidVersionJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidVersion____json]);
    EXPECT_THROW(invalidVersionJsonGame.deserialise(invalidVersionJsonGameStream), std::invalid_argument);

    // invalid number of mines
    minesweeper::Game invalidNumOfMinesJsonGame;
    std::ifstream invalidNumOfMinesJsonGameStream(
        minesweeper::TEST_DATA[minesweeper::invalid____serialisation__DIR__invalidNumOfMines____json]);
    EXPECT_THROW(invalidNumOfMinesJsonGame.deserialise(invalidNumOfMinesJsonGameStream), std::out_of_range);
}

TEST_F(MinesweeperTest, SerialiseAndDeserialiseTest) {

    // +---------------------------+
    // | empty game serialisation: |
    // +---------------------------+

    minesweeper::Game emptyGame(0, 0, &myRandom);     // initialise empty game
    std::stringstream emptyGameStream;                // create a stringstream
    emptyGame.serialise(emptyGameStream);             // serialise empty game
    std::string emptyGameStr = emptyGameStream.str(); // make string version of this serialisation

    minesweeper::Game emptyGameCopy;              // make new empty game
    emptyGameCopy.deserialise(emptyGameStream);   // deserialise this new game with serialisation of the previous one
    std::ostringstream emptyGameCopyStream;       // create a new ostringstream
    emptyGameCopy.serialise(emptyGameCopyStream); // serialise new empty game
    std::string emptyGameCopyStr = emptyGameCopyStream.str(); // make string version of this new serialisation

    EXPECT_EQ(emptyGame.getGridHeight(), emptyGameCopy.getGridHeight());
    EXPECT_EQ(emptyGame.getGridWidth(), emptyGameCopy.getGridWidth());
    EXPECT_EQ(emptyGame.getNumOfMines(), emptyGameCopy.getNumOfMines());
    EXPECT_EQ(emptyGame.playerHasLost(), emptyGameCopy.playerHasLost());
    EXPECT_EQ(emptyGame.playerHasWon(), emptyGameCopy.playerHasWon());
    EXPECT_EQ(emptyGameStr, emptyGameCopyStr);

    // +-------------------------------------+
    // | simple (random) game serialisation: |
    // +-------------------------------------+

    minesweeper::Game simpleGame(15, 11, 38, &myRandom); // initialise simple game
    simpleGame.checkInputCoordinates(5, 9);              // check some coordinates to initialise the game/grid
    std::stringstream simpleGameStream;                  // create a stringstream
    simpleGame.serialise(simpleGameStream);              // serialise simple game
    std::string simpleGameStr = simpleGameStream.str();  // make string version of this serialisation

    minesweeper::Game simpleGameCopy;               // make new simple game
    simpleGameCopy.deserialise(simpleGameStream);   // deserialise this new game with serialisation of the previous one
    std::ostringstream simpleGameCopyStream;        // create a new ostringstream
    simpleGameCopy.serialise(simpleGameCopyStream); // serialise new simple game
    std::string simpleGameCopyStr = simpleGameCopyStream.str(); // make string version of this new serialisation

    EXPECT_EQ(simpleGame.getGridHeight(), simpleGameCopy.getGridHeight());
    EXPECT_EQ(simpleGame.getGridWidth(), simpleGameCopy.getGridWidth());
    EXPECT_EQ(simpleGame.getNumOfMines(), simpleGameCopy.getNumOfMines());
    EXPECT_EQ(simpleGame.playerHasLost(), simpleGameCopy.playerHasLost());
    EXPECT_EQ(simpleGame.playerHasWon(), simpleGameCopy.playerHasWon());
    EXPECT_EQ(simpleGameStr, simpleGameCopyStr);
}

TEST_F(MinesweeperTest, DefaultRandomTest) {

    // reset default static random to nullptr
    minesweeper::Game::setDefaultRandom(nullptr);

    // UNSET TEST:
    // unset default static random test
    minesweeper::Game myUnsetDefaultRandomGame(9, 19);
    EXPECT_THROW(myUnsetDefaultRandomGame.checkInputCoordinates(5, 4), std::invalid_argument);

    // PRESET TEST:
    // set default static random
    minesweeper::Game::setDefaultRandom(&myRandom);

    // preset default static random test
    minesweeper::Game myPresetDefaultRandomGame(11, 23);
    EXPECT_NO_THROW(myPresetDefaultRandomGame.checkInputCoordinates(6, 3));
    EXPECT_NO_THROW(myPresetDefaultRandomGame.checkInputCoordinates(6, 4));
    EXPECT_NO_THROW(myPresetDefaultRandomGame.checkInputCoordinates(8, 9));

    // POST-SET TEST:
    // reset default static random
    minesweeper::Game::setDefaultRandom(nullptr);

    // initialise game which does not have static random set yet
    minesweeper::Game myPostSetDefaultRandomGame(13, 25);

    // set default static random
    minesweeper::Game::setDefaultRandom(&myRandom);

    // post-set default static random test
    EXPECT_NO_THROW(myPostSetDefaultRandomGame.checkInputCoordinates(3, 6));
    EXPECT_NO_THROW(myPostSetDefaultRandomGame.checkInputCoordinates(4, 7));
    EXPECT_NO_THROW(myPostSetDefaultRandomGame.checkInputCoordinates(7, 2));

    // reset default static random (just in case)
    minesweeper::Game::setDefaultRandom(nullptr);
}

TEST(MinesweeperStaticTest, MaxNumOfMinesMethodTest) {

    // completely negative grid
    EXPECT_THROW(minesweeper::Game::maxNumOfMines(-5, -3), std::out_of_range);

    // negative height grid
    EXPECT_THROW(minesweeper::Game::maxNumOfMines(-4, 5), std::out_of_range);

    // negative width grid
    EXPECT_THROW(minesweeper::Game::maxNumOfMines(6, -7), std::out_of_range);

    // zero size grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(0, 0), 0);

    // zero height grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(0, 8), 0);

    // zero width grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(4, 0), 0);

    // 1x1 grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(1, 1), 0);

    // 2x2 grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(2, 2), 0);

    // 3x3 grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(3, 3), 0);

    // normal grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(8, 10), 71);

    // large grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(114, 129), 14'697);

    // very large grid (this test takes relatively long, consider removing)
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(10'020, 20'458), 204'989'151);
}

TEST(MinesweeperStaticTest, MinNumOfMinesMethodTest) {

    // unspecified grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(), 0);

    // completely negative grid
    EXPECT_THROW(minesweeper::Game::minNumOfMines(-5, -3), std::out_of_range);

    // negative height grid
    EXPECT_THROW(minesweeper::Game::minNumOfMines(-4, 5), std::out_of_range);

    // negative width grid
    EXPECT_THROW(minesweeper::Game::minNumOfMines(6, -7), std::out_of_range);

    // zero size grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(0, 0), 0);

    // zero height grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(0, 8), 0);

    // zero width grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(4, 0), 0);

    // 1x1 grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(1, 1), 0);

    // 2x2 grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(2, 2), 0);

    // normal grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(8, 10), 0);

    // large grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(90, 138), 0);

    // very large grid
    EXPECT_EQ(minesweeper::Game::minNumOfMines(13'520, 22'498), 0);
}
