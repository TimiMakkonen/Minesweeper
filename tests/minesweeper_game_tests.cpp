#include <fstream> // std::ifstream
#include <list>    // std::list
#include <sstream> // std::stringstream
#include <string>  // std::string
#include <vector>  // std::vector

#include <gtest/gtest.h>

#include <test_config.h>

#include <minesweeper/game.h>
#include <minesweeper/random.h>
#include <minesweeper/visual_minesweeper_cell.h>

class MinesweeperGameTest : public ::testing::Test {
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

        checkedWinFourByEightGameStr =
            fileContentsToString(minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json]);

        unfinishedSevenBySevenGameStr = fileContentsToString(
            minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json]);

        invalidMarkAndLossNineByFiveGameStr = fileContentsToString(
            minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json]);
    }

    // declarations:
    minesweeper::Random myRandom;
    std::string startedFiveBySixGameStr;
    std::string startedSixByEightGameStr;
    std::string markedWinFiveByFourGameStr;
    std::string lossFiveByThreeGameStr;
    std::string checkedWinFourByEightGameStr;
    std::string unfinishedSevenBySevenGameStr;
    std::string invalidMarkAndLossNineByFiveGameStr;

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

// using MinesweeperGameDeathTest = MinesweeperGameTest;

TEST_F(MinesweeperGameTest, ConstructorTest) {

    // default constructor
    minesweeper::Game myDefaultGame;
    EXPECT_EQ(myDefaultGame.getGridHeight(), 0);
    EXPECT_EQ(myDefaultGame.getGridWidth(), 0);
    EXPECT_EQ(myDefaultGame.getNumOfMines(), 0);

    // +---------------------------------+
    // | constructors with random field: |
    // +---------------------------------+

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

    // basic/expected (proportional mines) constructor
    minesweeper::Game myBasicPropMinesGame(15, 21, 0.37, &myRandom);
    EXPECT_EQ(myBasicPropMinesGame.getGridHeight(), 15);
    EXPECT_EQ(myBasicPropMinesGame.getGridWidth(), 21);
    EXPECT_EQ(myBasicPropMinesGame.getNumOfMines(), 116);

    // square grid size (proportional mines) constructor
    minesweeper::Game mySquarePropMinesGame(19, 0.48, &myRandom);
    EXPECT_EQ(mySquarePropMinesGame.getGridHeight(), 19);
    EXPECT_EQ(mySquarePropMinesGame.getGridWidth(), 19);
    EXPECT_EQ(mySquarePropMinesGame.getNumOfMines(), 173);

    // +----------------------------------+
    // | constructors with static random: |
    // +----------------------------------+

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

    // basic (proportional mines) constructor with random statically set or not set at all
    minesweeper::Game myUnsetRandomPropMinesGame(10, 8, 0.19);
    EXPECT_EQ(myUnsetRandomPropMinesGame.getGridHeight(), 10);
    EXPECT_EQ(myUnsetRandomPropMinesGame.getGridWidth(), 8);
    EXPECT_EQ(myUnsetRandomPropMinesGame.getNumOfMines(), 15);

    // square grid size (proportional mines) constructor with random statically set or not set at all
    minesweeper::Game mySquareUnsetRandomPropMinesGame(7, 0.24);
    EXPECT_EQ(mySquareUnsetRandomPropMinesGame.getGridHeight(), 7);
    EXPECT_EQ(mySquareUnsetRandomPropMinesGame.getGridWidth(), 7);
    EXPECT_EQ(mySquareUnsetRandomPropMinesGame.getNumOfMines(), 11);
}

TEST_F(MinesweeperGameTest, ConstructorGridSizeTest) {

    // negative grid
    EXPECT_THROW(minesweeper::Game myNegativeGridGame(-5, -3, 4, &myRandom), std::out_of_range);

    // negative grid (with proportional mines)
    EXPECT_THROW(minesweeper::Game myNegativeGridPropMinesGame(-2, -8, 0.19, &myRandom), std::out_of_range);

    // negative square grid
    EXPECT_THROW(minesweeper::Game myNegativeSquareGridGame(-5, 8, &myRandom), std::out_of_range);

    // negative square grid (with proportional mines)
    EXPECT_THROW(minesweeper::Game myNegativeSquareGridPropMinesGame(-8, 0.5, &myRandom), std::out_of_range);

    // negative grid height
    EXPECT_THROW(minesweeper::Game myNegativeGridHeightGame(-6, 7, 0.45, &myRandom), std::out_of_range);
    // negative grid height (with proportional mines)
    EXPECT_THROW(minesweeper::Game myNegativeGridHeightPropMinesGame(-18, 15, 0.79, &myRandom), std::out_of_range);
    // negative grid width
    EXPECT_THROW(minesweeper::Game myNegativeGridWidthGame(13, -11, 0.81, &myRandom), std::out_of_range);
    // negative grid width (with proportional mines)
    EXPECT_THROW(minesweeper::Game myNegativeGridWidthPropMinesGame(21, -9, 0.68, &myRandom), std::out_of_range);

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

    // large grid (with proportional mines)
    minesweeper::Game myLargePropMinesGame(135, 111, 0.67, &myRandom);
    EXPECT_EQ(myLargePropMinesGame.getGridHeight(), 135);
    EXPECT_EQ(myLargePropMinesGame.getGridWidth(), 111);
    EXPECT_EQ(myLargePropMinesGame.getNumOfMines(), 10'039);

    // very large grid
    minesweeper::Game myVeryLargeGame(1'000, 1'500, 200'000, &myRandom);
    EXPECT_EQ(myVeryLargeGame.getGridHeight(), 1'000);
    EXPECT_EQ(myVeryLargeGame.getGridWidth(), 1'500);
    EXPECT_EQ(myVeryLargeGame.getNumOfMines(), 200'000);
}

TEST_F(MinesweeperGameTest, ConstructorMineTest) {

    // +------------------------+
    // | number of mines tests: |
    // +------------------------+

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

    // +----------------------------+
    // | proportion of mines tests: |
    // +----------------------------+

    // negative proportion of mines
    EXPECT_THROW(minesweeper::Game myNegativePropMinesGame1(6, 9, -0.52, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game myNegativePropMinesGame2(8, 10, -15.78, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game myNegativePropMinesGame3(17, 13, -1.25, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game myNegativePropMinesGame4(2'544, 7'410, -54'514'518.52, &myRandom),
                 std::out_of_range);

    // zero proportion of mines
    minesweeper::Game myZeroPropMinesGame(8, 0.0, &myRandom);
    EXPECT_EQ(myZeroPropMinesGame.getNumOfMines(), 0);

    // max proportion of mines
    minesweeper::Game maxPropMinesGame(18, 16, 0.968'75, &myRandom);
    EXPECT_EQ(maxPropMinesGame.getNumOfMines(), 279);

    // close to max proportion of mines
    minesweeper::Game closeToMaxPropMinesGame(9, 15, 0.933'333'333'333'3, &myRandom);
    EXPECT_EQ(closeToMaxPropMinesGame.getNumOfMines(), 125);

    // close to max proportion of mines again
    minesweeper::Game closeToMaxPropMinesGame2(9, 0.888'888'888'888'88, &myRandom);
    EXPECT_EQ(closeToMaxPropMinesGame2.getNumOfMines(), 71);

    // slightly too big proportion of mines
    EXPECT_THROW(minesweeper::Game slightTooBigPropMinesGame(18, 27, 0.981'481'481'481'49, &myRandom),
                 std::out_of_range);

    // slightly too big proportion (and number) of mines
    EXPECT_THROW(minesweeper::Game slightTooBigPropAndNumMinesGame(17, 16, 0.970'588'235'295, &myRandom),
                 std::out_of_range);

    // too big proportion mines
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame1(14, 0.959'4, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame2(19, 9, 1.0, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame3(11, 18, 1.000'000'000'5, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame4(41, 1.01235565256, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame5(27, 12, 2.012510215, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame6(78, 35, 12.0251522, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame7(27, 39, 46.691851081, &myRandom), std::out_of_range);
    EXPECT_THROW(minesweeper::Game tooBigPropMinesGame8(35, 44, 85.0, &myRandom), std::out_of_range);
}

TEST_F(MinesweeperGameTest, CopyConstructorTest) {

    // empty game with random
    minesweeper::Game emptyGameWithRandom(0, 0, &myRandom);
    minesweeper::Game emptyGameWithRandomCopy(emptyGameWithRandom);
    EXPECT_EQ(serialiseToString(emptyGameWithRandom), serialiseToString(emptyGameWithRandomCopy));

    // empty game without random
    minesweeper::Game emptyGameWithoutRandom(0, 0);
    minesweeper::Game emptyGameWithoutRandomCopy(emptyGameWithoutRandom);
    EXPECT_EQ(serialiseToString(emptyGameWithoutRandom), serialiseToString(emptyGameWithoutRandomCopy));
    EXPECT_EQ(serialiseToString(emptyGameWithoutRandom), serialiseToString(emptyGameWithRandom));

    // simple unstarted game with random
    minesweeper::Game simpleUnstartedGameWithRandom(11, 12, 37, &myRandom);
    minesweeper::Game simpleUnstartedGameWithRandomCopy(simpleUnstartedGameWithRandom);
    EXPECT_EQ(serialiseToString(simpleUnstartedGameWithRandom), serialiseToString(simpleUnstartedGameWithRandomCopy));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandom.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy.checkInputCoordinates(6, 5));

    // simple unstarted game without random
    minesweeper::Game simpleUnstartedGameWithoutRandom(13, 10, 38);
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy(simpleUnstartedGameWithoutRandom);
    EXPECT_EQ(serialiseToString(simpleUnstartedGameWithoutRandom),
              serialiseToString(simpleUnstartedGameWithoutRandomCopy));
    EXPECT_THROW(simpleUnstartedGameWithoutRandom.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy.checkInputCoordinates(7, 3), std::invalid_argument);

    // simple started game
    minesweeper::Game simpleGame(9, 16, 29, &myRandom);
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(2, 7));
    minesweeper::Game simpleGameCopy(simpleGame);
    EXPECT_EQ(serialiseToString(simpleGame), serialiseToString(simpleGameCopy));

    // normal started game with checks and marks
    // (Since the grid is randomly generated, this will check different grid each time.
    //  Hence this might suddenly catch an unexpected error)
    minesweeper::Game normalGame(13, 9, 0.5, &myRandom);
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(7, 1));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(5, 5));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(1, 0));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(4, 11));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(2, 8));
    minesweeper::Game normalGameCopy(normalGame);
    EXPECT_EQ(serialiseToString(normalGame), serialiseToString(normalGameCopy));

    // lost game with invalid mark (should explore/validate most of Game fields)
    minesweeper::Game invalidMarkAndLossGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json],
                            invalidMarkAndLossGame));
    EXPECT_NE(invalidMarkAndLossNineByFiveGameStr, "");
    minesweeper::Game invalidMarkAndLossGameCopy(invalidMarkAndLossGame);
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy));
}

TEST_F(MinesweeperGameTest, MoveConstructorTest) {

    // empty game with random
    minesweeper::Game emptyGameWithRandom(0, 0, &myRandom);
    std::string emptyGameWithRandomStr = serialiseToString(emptyGameWithRandom);
    minesweeper::Game emptyGameWithRandomCopy(std::move(emptyGameWithRandom));
    EXPECT_EQ(emptyGameWithRandomStr, serialiseToString(emptyGameWithRandomCopy));

    // empty game without random
    minesweeper::Game emptyGameWithoutRandom(0, 0);
    std::string emptyGameWithoutRandomStr = serialiseToString(emptyGameWithoutRandom);
    minesweeper::Game emptyGameWithoutRandomCopy(std::move(emptyGameWithoutRandom));
    EXPECT_EQ(emptyGameWithoutRandomStr, serialiseToString(emptyGameWithoutRandomCopy));
    EXPECT_EQ(emptyGameWithoutRandomStr, serialiseToString(emptyGameWithRandom));

    // simple unstarted game with random
    minesweeper::Game simpleUnstartedGameWithRandom(11, 12, 37, &myRandom);
    std::string simpleUnstartedGameWithRandomStr = serialiseToString(simpleUnstartedGameWithRandom);
    minesweeper::Game simpleUnstartedGameWithRandomCopy(std::move(simpleUnstartedGameWithRandom));
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy.checkInputCoordinates(6, 5));

    // simple unstarted game without random
    minesweeper::Game simpleUnstartedGameWithoutRandom(13, 10, 38);
    std::string simpleUnstartedGameWithoutRandomStr = serialiseToString(simpleUnstartedGameWithoutRandom);
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy(std::move(simpleUnstartedGameWithoutRandom));
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy));
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy.checkInputCoordinates(7, 3), std::invalid_argument);

    // simple started game
    minesweeper::Game simpleGame(9, 16, 29, &myRandom);
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(2, 7));
    std::string simpleGameStr = serialiseToString(simpleGame);
    minesweeper::Game simpleGameCopy(std::move(simpleGame));
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy));

    // normal started game with checks and marks
    // (Since the grid is randomly generated, this will check different grid each time.
    //  Hence this might suddenly catch an unexpected error)
    minesweeper::Game normalGame(13, 9, 0.5, &myRandom);
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(7, 1));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(5, 5));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(1, 0));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(4, 11));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(2, 8));
    std::string normalGameStr = serialiseToString(normalGame);
    minesweeper::Game normalGameCopy(std::move(normalGame));
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy));

    // lost game with invalid mark (should explore/validate most of Game fields)
    minesweeper::Game invalidMarkAndLossGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json],
                            invalidMarkAndLossGame));
    EXPECT_NE(invalidMarkAndLossNineByFiveGameStr, "");
    minesweeper::Game invalidMarkAndLossGameCopy(std::move(invalidMarkAndLossGame));
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy));
}

TEST_F(MinesweeperGameTest, CopyAssignmentTest) {

    // empty game with random
    minesweeper::Game emptyGameWithRandom(0, 0, &myRandom);
    minesweeper::Game emptyGameWithRandomCopy(15, 20);
    emptyGameWithRandomCopy = emptyGameWithRandom;
    EXPECT_EQ(serialiseToString(emptyGameWithRandom), serialiseToString(emptyGameWithRandomCopy));

    // empty game without random
    minesweeper::Game emptyGameWithoutRandom(0, 0);
    minesweeper::Game emptyGameWithoutRandomCopy(11, 12, 0.5, &myRandom);
    emptyGameWithoutRandomCopy = emptyGameWithoutRandom;
    EXPECT_EQ(serialiseToString(emptyGameWithoutRandom), serialiseToString(emptyGameWithoutRandomCopy));
    EXPECT_EQ(serialiseToString(emptyGameWithoutRandom), serialiseToString(emptyGameWithRandom));

    // +-----------------------------------+
    // | simple unstarted game with random |
    // +-----------------------------------+
    minesweeper::Game simpleUnstartedGameWithRandom(11, 12, 37, &myRandom);
    std::string simpleUnstartedGameWithRandomStr = serialiseToString(simpleUnstartedGameWithRandom);
    // simple copy
    minesweeper::Game simpleUnstartedGameWithRandomCopy1(9, 8, &myRandom);
    simpleUnstartedGameWithRandomCopy1 = simpleUnstartedGameWithRandom;
    // copy without random
    minesweeper::Game simpleUnstartedGameWithRandomCopy2;
    simpleUnstartedGameWithRandomCopy2 = simpleUnstartedGameWithRandom;
    // copy with progress made already
    minesweeper::Game simpleUnstartedGameWithRandomCopy3(10, 5, &myRandom);
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(2, 4));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(4, 7));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.markInputCoordinates(0, 0));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.markInputCoordinates(0, 8));
    simpleUnstartedGameWithRandomCopy3 = simpleUnstartedGameWithRandom;
    // checks
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy1));
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy2));
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy3));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandom.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy1.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy2.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(6, 5));

    // +--------------------------------------+
    // | simple unstarted game without random |
    // +--------------------------------------+
    minesweeper::Game simpleUnstartedGameWithoutRandom(13, 10, 38);
    std::string simpleUnstartedGameWithoutRandomStr = serialiseToString(simpleUnstartedGameWithoutRandom);
    // simple copy
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy1(10, 3, nullptr);
    simpleUnstartedGameWithoutRandomCopy1 = simpleUnstartedGameWithoutRandom;
    // copy with random
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy2(4, 7, &myRandom);
    simpleUnstartedGameWithoutRandomCopy2 = simpleUnstartedGameWithoutRandom;
    // copy with progress made already
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy3(9, 29, &myRandom);
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(4, 3));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(0, 1));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.markInputCoordinates(8, 7));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.markInputCoordinates(5, 0));
    simpleUnstartedGameWithoutRandomCopy3 = simpleUnstartedGameWithoutRandom;
    // checks
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy1));
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy2));
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy3));
    EXPECT_THROW(simpleUnstartedGameWithoutRandom.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy1.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy2.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(7, 3), std::invalid_argument);

    // +---------------------+
    // | simple started game |
    // +---------------------+
    minesweeper::Game simpleGame(9, 16, 29, &myRandom);
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(2, 7));
    std::string simpleGameStr = serialiseToString(simpleGame);
    // simple copy
    minesweeper::Game simpleGameCopy1(7, 0.6, &myRandom);
    simpleGameCopy1 = simpleGame;
    // copy without random
    minesweeper::Game simpleGameCopy2(5, 7);
    simpleGameCopy2 = simpleGame;
    // copy with progress made already
    minesweeper::Game simpleGameCopy3(8, 12, 24, &myRandom);
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(2, 1));
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(10, 4));
    EXPECT_NO_THROW(simpleGameCopy3.markInputCoordinates(0, 6));
    EXPECT_NO_THROW(simpleGameCopy3.markInputCoordinates(9, 5));
    simpleGameCopy3 = simpleGame;
    // checks
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy1));
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy2));
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy3));
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy1.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy2.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(11, 0));

    // +-------------------------------------------+
    // | normal started game with checks and marks |
    // +-------------------------------------------+
    // (Since the grid is randomly generated, this will check different grid each time.
    //  Hence this might suddenly catch an unexpected error)
    minesweeper::Game normalGame(13, 9, 0.5, &myRandom);
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(7, 1));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(5, 5));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(1, 0));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(4, 11));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(2, 8));
    std::string normalGameStr = serialiseToString(normalGame);
    // simple copy
    minesweeper::Game normalGameCopy1(9, 14, 34, &myRandom);
    normalGameCopy1 = normalGame;
    // copy without random
    minesweeper::Game normalGameCopy2(8, 7, 0.445'45);
    normalGameCopy2 = normalGame;
    // copy with progress made already
    minesweeper::Game normalGameCopy3(11, 0.531, &myRandom);
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(8, 2));
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(2, 3));
    EXPECT_NO_THROW(normalGameCopy3.markInputCoordinates(2, 7));
    EXPECT_NO_THROW(normalGameCopy3.markInputCoordinates(5, 5));
    normalGameCopy3 = normalGame;
    // checks
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy1));
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy2));
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy3));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy1.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy2.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(4, 9));

    // +-----------------------------+
    // | lost game with invalid mark |
    // +-----------------------------+
    // (should explore/validate most of Game fields)
    minesweeper::Game invalidMarkAndLossGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json],
                            invalidMarkAndLossGame));
    EXPECT_NE(invalidMarkAndLossNineByFiveGameStr, "");
    // simple copy
    minesweeper::Game invalidMarkAndLossGameCopy1(13, 0.354, &myRandom);
    invalidMarkAndLossGameCopy1 = invalidMarkAndLossGame;
    // copy without random
    minesweeper::Game invalidMarkAndLossGameCopy2(7, 11, 28);
    invalidMarkAndLossGameCopy2 = invalidMarkAndLossGame;
    // copy with progress made already
    minesweeper::Game invalidMarkAndLossGameCopy3(8, 11, 28, &myRandom);
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(8, 2));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(2, 3));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.markInputCoordinates(2, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.markInputCoordinates(5, 5));
    invalidMarkAndLossGameCopy3 = invalidMarkAndLossGame;
    // checks
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy1));
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy2));
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy3));
    EXPECT_NO_THROW(invalidMarkAndLossGame.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy1.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy2.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(1, 7));
}

TEST_F(MinesweeperGameTest, MoveAssignmentTest) {

    //----------------------------------------------------------------------
    // As it is, these tests a quite redundant, since they are almost straight up copies of 'CopyAssignmentTest'.
    // They are left just as a precaution and/or in case copy and move assignment get separated later on.
    //----------------------------------------------------------------------

    // empty game with random
    minesweeper::Game emptyGameWithRandom(0, 0, &myRandom);
    std::string emptyGameWithRandomStr = serialiseToString(emptyGameWithRandom);
    minesweeper::Game emptyGameWithRandomCopy(15, 20);
    emptyGameWithRandomCopy = std::move(emptyGameWithRandom);
    EXPECT_EQ(emptyGameWithRandomStr, serialiseToString(emptyGameWithRandomCopy));

    // empty game without random
    minesweeper::Game emptyGameWithoutRandom(0, 0);
    minesweeper::Game emptyGameWithoutRandomCopy(11, 12, 0.5, &myRandom);
    std::string emptyGameWithoutRandomStr = serialiseToString(emptyGameWithoutRandom);
    emptyGameWithoutRandomCopy = std::move(emptyGameWithoutRandom);
    EXPECT_EQ(emptyGameWithoutRandomStr, serialiseToString(emptyGameWithoutRandomCopy));
    EXPECT_EQ(emptyGameWithoutRandomStr, emptyGameWithRandomStr);

    // +-----------------------------------+
    // | simple unstarted game with random |
    // +-----------------------------------+
    minesweeper::Game simpleUnstartedGameWithRandom(11, 12, 37, &myRandom);
    std::string simpleUnstartedGameWithRandomStr = serialiseToString(simpleUnstartedGameWithRandom);
    // simple copy
    minesweeper::Game simpleUnstartedGameWithRandomCopy1(9, 8, &myRandom);
    auto simpleUnstartedGameWithRandomCopy1BeforeMove = simpleUnstartedGameWithRandom;
    simpleUnstartedGameWithRandomCopy1 = std::move(simpleUnstartedGameWithRandomCopy1BeforeMove);
    // copy without random
    minesweeper::Game simpleUnstartedGameWithRandomCopy2;
    auto simpleUnstartedGameWithRandomCopy2BeforeMove = simpleUnstartedGameWithRandom;
    simpleUnstartedGameWithRandomCopy2 = std::move(simpleUnstartedGameWithRandomCopy2BeforeMove);
    // copy with progress made already
    minesweeper::Game simpleUnstartedGameWithRandomCopy3(10, 5, &myRandom);
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(2, 4));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(4, 7));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.markInputCoordinates(0, 0));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.markInputCoordinates(0, 8));
    auto simpleUnstartedGameWithRandomCopy3BeforeMove = simpleUnstartedGameWithRandom;
    simpleUnstartedGameWithRandomCopy3 = std::move(simpleUnstartedGameWithRandomCopy3BeforeMove);
    // checks
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy1));
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy2));
    EXPECT_EQ(simpleUnstartedGameWithRandomStr, serialiseToString(simpleUnstartedGameWithRandomCopy3));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandom.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy1.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy2.checkInputCoordinates(6, 5));
    EXPECT_NO_THROW(simpleUnstartedGameWithRandomCopy3.checkInputCoordinates(6, 5));

    // +--------------------------------------+
    // | simple unstarted game without random |
    // +--------------------------------------+
    minesweeper::Game simpleUnstartedGameWithoutRandom(13, 10, 38);
    std::string simpleUnstartedGameWithoutRandomStr = serialiseToString(simpleUnstartedGameWithoutRandom);
    // simple copy
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy1(10, 3, nullptr);
    auto simpleUnstartedGameWithoutRandomCopy1BeforeMove = simpleUnstartedGameWithoutRandom;
    simpleUnstartedGameWithoutRandomCopy1 = std::move(simpleUnstartedGameWithoutRandomCopy1BeforeMove);
    // copy with random
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy2(4, 7, &myRandom);
    auto simpleUnstartedGameWithoutRandomCopy2BeforeMove = simpleUnstartedGameWithoutRandom;
    simpleUnstartedGameWithoutRandomCopy2 = std::move(simpleUnstartedGameWithoutRandomCopy2BeforeMove);
    // copy with progress made already
    minesweeper::Game simpleUnstartedGameWithoutRandomCopy3(9, 29, &myRandom);
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(4, 3));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(0, 1));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.markInputCoordinates(8, 7));
    EXPECT_NO_THROW(simpleUnstartedGameWithoutRandomCopy3.markInputCoordinates(5, 0));
    auto simpleUnstartedGameWithoutRandomCopy3BeforeMove = simpleUnstartedGameWithoutRandom;
    simpleUnstartedGameWithoutRandomCopy3 = std::move(simpleUnstartedGameWithoutRandomCopy3BeforeMove);
    // checks
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy1));
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy2));
    EXPECT_EQ(simpleUnstartedGameWithoutRandomStr, serialiseToString(simpleUnstartedGameWithoutRandomCopy3));
    EXPECT_THROW(simpleUnstartedGameWithoutRandom.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy1.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy2.checkInputCoordinates(7, 3), std::invalid_argument);
    EXPECT_THROW(simpleUnstartedGameWithoutRandomCopy3.checkInputCoordinates(7, 3), std::invalid_argument);

    // +---------------------+
    // | simple started game |
    // +---------------------+
    minesweeper::Game simpleGame(9, 16, 29, &myRandom);
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(2, 7));
    std::string simpleGameStr = serialiseToString(simpleGame);
    // simple copy
    minesweeper::Game simpleGameCopy1(7, 0.6, &myRandom);
    auto simpleGameCopy1BeforeMove = simpleGame;
    simpleGameCopy1 = std::move(simpleGameCopy1BeforeMove);
    // copy without random
    minesweeper::Game simpleGameCopy2(5, 7);
    auto simpleGameCopy2BeforeMove = simpleGame;
    simpleGameCopy2 = std::move(simpleGameCopy2BeforeMove);
    // copy with progress made already
    minesweeper::Game simpleGameCopy3(8, 12, 24, &myRandom);
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(2, 1));
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(10, 4));
    EXPECT_NO_THROW(simpleGameCopy3.markInputCoordinates(0, 6));
    EXPECT_NO_THROW(simpleGameCopy3.markInputCoordinates(9, 5));
    auto simpleGameCopy3BeforeMove = simpleGame;
    simpleGameCopy3 = std::move(simpleGameCopy3BeforeMove);
    // checks
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy1));
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy2));
    EXPECT_EQ(simpleGameStr, serialiseToString(simpleGameCopy3));
    EXPECT_NO_THROW(simpleGame.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy1.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy2.checkInputCoordinates(11, 0));
    EXPECT_NO_THROW(simpleGameCopy3.checkInputCoordinates(11, 0));

    // +-------------------------------------------+
    // | normal started game with checks and marks |
    // +-------------------------------------------+
    // (Since the grid is randomly generated, this will check different grid each time.
    //  Hence this might suddenly catch an unexpected error)
    minesweeper::Game normalGame(13, 9, 0.5, &myRandom);
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(7, 1));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(5, 5));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(1, 0));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(4, 11));
    EXPECT_NO_THROW(normalGame.markInputCoordinates(2, 8));
    std::string normalGameStr = serialiseToString(normalGame);
    // simple copy
    minesweeper::Game normalGameCopy1(9, 14, 34, &myRandom);
    auto normalGameCopy1BeforeMove = normalGame;
    normalGameCopy1 = std::move(normalGameCopy1BeforeMove);
    // copy without random
    minesweeper::Game normalGameCopy2(8, 7, 0.445'45);
    auto normalGameCopy2BeforeMove = normalGame;
    normalGameCopy2 = std::move(normalGameCopy2BeforeMove);
    // copy with progress made already
    minesweeper::Game normalGameCopy3(11, 0.531, &myRandom);
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(8, 2));
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(2, 3));
    EXPECT_NO_THROW(normalGameCopy3.markInputCoordinates(2, 7));
    EXPECT_NO_THROW(normalGameCopy3.markInputCoordinates(5, 5));
    auto normalGameCopy3BeforeMove = normalGame;
    normalGameCopy3 = std::move(normalGameCopy3BeforeMove);
    // checks
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy1));
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy2));
    EXPECT_EQ(normalGameStr, serialiseToString(normalGameCopy3));
    EXPECT_NO_THROW(normalGame.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy1.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy2.checkInputCoordinates(4, 9));
    EXPECT_NO_THROW(normalGameCopy3.checkInputCoordinates(4, 9));

    // +-----------------------------+
    // | lost game with invalid mark |
    // +-----------------------------+
    // (should explore/validate most of Game fields)
    minesweeper::Game invalidMarkAndLossGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json],
                            invalidMarkAndLossGame));
    EXPECT_NE(invalidMarkAndLossNineByFiveGameStr, "");
    // simple copy
    minesweeper::Game invalidMarkAndLossGameCopy1(13, 0.354, &myRandom);
    auto invalidMarkAndLossGameCopy1BeforeMove = invalidMarkAndLossGame;
    invalidMarkAndLossGameCopy1 = std::move(invalidMarkAndLossGameCopy1BeforeMove);
    // copy without random
    minesweeper::Game invalidMarkAndLossGameCopy2(7, 11, 28);
    auto invalidMarkAndLossGameCopy2BeforeMove = invalidMarkAndLossGame;
    invalidMarkAndLossGameCopy2 = std::move(invalidMarkAndLossGameCopy2BeforeMove);
    // copy with progress made already
    minesweeper::Game invalidMarkAndLossGameCopy3(8, 11, 28, &myRandom);
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(8, 2));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(2, 3));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.markInputCoordinates(2, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.markInputCoordinates(5, 5));
    auto invalidMarkAndLossGameCopy3BeforeMove = invalidMarkAndLossGame;
    invalidMarkAndLossGameCopy3 = std::move(invalidMarkAndLossGameCopy3BeforeMove);
    // checks
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy1));
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy2));
    EXPECT_EQ(invalidMarkAndLossNineByFiveGameStr, serialiseToString(invalidMarkAndLossGameCopy3));
    EXPECT_NO_THROW(invalidMarkAndLossGame.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy1.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy2.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(invalidMarkAndLossGameCopy3.checkInputCoordinates(1, 7));
}

TEST_F(MinesweeperGameTest, SwapTest) {
    // +--------------------------------------------------------+
    // | swap between empty game and simple started random game |
    // +--------------------------------------------------------+
    // empty game
    minesweeper::Game emptyGame(0, 0);
    std::string emptyGameBeforeSwapStr = serialiseToString(emptyGame);
    EXPECT_NE(emptyGameBeforeSwapStr, "");
    // simple game with random
    minesweeper::Game simpleGameWithRandom(14, 5, 24, &myRandom);
    EXPECT_NO_THROW(simpleGameWithRandom.checkInputCoordinates(2, 8));
    EXPECT_NO_THROW(simpleGameWithRandom.checkInputCoordinates(4, 2));
    EXPECT_NO_THROW(simpleGameWithRandom.markInputCoordinates(3, 5));
    EXPECT_NO_THROW(simpleGameWithRandom.markInputCoordinates(1, 11));
    std::string simpleGameWithRandomBeforeSwapStr = serialiseToString(simpleGameWithRandom);
    EXPECT_NE(simpleGameWithRandomBeforeSwapStr, "");
    EXPECT_NE(simpleGameWithRandomBeforeSwapStr, emptyGameBeforeSwapStr);
    // swap
    swap(emptyGame, simpleGameWithRandom);
    // checks
    EXPECT_EQ(serialiseToString(emptyGame), simpleGameWithRandomBeforeSwapStr);
    EXPECT_EQ(serialiseToString(simpleGameWithRandom), emptyGameBeforeSwapStr);

    // +--------------------------------------------------------------------------------+
    // | swap between simple started random game (2) and premade invalid mark loss game |
    // +--------------------------------------------------------------------------------+
    // simple started random game
    minesweeper::Game simpleGameWithRandom2(11, 7, 22, &myRandom);
    EXPECT_NO_THROW(simpleGameWithRandom2.checkInputCoordinates(1, 7));
    EXPECT_NO_THROW(simpleGameWithRandom2.checkInputCoordinates(0, 3));
    EXPECT_NO_THROW(simpleGameWithRandom2.markInputCoordinates(6, 9));
    EXPECT_NO_THROW(simpleGameWithRandom2.markInputCoordinates(6, 2));
    std::string simpleGameWithRandom2BeforeSwapStr = serialiseToString(simpleGameWithRandom2);
    EXPECT_NE(simpleGameWithRandom2BeforeSwapStr, "");
    // premade invalid mark loss game
    minesweeper::Game invalidMarkAndLossGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::invalidMarkLossGame_nineByFive_serialisation____json],
                            invalidMarkAndLossGame));
    std::string invalidMarkAndLossGameBeforeSwapStr = serialiseToString(invalidMarkAndLossGame);
    EXPECT_NE(invalidMarkAndLossGameBeforeSwapStr, "");
    // swap
    swap(simpleGameWithRandom2, invalidMarkAndLossGame);
    // checks
    EXPECT_EQ(serialiseToString(simpleGameWithRandom2), invalidMarkAndLossGameBeforeSwapStr);
    EXPECT_EQ(serialiseToString(invalidMarkAndLossGame), simpleGameWithRandom2BeforeSwapStr);
}

TEST_F(MinesweeperGameTest, CreateMinesAndNumsTest) {

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

TEST_F(MinesweeperGameTest, ResetTest) {

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

TEST_F(MinesweeperGameTest, NewGameTest) {

    // unstarted game should not change after newGame with same gridsize and number of mines
    // (normal grid, number of mines)
    minesweeper::Game unstartedGame(9, 12, 26, &myRandom);
    std::string unstartedGameStr = serialiseToString(unstartedGame);
    unstartedGame.newGame(9, 12, 26);
    std::string unstartedGameAfterNewGameStr = serialiseToString(unstartedGame);
    EXPECT_EQ(unstartedGameStr, unstartedGameAfterNewGameStr);

    // unstarted game should not change after newGame with same gridsize and number of mines
    // (square grid, number of mines)
    minesweeper::Game unstartedGame2(15, 15, 57, &myRandom);
    std::string unstartedGame2Str = serialiseToString(unstartedGame2);
    unstartedGame2.newGame(15, 57);
    std::string unstartedGame2AfterNewGameStr = serialiseToString(unstartedGame2);
    EXPECT_EQ(unstartedGame2Str, unstartedGame2AfterNewGameStr);

    // unstarted game should not change after newGame with same gridsize and number of mines
    // (normal grid, proportion of mines)
    minesweeper::Game unstartedGame3(19, 0.86475, &myRandom);
    std::string unstartedGame3Str = serialiseToString(unstartedGame3);
    unstartedGame3.newGame(19, 0.86475);
    std::string unstartedGame3AfterNewGameStr = serialiseToString(unstartedGame3);
    EXPECT_EQ(unstartedGame3Str, unstartedGame3AfterNewGameStr);

    // unstarted game should not change after newGame with same gridsize and number of mines
    // (square grid, proportion of mines)
    minesweeper::Game unstartedGame4(11, 13, 0.45954594418, &myRandom);
    std::string unstartedGame4Str = serialiseToString(unstartedGame4);
    unstartedGame4.newGame(11, 13, 0.45954594418);
    std::string unstartedGame4AfterNewGameStr = serialiseToString(unstartedGame4);
    EXPECT_EQ(unstartedGame4Str, unstartedGame4AfterNewGameStr);

    // +----------------+
    // | Throwing Tests |
    // +----------------+

    // negative grid sizes should throw
    // negative x
    minesweeper::Game newNegativeXGame(10, 18, 28, &myRandom);
    EXPECT_THROW(newNegativeXGame.newGame(-2, 30, 10), std::out_of_range);
    minesweeper::Game newNegativeXPropMinesGame(24, 11, 45, &myRandom);
    EXPECT_THROW(newNegativeXPropMinesGame.newGame(-8, 10, 0.45145), std::out_of_range);
    // negative y
    minesweeper::Game newNegativeYGame(8, 20, 50, &myRandom);
    EXPECT_THROW(newNegativeYGame.newGame(15, -10, 34), std::out_of_range);
    // negative x and y
    minesweeper::Game newNegativeXandYGame(32, 24, 148, &myRandom);
    EXPECT_THROW(newNegativeXandYGame.newGame(-7, -24, 0), std::out_of_range);
    minesweeper::Game newNegativeXandYPropMinesGame(28, 35, 245, &myRandom);
    EXPECT_THROW(newNegativeXandYPropMinesGame.newGame(-28, -12, 0.0), std::out_of_range);

    // invalid number of mines should throw
    // too many mines
    minesweeper::Game newTooManyMinesGame(17, 24, 258, &myRandom);
    EXPECT_THROW(newTooManyMinesGame.newGame(85, 42, 3'612), std::out_of_range);

    minesweeper::Game newTooManyMinesGame2(16, 7, 75, &myRandom);
    EXPECT_THROW(newTooManyMinesGame2.newGame(8, 19, 144), std::out_of_range);

    // negative number of mines
    minesweeper::Game newNegativeMinesGame(15, 6, 40, &myRandom);
    EXPECT_THROW(newNegativeMinesGame.newGame(21, 18, -8), std::out_of_range);

    // too big proportion of mines
    minesweeper::Game newTooBigPropMinesGame(18, 125, &myRandom);
    EXPECT_THROW(newTooBigPropMinesGame.newGame(9, 13, 0.923'076'923'1), std::out_of_range);

    minesweeper::Game newTooBigPropMinesGame2(13, 17, 114, &myRandom);
    EXPECT_THROW(newTooBigPropMinesGame2.newGame(14, 0.959'183'673'47), std::out_of_range);

    minesweeper::Game newTooBigPropMinesGame3(0, 0, &myRandom);
    EXPECT_THROW(newTooBigPropMinesGame3.newGame(99, 82, 1.000'001), std::out_of_range);

    minesweeper::Game newTooBigPropMinesGame4(1, 1, 0, &myRandom);
    EXPECT_THROW(newTooBigPropMinesGame4.newGame(155, 2.191'15), std::out_of_range);

    // too small proportion of mines
    minesweeper::Game newTooSmallPropMinesGame(0, 0, &myRandom);
    EXPECT_THROW(newTooSmallPropMinesGame.newGame(9, -0.000'000'000'1), std::out_of_range);

    minesweeper::Game newTooSmallPropMinesGame2(13, 18, 42, &myRandom);
    EXPECT_THROW(newTooSmallPropMinesGame2.newGame(28, -0.061'896'165'168), std::out_of_range);

    minesweeper::Game newTooSmallPropMinesGame3(1, 9, 0, &myRandom);
    EXPECT_THROW(newTooSmallPropMinesGame3.newGame(55, 79, -242.0), std::out_of_range);

    // +------------------------------------+
    // | new game after simple started game |
    // +------------------------------------+

    // premade started 5x6 game, remove mines reset
    minesweeper::Game premadeGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_fiveBySix_serialisation____json], premadeGame));
    EXPECT_NE(startedFiveBySixGameStr, "");
    // does cell have mine
    EXPECT_TRUE(premadeGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeGame.doesCellHaveMine(3, 2));
    // is cell visible
    EXPECT_FALSE(premadeGame.isCellVisible(4, 3));
    EXPECT_TRUE(premadeGame.isCellVisible(1, 2));
    // num of mines around cell
    EXPECT_EQ(premadeGame.numOfMinesAroundCell(3, 3), 3);
    EXPECT_EQ(premadeGame.numOfMinesAroundCell(2, 2), 1);
    // game fields
    EXPECT_EQ(premadeGame.getGridHeight(), 5);
    EXPECT_EQ(premadeGame.getGridWidth(), 6);
    EXPECT_EQ(premadeGame.getNumOfMines(), 8);
    // NEW GAME
    premadeGame.newGame(7, 9, 21);
    // serialisation comparisons
    std::string premadeGameStr = serialiseToString(premadeGame);
    EXPECT_NE(premadeGameStr, "");
    EXPECT_NE(premadeGameStr, startedSixByEightGameStr);
    // does cell have mine
    EXPECT_FALSE(premadeGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(premadeGame.doesCellHaveMine(3, 2));
    // is cell visible
    EXPECT_FALSE(premadeGame.isCellVisible(4, 3));
    EXPECT_FALSE(premadeGame.isCellVisible(1, 2));
    // num of mines around cell
    EXPECT_EQ(premadeGame.numOfMinesAroundCell(3, 3), 0);
    EXPECT_EQ(premadeGame.numOfMinesAroundCell(2, 2), 0);
    // game fields
    EXPECT_EQ(premadeGame.getGridHeight(), 7);
    EXPECT_EQ(premadeGame.getGridWidth(), 9);
    EXPECT_EQ(premadeGame.getNumOfMines(), 21);

    // +--------------------+
    // | multiple new games |
    // +--------------------+

    // gridHeight: 12, gridWidth: 7, numOfMines: 30
    minesweeper::Game multipleNewGamesGame(12, 7, 30, &myRandom);
    std::string multipleNewGamesGameStr1 = serialiseToString(multipleNewGamesGame);
    EXPECT_NE(multipleNewGamesGameStr1, "");

    // gridHeight: 10, gridWidth: 10, numOfMines: 23
    EXPECT_NO_THROW(multipleNewGamesGame.newGame(10, 23));
    std::string multipleNewGamesGameStr2 = serialiseToString(multipleNewGamesGame);
    EXPECT_NE(multipleNewGamesGameStr2, "");
    EXPECT_NE(multipleNewGamesGameStr2, multipleNewGamesGameStr1);

    // gridHeight: 12, gridWidth: 7, numOfMines: 33
    EXPECT_NO_THROW(multipleNewGamesGame.newGame(12, 7, 33));
    std::string multipleNewGamesGameStr3 = serialiseToString(multipleNewGamesGame);
    EXPECT_NE(multipleNewGamesGameStr3, "");
    EXPECT_NE(multipleNewGamesGameStr3, multipleNewGamesGameStr1);
    EXPECT_NE(multipleNewGamesGameStr3, multipleNewGamesGameStr2);

    // gridHeight: 10, gridWidth: 10, numOfMines: 23
    EXPECT_NO_THROW(multipleNewGamesGame.newGame(10, 10, 23));
    std::string multipleNewGamesGameStr4 = serialiseToString(multipleNewGamesGame);
    EXPECT_NE(multipleNewGamesGameStr4, "");
    EXPECT_NE(multipleNewGamesGameStr4, multipleNewGamesGameStr1);
    EXPECT_EQ(multipleNewGamesGameStr4, multipleNewGamesGameStr2);
    EXPECT_NE(multipleNewGamesGameStr4, multipleNewGamesGameStr3);

    // gridHeight: 12, gridWidth: 7, numOfMines: 30 (AFTER CHECKING CELL!)
    EXPECT_NO_THROW(multipleNewGamesGame.newGame(12, 7, 30));
    EXPECT_NO_THROW(multipleNewGamesGame.checkInputCoordinates(5, 4));
    std::string multipleNewGamesGameStr5 = serialiseToString(multipleNewGamesGame);
    EXPECT_NE(multipleNewGamesGameStr5, "");
    EXPECT_NE(multipleNewGamesGameStr5, multipleNewGamesGameStr1);
    EXPECT_NE(multipleNewGamesGameStr5, multipleNewGamesGameStr2);
    EXPECT_NE(multipleNewGamesGameStr5, multipleNewGamesGameStr3);
    EXPECT_NE(multipleNewGamesGameStr5, multipleNewGamesGameStr4);

    // +------------------------+
    // | multiple new games (2) |
    // +------------------------+

    // gridHeight: 0, gridWidth: 0, numOfMines: 0
    minesweeper::Game multipleNewGamesGame2(0, 0, &myRandom);
    std::string multipleNewGamesGame2Str1 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str1, "");

    // gridHeight: 10, gridWidth: 14, proportionOfMines: 0.5715
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(10, 14, 0.5715));
    std::string multipleNewGamesGame2Str2 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str2, "");
    EXPECT_NE(multipleNewGamesGame2Str2, multipleNewGamesGame2Str1);

    // gridHeight: 8, gridWidth: 8, proportionOfMines: 0.124'521'984'81
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(8, 8, 0.124'521'984'81));
    std::string multipleNewGamesGame2Str3 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str3, "");
    EXPECT_NE(multipleNewGamesGame2Str3, multipleNewGamesGame2Str1);
    EXPECT_NE(multipleNewGamesGame2Str3, multipleNewGamesGame2Str2);

    // gridHeight: 16, gridWidth: 9, proportionOfMines: 0.000'000'01
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(16, 9, 0.000'000'01));
    std::string multipleNewGamesGame2Str4 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str4, "");
    EXPECT_NE(multipleNewGamesGame2Str4, multipleNewGamesGame2Str1);
    EXPECT_NE(multipleNewGamesGame2Str4, multipleNewGamesGame2Str2);
    EXPECT_NE(multipleNewGamesGame2Str4, multipleNewGamesGame2Str3);

    // gridHeight: 10, gridWidth: 14, proportionOfMines: 0.5715
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(10, 14, 0.5715));
    std::string multipleNewGamesGame2Str5 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str5, "");
    EXPECT_NE(multipleNewGamesGame2Str5, multipleNewGamesGame2Str1);
    EXPECT_EQ(multipleNewGamesGame2Str5, multipleNewGamesGame2Str2);
    EXPECT_NE(multipleNewGamesGame2Str5, multipleNewGamesGame2Str3);
    EXPECT_NE(multipleNewGamesGame2Str5, multipleNewGamesGame2Str4);

    // gridHeight: 8, gridWidth: 8, proportionOfMines: 0.124'444'444'449
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(8, 8, 0.124'444'444'449));
    std::string multipleNewGamesGame2Str6 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str6, "");
    EXPECT_NE(multipleNewGamesGame2Str6, multipleNewGamesGame2Str1);
    EXPECT_NE(multipleNewGamesGame2Str6, multipleNewGamesGame2Str2);
    EXPECT_EQ(multipleNewGamesGame2Str6, multipleNewGamesGame2Str3);
    EXPECT_NE(multipleNewGamesGame2Str6, multipleNewGamesGame2Str4);
    EXPECT_NE(multipleNewGamesGame2Str6, multipleNewGamesGame2Str5);

    // gridHeight: 16, gridWidth: 9, proportionOfMines: 0.000'000'01
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(16, 9, 0.006'944'444'45));
    std::string multipleNewGamesGame2Str7 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str7, "");
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str1);
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str2);
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str3);
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str4);
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str5);
    EXPECT_NE(multipleNewGamesGame2Str7, multipleNewGamesGame2Str6);

    // gridHeight: 8, gridWidth: 8, numOfMines: 7
    EXPECT_NO_THROW(multipleNewGamesGame2.newGame(8, 8, 7));
    std::string multipleNewGamesGame2Str8 = serialiseToString(multipleNewGamesGame2);
    EXPECT_NE(multipleNewGamesGame2Str8, "");
    EXPECT_NE(multipleNewGamesGame2Str8, multipleNewGamesGame2Str1);
    EXPECT_NE(multipleNewGamesGame2Str8, multipleNewGamesGame2Str2);
    EXPECT_EQ(multipleNewGamesGame2Str8, multipleNewGamesGame2Str3);
    EXPECT_NE(multipleNewGamesGame2Str8, multipleNewGamesGame2Str4);
    EXPECT_NE(multipleNewGamesGame2Str8, multipleNewGamesGame2Str5);
    EXPECT_EQ(multipleNewGamesGame2Str8, multipleNewGamesGame2Str6);
    EXPECT_NE(multipleNewGamesGame2Str8, multipleNewGamesGame2Str7);

    // +---------------------------------------------------------------------------------------+
    // | 'newGame' with same gridsize and number of mines should be equivalent to reset(false) |
    // +---------------------------------------------------------------------------------------+

    // already won 4x8 game
    minesweeper::Game premadeWonGameToReset;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json], premadeWonGameToReset));
    EXPECT_NE(checkedWinFourByEightGameStr, "");
    std::string premadeWonGameToResetStr = serialiseToString(premadeWonGameToReset);
    EXPECT_EQ(premadeWonGameToResetStr, checkedWinFourByEightGameStr);

    // already won 4x8 game (copy)
    minesweeper::Game premadeWonGameToNewGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json],
                            premadeWonGameToNewGame));
    std::string premadeWonGameToNewGameStr = serialiseToString(premadeWonGameToNewGame);
    EXPECT_EQ(premadeWonGameToNewGameStr, premadeWonGameToResetStr);

    // already won 4x8 game (second copy)
    minesweeper::Game premadeWonGameWithPropMinesToNewGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json],
                            premadeWonGameWithPropMinesToNewGame));
    std::string premadeWonGameWithPropMinesToNewGameStr = serialiseToString(premadeWonGameWithPropMinesToNewGame);
    EXPECT_EQ(premadeWonGameWithPropMinesToNewGameStr, premadeWonGameToNewGameStr);

    // RESET already won 4x8 game
    premadeWonGameToReset.reset(false);
    std::string premadeWonGameAfterResetStr = serialiseToString(premadeWonGameToReset);
    EXPECT_NE(premadeWonGameAfterResetStr, "");
    EXPECT_NE(premadeWonGameAfterResetStr, premadeWonGameToResetStr);

    // NEW GAME already won 4x8 game
    premadeWonGameToNewGame.newGame(4, 8, 8);
    std::string premadeWonGameAfterNewGameStr = serialiseToString(premadeWonGameToNewGame);
    EXPECT_NE(premadeWonGameAfterNewGameStr, "");
    EXPECT_NE(premadeWonGameAfterNewGameStr, premadeWonGameToNewGameStr);
    EXPECT_EQ(premadeWonGameAfterNewGameStr, premadeWonGameAfterResetStr);

    // NEW GAME already won 4x8 game (with proportion of mines)
    premadeWonGameWithPropMinesToNewGame.newGame(4, 8, 0.25);
    std::string premadeWonGameWithPropMinesAfterNewGameStr = serialiseToString(premadeWonGameWithPropMinesToNewGame);
    EXPECT_NE(premadeWonGameWithPropMinesAfterNewGameStr, "");
    EXPECT_NE(premadeWonGameWithPropMinesAfterNewGameStr, premadeWonGameWithPropMinesToNewGameStr);
    EXPECT_EQ(premadeWonGameWithPropMinesAfterNewGameStr, premadeWonGameAfterResetStr);
    EXPECT_EQ(premadeWonGameWithPropMinesAfterNewGameStr, premadeWonGameAfterNewGameStr);
}

TEST_F(MinesweeperGameTest, CheckInputCoordinatesTest) {

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

TEST_F(MinesweeperGameTest, MarkInputCoordinatesTest) {

    // negative x and y
    minesweeper::Game negativeXandYMarkInputCoordsGame(10, 23, &myRandom);
    EXPECT_NO_THROW(negativeXandYMarkInputCoordsGame.checkInputCoordinates(1, 8));
    EXPECT_THROW(negativeXandYMarkInputCoordsGame.markInputCoordinates(-2, -3), std::out_of_range);

    // negative x
    minesweeper::Game negativeXMarkInputCoordsGame(9, 6, 15, &myRandom);
    EXPECT_NO_THROW(negativeXMarkInputCoordsGame.checkInputCoordinates(1, 8));
    EXPECT_THROW(negativeXMarkInputCoordsGame.markInputCoordinates(-4, 3), std::out_of_range);

    // negative y
    minesweeper::Game negativeYMarkInputCoordsGame(5, 8, 14, &myRandom);
    EXPECT_NO_THROW(negativeYMarkInputCoordsGame.checkInputCoordinates(1, 0));
    EXPECT_THROW(negativeYMarkInputCoordsGame.markInputCoordinates(3, -1), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYMarkInputCoordsGame(9, 30, &myRandom);
    EXPECT_NO_THROW(tooLargeXandYMarkInputCoordsGame.checkInputCoordinates(8, 7));
    EXPECT_THROW(tooLargeXandYMarkInputCoordsGame.markInputCoordinates(10, 15), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXMarkInputCoordsGame(16, 70, &myRandom);
    EXPECT_NO_THROW(tooLargeXMarkInputCoordsGame.checkInputCoordinates(14, 10));
    EXPECT_THROW(tooLargeXMarkInputCoordsGame.markInputCoordinates(35, 0), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYMarkInputCoordsGame(50, 500, &myRandom);
    EXPECT_NO_THROW(tooLargeYMarkInputCoordsGame.checkInputCoordinates(35, 41));
    EXPECT_THROW(tooLargeYMarkInputCoordsGame.markInputCoordinates(16, 50), std::out_of_range);

    // negative x and too large y
    minesweeper::Game negativeXandTooLargeYMarkInputCoordsGame(45, 80, 600, &myRandom);
    EXPECT_NO_THROW(negativeXandTooLargeYMarkInputCoordsGame.checkInputCoordinates(50, 30));
    EXPECT_THROW(negativeXandTooLargeYMarkInputCoordsGame.markInputCoordinates(-14, 90), std::out_of_range);

    // valid check set mark
    minesweeper::Game validMarkInputCoordsGame(11, 13, 30, &myRandom);
    EXPECT_NO_THROW(validMarkInputCoordsGame.checkInputCoordinates(5, 7));
    EXPECT_FALSE(validMarkInputCoordsGame.isCellMarked(8, 2));
    EXPECT_NO_THROW(validMarkInputCoordsGame.markInputCoordinates(8, 2));
    if (!validMarkInputCoordsGame.isCellVisible(8, 2)) {
        EXPECT_TRUE(validMarkInputCoordsGame.isCellMarked(8, 2));
    }

    // valid 'createMinesAndNums' set mark
    minesweeper::Game validCreateMinesAndNumsYMarkInputCoordsGame(47, 35, 700, &myRandom);
    EXPECT_NO_THROW(validCreateMinesAndNumsYMarkInputCoordsGame.createMinesAndNums(25, 35));
    EXPECT_FALSE(validCreateMinesAndNumsYMarkInputCoordsGame.isCellMarked(30, 42));
    EXPECT_NO_THROW(validCreateMinesAndNumsYMarkInputCoordsGame.markInputCoordinates(30, 42));
    if (!validCreateMinesAndNumsYMarkInputCoordsGame.isCellVisible(30, 42)) {
        EXPECT_TRUE(validCreateMinesAndNumsYMarkInputCoordsGame.isCellMarked(30, 42));
    }

    // not set mark
    minesweeper::Game notSetMinesMarkInputCoordsGame(110, 3000, &myRandom);
    EXPECT_THROW(notSetMinesMarkInputCoordsGame.markInputCoordinates(50, 68), std::invalid_argument);

    // unmark
    minesweeper::Game unmarkMarkInputCoordsGame(15, 32, 105, &myRandom);
    EXPECT_NO_THROW(unmarkMarkInputCoordsGame.checkInputCoordinates(20, 7));
    EXPECT_FALSE(unmarkMarkInputCoordsGame.isCellMarked(18, 10));
    EXPECT_NO_THROW(unmarkMarkInputCoordsGame.markInputCoordinates(18, 10));
    if (!unmarkMarkInputCoordsGame.isCellVisible(18, 10)) {
        EXPECT_TRUE(unmarkMarkInputCoordsGame.isCellMarked(18, 10));
    }
    EXPECT_NO_THROW(unmarkMarkInputCoordsGame.markInputCoordinates(18, 10));
    EXPECT_FALSE(unmarkMarkInputCoordsGame.isCellMarked(18, 10));
}

TEST_F(MinesweeperGameTest, CompleteAroundInputCoordinatesTest) {

    // +------------------------+
    // | Invalid Argument Tests |
    // +------------------------+

    // negative x and y
    minesweeper::Game negativeXandYCompleteAroundGame(21, 11, 0.1545, &myRandom);
    EXPECT_THROW(negativeXandYCompleteAroundGame.completeAroundInputCoordinates(-4, -11), std::out_of_range);
    EXPECT_NO_THROW(negativeXandYCompleteAroundGame.checkInputCoordinates(3, 8));
    EXPECT_THROW(negativeXandYCompleteAroundGame.completeAroundInputCoordinates(-1, -7), std::out_of_range);

    // negative x
    minesweeper::Game negativeXCompleteAroundGame(8, 13, 28, &myRandom);
    EXPECT_THROW(negativeXCompleteAroundGame.completeAroundInputCoordinates(-2, 5), std::out_of_range);
    EXPECT_NO_THROW(negativeXCompleteAroundGame.checkInputCoordinates(2, 6));
    EXPECT_THROW(negativeXCompleteAroundGame.completeAroundInputCoordinates(-9, 3), std::out_of_range);

    // negative x and y
    minesweeper::Game negativeYCompleteAroundGame(9, 22, 64, &myRandom);
    EXPECT_THROW(negativeYCompleteAroundGame.completeAroundInputCoordinates(11, -4), std::out_of_range);
    EXPECT_NO_THROW(negativeYCompleteAroundGame.checkInputCoordinates(10, 1));
    EXPECT_THROW(negativeYCompleteAroundGame.completeAroundInputCoordinates(18, -9), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYCompleteAroundGame(34, 0.2345, &myRandom);
    EXPECT_THROW(tooLargeXandYCompleteAroundGame.completeAroundInputCoordinates(88, 45), std::out_of_range);
    EXPECT_NO_THROW(tooLargeXandYCompleteAroundGame.checkInputCoordinates(19, 17));
    EXPECT_THROW(tooLargeXandYCompleteAroundGame.completeAroundInputCoordinates(36, 34), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXCompleteAroundGame(14, 45, &myRandom);
    EXPECT_THROW(tooLargeXCompleteAroundGame.completeAroundInputCoordinates(15, 8), std::out_of_range);
    EXPECT_NO_THROW(tooLargeXCompleteAroundGame.checkInputCoordinates(1, 5));
    EXPECT_THROW(tooLargeXCompleteAroundGame.completeAroundInputCoordinates(24, 1), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYCompleteAroundGame(9, 21, 0.425, &myRandom);
    EXPECT_THROW(tooLargeYCompleteAroundGame.completeAroundInputCoordinates(19, 16), std::out_of_range);
    EXPECT_NO_THROW(tooLargeYCompleteAroundGame.checkInputCoordinates(18, 0));
    EXPECT_THROW(tooLargeYCompleteAroundGame.completeAroundInputCoordinates(7, 9), std::out_of_range);

    // too large x and negative y
    minesweeper::Game tooLargeXandNegativeYCompleteAroundGame(26, 0.31, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYCompleteAroundGame.completeAroundInputCoordinates(26, -7), std::out_of_range);
    EXPECT_NO_THROW(tooLargeXandNegativeYCompleteAroundGame.checkInputCoordinates(19, 4));
    EXPECT_THROW(tooLargeXandNegativeYCompleteAroundGame.completeAroundInputCoordinates(67, -1), std::out_of_range);

    // game has not started (using method on a cell that is not visible should throw)
    minesweeper::Game unstartedGameCompleteAroundGame(10, 32, &myRandom);
    EXPECT_THROW(unstartedGameCompleteAroundGame.completeAroundInputCoordinates(0, 8), std::invalid_argument);
    EXPECT_THROW(unstartedGameCompleteAroundGame.completeAroundInputCoordinates(8, 5), std::invalid_argument);
    EXPECT_THROW(unstartedGameCompleteAroundGame.completeAroundInputCoordinates(1, 2), std::invalid_argument);

    // game has not started 2 (using method on a cell that is not visible should throw)
    minesweeper::Game unstartedGameCompleteAroundGame2(18, 16, 0.4, &myRandom);
    EXPECT_THROW(unstartedGameCompleteAroundGame2.completeAroundInputCoordinates(9, 5), std::invalid_argument);
    EXPECT_THROW(unstartedGameCompleteAroundGame2.completeAroundInputCoordinates(11, 17), std::invalid_argument);
    EXPECT_THROW(unstartedGameCompleteAroundGame2.completeAroundInputCoordinates(0, 16), std::invalid_argument);

    // cell is not visible (using method on a cell that is not visible should throw)
    minesweeper::Game notVisibleCellCompleteAroundGame(8, 11, 26, &myRandom);
    EXPECT_NO_THROW(notVisibleCellCompleteAroundGame.checkInputCoordinates(6, 3));
    for (int y = 0; y < notVisibleCellCompleteAroundGame.getGridHeight(); ++y) {
        for (int x = 0; x < notVisibleCellCompleteAroundGame.getGridWidth(); ++x) {
            if (!notVisibleCellCompleteAroundGame.isCellVisible(x, y)) {
                EXPECT_THROW(notVisibleCellCompleteAroundGame.completeAroundInputCoordinates(x, y),
                             std::invalid_argument);
            }
        }
    }

    // is cell visible
    // (using method on a valid cell should either throw or not, depending on if it is not visible or visible)
    minesweeper::Game isCellVisibleCompleteAroundGame(14, 7, 31, &myRandom);
    EXPECT_NO_THROW(isCellVisibleCompleteAroundGame.checkInputCoordinates(4, 7));
    for (int y = 0; y < isCellVisibleCompleteAroundGame.getGridHeight(); ++y) {
        for (int x = 0; x < isCellVisibleCompleteAroundGame.getGridWidth(); ++x) {
            if (!isCellVisibleCompleteAroundGame.isCellVisible(x, y)) {
                EXPECT_THROW(isCellVisibleCompleteAroundGame.completeAroundInputCoordinates(x, y),
                             std::invalid_argument);
            } else {
                EXPECT_NO_THROW(isCellVisibleCompleteAroundGame.completeAroundInputCoordinates(x, y));
            }
        }
    }

    // +-------------------+
    // | Valid Usage Tests |
    // +-------------------+

    // complete around initial cell
    // (since standard first move already guarantees that cells around get checked, using this method on initial cell
    //  should be useless)
    minesweeper::Game initialCellCompleteAroundGame(14, 0.24, &myRandom);
    EXPECT_NO_THROW(initialCellCompleteAroundGame.checkInputCoordinates(8, 5));
    auto initialCellCompleteAroundGameStr = serialiseToString(initialCellCompleteAroundGame);
    EXPECT_NO_THROW(initialCellCompleteAroundGame.completeAroundInputCoordinates(8, 5));
    EXPECT_EQ(serialiseToString(initialCellCompleteAroundGame), initialCellCompleteAroundGameStr);

    // complete around initial cell 2
    minesweeper::Game initialCellCompleteAroundGame2(14, 9, 37, &myRandom);
    EXPECT_NO_THROW(initialCellCompleteAroundGame2.checkInputCoordinates(6, 11));
    auto initialCellCompleteAroundGame2Str = serialiseToString(initialCellCompleteAroundGame2);
    EXPECT_NO_THROW(initialCellCompleteAroundGame2.completeAroundInputCoordinates(6, 11));
    EXPECT_EQ(serialiseToString(initialCellCompleteAroundGame2), initialCellCompleteAroundGame2Str);

    // +-----------------------------+
    // | premade unfinished 7x7 game |
    // +-----------------------------+

    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    // method on invisible cells
    EXPECT_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(0, 1), std::invalid_argument);
    EXPECT_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(0, 6), std::invalid_argument);
    EXPECT_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(2, 2), std::invalid_argument);
    EXPECT_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(6, 4), std::invalid_argument);
    // method on useless cells
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(3, 4));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(1, 3));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(2, 5));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(1, 5));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(0, 5));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(4, 4));
    EXPECT_EQ(serialiseToString(premadeUnfinishedSevenBySevenGame),
              unfinishedSevenBySevenGameStr); // no change expected
    // method on cells which are not useful yet
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(0, 2));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(2, 1));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(4, 1));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(4, 5));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(5, 2));
    EXPECT_EQ(serialiseToString(premadeUnfinishedSevenBySevenGame),
              unfinishedSevenBySevenGameStr); // no change expected
    // useful usage
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellVisible(5, 1)); // usage 1
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(4, 2));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(5, 1));  // end of usage 1
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellVisible(5, 6)); // usage 2
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.markInputCoordinates(4, 6));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.markInputCoordinates(5, 5));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(4, 5));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(5, 6));  // end of usage 2
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellVisible(0, 1)); // usage 3
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(0, 4));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.markInputCoordinates(3, 0));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(3, 1));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(0, 2));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(0, 4)); // end of usage 3
    // wrongly marked usage
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.playerHasLost());
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellVisible(1, 1));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.markInputCoordinates(0, 1));
    EXPECT_NO_THROW(premadeUnfinishedSevenBySevenGame.completeAroundInputCoordinates(1, 2));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellVisible(1, 1));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.playerHasLost());

    // +-----------------------+
    // | premade 5x3 loss game |
    // +-----------------------+

    minesweeper::Game premadeLossFiveByThreeGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeLossFiveByThreeGame));
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(0, 4));
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(1, 3));
    EXPECT_NO_THROW(premadeLossFiveByThreeGame.completeAroundInputCoordinates(0, 3));
    EXPECT_EQ(serialiseToString(premadeLossFiveByThreeGame), lossFiveByThreeGameStr);
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(0, 4));
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(1, 3));
    EXPECT_NO_THROW(premadeLossFiveByThreeGame.markInputCoordinates(0, 4));
    EXPECT_NO_THROW(premadeLossFiveByThreeGame.markInputCoordinates(1, 4));
    EXPECT_NO_THROW(premadeLossFiveByThreeGame.completeAroundInputCoordinates(0, 3));
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(1, 3)); // good
    EXPECT_NO_THROW(premadeLossFiveByThreeGame.completeAroundInputCoordinates(1, 2));
    EXPECT_FALSE(premadeLossFiveByThreeGame.isCellVisible(1, 3));
}

TEST_F(MinesweeperGameTest, PlayerHasWonTest) {

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

TEST_F(MinesweeperGameTest, PlayerHasLostTest) {

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

    for (const auto& gameParas : gameParameterSet) {
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

TEST_F(MinesweeperGameTest, IsCellVisibleTest) {

    // +------------------------+
    // | Invalid Argument Tests |
    // +------------------------+

    // negative X and Y
    minesweeper::Game negativeXandYGame(18, 92, &myRandom);
    EXPECT_THROW(negativeXandYGame.isCellVisible(-3, -4), std::out_of_range);

    // negative X
    minesweeper::Game negativeXGame(10, 16, 47, &myRandom);
    EXPECT_THROW(negativeXGame.isCellVisible(-2, 8), std::out_of_range);

    // negative Y
    minesweeper::Game negativeYGame(17, 42, &myRandom);
    EXPECT_THROW(negativeYGame.isCellVisible(14, -1), std::out_of_range);

    // too large X and Y
    minesweeper::Game tooLargeXandYGame(19, 84, &myRandom);
    EXPECT_THROW(tooLargeXandYGame.isCellVisible(28, 72), std::out_of_range);

    // too large X
    minesweeper::Game tooLargeXGame(44, 24, 214, &myRandom);
    EXPECT_THROW(tooLargeXGame.isCellVisible(34, 0), std::out_of_range);

    // too large Y
    minesweeper::Game tooLargeYGame(28, 167, &myRandom);
    EXPECT_THROW(tooLargeYGame.isCellVisible(0, 28), std::out_of_range);

    // too large X and negative Y
    minesweeper::Game tooLargeXandNegativeYGame(27, 18, 134, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYGame.isCellVisible(19, -4), std::out_of_range);

    // +-------------------+
    // | Valid Usage Tests |
    // +-------------------+

    // unstarted game should have no visible cells
    minesweeper::Game unstartedGame(5, 9, 20, &myRandom);
    EXPECT_FALSE(unstartedGame.isCellVisible(5, 4));
    EXPECT_FALSE(unstartedGame.isCellVisible(8, 3));
    EXPECT_FALSE(unstartedGame.isCellVisible(3, 1));
    EXPECT_FALSE(unstartedGame.isCellVisible(1, 2));
    EXPECT_FALSE(unstartedGame.isCellVisible(1, 0));

    // simple game
    minesweeper::Game simpleGame(11, 14, 34, &myRandom);
    EXPECT_FALSE(simpleGame.isCellVisible(0, 1));
    simpleGame.checkInputCoordinates(0, 1);
    EXPECT_TRUE(simpleGame.isCellVisible(0, 1));

    simpleGame.checkInputCoordinates(4, 5);
    EXPECT_TRUE(simpleGame.isCellVisible(4, 5));
    simpleGame.checkInputCoordinates(4, 5);
    EXPECT_TRUE(simpleGame.isCellVisible(4, 5));

    simpleGame.checkInputCoordinates(10, 8);
    EXPECT_TRUE(simpleGame.isCellVisible(10, 8));

    simpleGame.checkInputCoordinates(7, 7);
    EXPECT_TRUE(simpleGame.isCellVisible(7, 7));

    // premade started 6x8 game
    minesweeper::Game premadeSixByEightGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_sixByEight_serialisation____json], premadeSixByEightGame));
    EXPECT_NE(startedSixByEightGameStr, "");
    EXPECT_FALSE(premadeSixByEightGame.isCellVisible(2, 3));
    EXPECT_FALSE(premadeSixByEightGame.isCellVisible(0, 5));
    EXPECT_FALSE(premadeSixByEightGame.isCellVisible(7, 1));
    EXPECT_FALSE(premadeSixByEightGame.isCellVisible(1, 0));
    EXPECT_TRUE(premadeSixByEightGame.isCellVisible(5, 2));
    EXPECT_TRUE(premadeSixByEightGame.isCellVisible(6, 3));
    EXPECT_TRUE(premadeSixByEightGame.isCellVisible(5, 5));
    EXPECT_TRUE(premadeSixByEightGame.isCellVisible(7, 4));

    // premade unfinished 7x7 game
    minesweeper::Game premadeSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeSevenBySevenGame));
    EXPECT_NE(unfinishedSevenBySevenGameStr, "");
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(0, 2));
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(1, 2));
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(5, 4));
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(2, 6));
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(3, 5));
    EXPECT_TRUE(premadeSevenBySevenGame.isCellVisible(3, 4));
    EXPECT_FALSE(premadeSevenBySevenGame.isCellVisible(5, 0));
    EXPECT_FALSE(premadeSevenBySevenGame.isCellVisible(6, 3));
    EXPECT_FALSE(premadeSevenBySevenGame.isCellVisible(0, 1));
    EXPECT_FALSE(premadeSevenBySevenGame.isCellVisible(4, 0));

    // premade lost 5x3 game
    minesweeper::Game premadeFiveByThreeLossGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeFiveByThreeLossGame));
    EXPECT_NE(lossFiveByThreeGameStr, "");
    EXPECT_FALSE(premadeFiveByThreeLossGame.isCellVisible(0, 4));
    EXPECT_TRUE(premadeFiveByThreeLossGame.isCellVisible(2, 0));
    EXPECT_TRUE(premadeFiveByThreeLossGame.isCellVisible(1, 2));
    EXPECT_FALSE(premadeFiveByThreeLossGame.isCellVisible(1, 4));
    EXPECT_TRUE(premadeFiveByThreeLossGame.isCellVisible(2, 3));
    EXPECT_FALSE(premadeFiveByThreeLossGame.isCellVisible(2, 4));
    EXPECT_FALSE(premadeFiveByThreeLossGame.isCellVisible(0, 1));
}

TEST_F(MinesweeperGameTest, DoesCellHaveMineTest) {

    // +------------------------+
    // | Invalid Argument Tests |
    // +------------------------+

    // negative X and Y
    minesweeper::Game negativeXandYGame(51, 49, 744, &myRandom);
    EXPECT_THROW(negativeXandYGame.doesCellHaveMine(-10, -40), std::out_of_range);

    // negative X
    minesweeper::Game negativeXGame(144, 11'720, &myRandom);
    EXPECT_THROW(negativeXGame.doesCellHaveMine(-1, 124), std::out_of_range);

    // negative Y
    minesweeper::Game negativeYGame(151, 13, 108, &myRandom);
    EXPECT_THROW(negativeYGame.doesCellHaveMine(5, -3), std::out_of_range);

    // too large X and Y
    minesweeper::Game tooLargeXandYGame(80, 67, 4'011, &myRandom);
    EXPECT_THROW(tooLargeXandYGame.doesCellHaveMine(72, 90), std::out_of_range);

    // too large X
    minesweeper::Game tooLargeXGame(91, 1'136, &myRandom);
    EXPECT_THROW(tooLargeXGame.doesCellHaveMine(99, 0), std::out_of_range);

    // too large Y
    minesweeper::Game tooLargeYGame(127, 131, 10'882, &myRandom);
    EXPECT_THROW(tooLargeYGame.doesCellHaveMine(75, 127), std::out_of_range);

    // negative X and too large Y
    minesweeper::Game negativeXandTooLargeYGame(73, 407, &myRandom);
    EXPECT_THROW(negativeXandTooLargeYGame.doesCellHaveMine(-2, 75), std::out_of_range);

    // +-------------------+
    // | Valid Usage Tests |
    // +-------------------+

    // unstarted game should have no mines
    minesweeper::Game unstartedGame(14, 7, 28, &myRandom);
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(6, 11));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(0, 1));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(5, 2));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(3, 10));
    EXPECT_FALSE(unstartedGame.doesCellHaveMine(4, 7));

    // predetermined random game
    PreSetMinesRandom predeterminedRandom({35, 37, 41, 31, 3, 9, 56, 52, 65, 69, 76, 66, 39, 7, 13, 29});
    minesweeper::Game predeterminedRandomGame(10, 8, 16, &predeterminedRandom);
    predeterminedRandomGame.checkInputCoordinates(4, 7);
    EXPECT_FALSE(predeterminedRandomGame.doesCellHaveMine(6, 5));
    EXPECT_FALSE(predeterminedRandomGame.doesCellHaveMine(0, 3));
    EXPECT_FALSE(predeterminedRandomGame.doesCellHaveMine(2, 9));
    EXPECT_FALSE(predeterminedRandomGame.doesCellHaveMine(4, 3));
    EXPECT_FALSE(predeterminedRandomGame.doesCellHaveMine(5, 7));
    EXPECT_TRUE(predeterminedRandomGame.doesCellHaveMine(1, 2));
    EXPECT_TRUE(predeterminedRandomGame.doesCellHaveMine(7, 0));
    EXPECT_TRUE(predeterminedRandomGame.doesCellHaveMine(5, 4));
    EXPECT_TRUE(predeterminedRandomGame.doesCellHaveMine(0, 7));

    // premade started 5x6 game
    minesweeper::Game premadeFiveBySixGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_fiveBySix_serialisation____json], premadeFiveBySixGame));
    EXPECT_NE(startedFiveBySixGameStr, "");
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(0, 0));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(1, 1));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(5, 1));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(3, 3));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(2, 2));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(3, 0));
    EXPECT_TRUE(premadeFiveBySixGame.doesCellHaveMine(3, 4));
    EXPECT_TRUE(premadeFiveBySixGame.doesCellHaveMine(0, 4));
    EXPECT_TRUE(premadeFiveBySixGame.doesCellHaveMine(4, 1));
    EXPECT_TRUE(premadeFiveBySixGame.doesCellHaveMine(4, 4));
    EXPECT_FALSE(premadeFiveBySixGame.doesCellHaveMine(3, 0));

    // premade won 5x4 game
    minesweeper::Game premadeFiveByFourWonGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json], premadeFiveByFourWonGame));
    EXPECT_NE(markedWinFiveByFourGameStr, "");
    EXPECT_TRUE(premadeFiveByFourWonGame.doesCellHaveMine(0, 4));
    EXPECT_FALSE(premadeFiveByFourWonGame.doesCellHaveMine(3, 3));
    EXPECT_TRUE(premadeFiveByFourWonGame.doesCellHaveMine(1, 4));
    EXPECT_FALSE(premadeFiveByFourWonGame.doesCellHaveMine(2, 2));
    EXPECT_TRUE(premadeFiveByFourWonGame.doesCellHaveMine(0, 0));
    EXPECT_FALSE(premadeFiveByFourWonGame.doesCellHaveMine(3, 1));
    EXPECT_TRUE(premadeFiveByFourWonGame.doesCellHaveMine(1, 1));

    // unfinished 7x7 game
    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    EXPECT_NE(unfinishedSevenBySevenGameStr, "");
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(6, 1));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(0, 6));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(5, 2));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(2, 6));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(3, 3));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(3, 0));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(4, 6));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.doesCellHaveMine(0, 1));
}

TEST_F(MinesweeperGameTest, IsCellMarkedTest) {

    // +------------------------+
    // | Invalid Argument Tests |
    // +------------------------+

    // negative X and Y
    minesweeper::Game negativeXandYGame(150, 14'232, &myRandom);
    EXPECT_THROW(negativeXandYGame.isCellMarked(-125, -25), std::out_of_range);

    // negative X
    minesweeper::Game negativeXGame(42, 24, 588, &myRandom);
    EXPECT_THROW(negativeXGame.isCellMarked(-1, 20), std::out_of_range);

    // negative Y
    minesweeper::Game negativeYGame(105, 86, 1'650, &myRandom);
    EXPECT_THROW(negativeYGame.isCellMarked(54, -8), std::out_of_range);

    // too large X and Y
    minesweeper::Game tooLargeXandYGame(183, 35, 3'556, &myRandom);
    EXPECT_THROW(tooLargeXandYGame.isCellMarked(40, 201), std::out_of_range);

    // too large X
    minesweeper::Game tooLargeXGame(13, 30, &myRandom);
    EXPECT_THROW(tooLargeXGame.isCellMarked(14, 8), std::out_of_range);

    // too large Y
    minesweeper::Game tooLargeYGame(101, 6'146, &myRandom);
    EXPECT_THROW(tooLargeYGame.isCellMarked(82, 101), std::out_of_range);

    // negative X and too large Y
    minesweeper::Game negativeXandTooLargeYGame(17, 15, 129, &myRandom);
    EXPECT_THROW(negativeXandTooLargeYGame.isCellMarked(-4, 24), std::out_of_range);

    // +-------------------+
    // | Valid Usage Tests |
    // +-------------------+

    // unstarted game should have no marked cells
    minesweeper::Game unstartedGame(11, 16, 40, &myRandom);
    EXPECT_FALSE(unstartedGame.isCellMarked(9, 6));
    EXPECT_FALSE(unstartedGame.isCellMarked(13, 5));
    EXPECT_FALSE(unstartedGame.isCellMarked(11, 10));
    EXPECT_FALSE(unstartedGame.isCellMarked(2, 8));
    EXPECT_FALSE(unstartedGame.isCellMarked(0, 1));

    // simple game
    minesweeper::Game simpleGame(10, 91, &myRandom);
    simpleGame.checkInputCoordinates(6, 4);

    EXPECT_FALSE(simpleGame.isCellMarked(0, 1));
    simpleGame.markInputCoordinates(0, 1);
    EXPECT_TRUE(simpleGame.isCellMarked(0, 1));

    EXPECT_FALSE(simpleGame.isCellMarked(4, 3));
    simpleGame.markInputCoordinates(4, 3);
    EXPECT_TRUE(simpleGame.isCellMarked(4, 3));

    EXPECT_FALSE(simpleGame.isCellMarked(1, 3));
    simpleGame.markInputCoordinates(1, 3);
    EXPECT_TRUE(simpleGame.isCellMarked(1, 3));
    simpleGame.markInputCoordinates(1, 3);
    EXPECT_FALSE(simpleGame.isCellMarked(1, 3));

    EXPECT_FALSE(simpleGame.isCellMarked(2, 8));
    simpleGame.markInputCoordinates(2, 8);
    EXPECT_TRUE(simpleGame.isCellMarked(2, 8));

    EXPECT_FALSE(simpleGame.isCellMarked(9, 5));
    simpleGame.markInputCoordinates(9, 5);
    EXPECT_TRUE(simpleGame.isCellMarked(9, 5));
    simpleGame.markInputCoordinates(9, 5);
    EXPECT_FALSE(simpleGame.isCellMarked(9, 5));
    simpleGame.markInputCoordinates(9, 5);
    EXPECT_TRUE(simpleGame.isCellMarked(9, 5));

    EXPECT_TRUE(simpleGame.isCellMarked(4, 3));
    simpleGame.markInputCoordinates(4, 3);
    EXPECT_FALSE(simpleGame.isCellMarked(4, 3));

    EXPECT_FALSE(simpleGame.isCellMarked(2, 9));
    simpleGame.markInputCoordinates(2, 9);
    EXPECT_TRUE(simpleGame.isCellMarked(2, 9));

    // premade 7x7 unfinished game
    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    EXPECT_NE(unfinishedSevenBySevenGameStr, "");
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(0, 1));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(2, 2));
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(0, 6));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(4, 2));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(5, 0));
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(2, 4));

    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(4, 6));
    premadeUnfinishedSevenBySevenGame.markInputCoordinates(4, 6);
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(4, 6));

    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(3, 3));
    premadeUnfinishedSevenBySevenGame.markInputCoordinates(3, 3);
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(3, 3));
    premadeUnfinishedSevenBySevenGame.markInputCoordinates(3, 3);
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(3, 3));

    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(6, 0));
    premadeUnfinishedSevenBySevenGame.markInputCoordinates(6, 0);
    EXPECT_TRUE(premadeUnfinishedSevenBySevenGame.isCellMarked(6, 0));
    premadeUnfinishedSevenBySevenGame.markInputCoordinates(6, 0);
    EXPECT_FALSE(premadeUnfinishedSevenBySevenGame.isCellMarked(6, 0));

    // premade 4x8 checked win game
    minesweeper::Game premadeFourByEightWonGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json],
                            premadeFourByEightWonGame));
    EXPECT_NE(checkedWinFourByEightGameStr, "");
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(5, 0));
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(7, 2));
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(1, 1));
    EXPECT_TRUE(premadeFourByEightWonGame.isCellMarked(3, 1));
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(3, 2));
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(1, 3));
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(0, 3));
    EXPECT_TRUE(premadeFourByEightWonGame.isCellMarked(5, 3));

    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(5, 1));
    premadeFourByEightWonGame.markInputCoordinates(5, 1);
    EXPECT_FALSE(premadeFourByEightWonGame.isCellMarked(5, 1));
}

TEST_F(MinesweeperGameTest, NumOfMinesAroundCellTest) {

    // +------------------------+
    // | Invalid Argument Tests |
    // +------------------------+

    // negative X and Y
    minesweeper::Game negativeXandYGame(48, 1809, &myRandom);
    EXPECT_THROW(negativeXandYGame.numOfMinesAroundCell(-8, -89), std::out_of_range);

    // negative X
    minesweeper::Game negativeXGame(12, 84, 784, &myRandom);
    EXPECT_THROW(negativeXGame.numOfMinesAroundCell(-4, 4), std::out_of_range);

    // negative Y
    minesweeper::Game negativeYGame(90, 2563, &myRandom);
    EXPECT_THROW(negativeYGame.numOfMinesAroundCell(120, -17), std::out_of_range);

    // too large X and Y
    minesweeper::Game tooLargeXandYGame(60, 105, 3165, &myRandom);
    EXPECT_THROW(tooLargeXandYGame.numOfMinesAroundCell(111, 75), std::out_of_range);

    // too large X
    minesweeper::Game tooLargeXGame(119, 41, 3942, &myRandom);
    EXPECT_THROW(tooLargeXGame.numOfMinesAroundCell(41, 5), std::out_of_range);

    // too large Y
    minesweeper::Game tooLargeYGame(60, 2975, &myRandom);
    EXPECT_THROW(tooLargeYGame.numOfMinesAroundCell(157, 78), std::out_of_range);

    // too large X and negative Y
    minesweeper::Game tooLargeXandNegativeYGame(78, 874, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYGame.numOfMinesAroundCell(945, -26), std::out_of_range);

    // +-------------------+
    // | Valid Usage Tests |
    // +-------------------+

    // unstarted game should have no mines around
    minesweeper::Game unstartedGame(5, 10, 13, &myRandom);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(9, 1), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(2, 2), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(3, 0), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(7, 1), 0);
    EXPECT_EQ(unstartedGame.numOfMinesAroundCell(2, 3), 0);

    // premade 7x7 unfinished game
    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    EXPECT_NE(unfinishedSevenBySevenGameStr, "");
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(3, 0), 0);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(0, 2), 1);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(2, 5), 0);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(6, 3), 1);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(5, 6), 4);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(5, 0), 1);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(1, 4), 0);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(0, 4), 0);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(4, 0), 1);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(5, 1), 2);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.numOfMinesAroundCell(2, 2), 2);

    // premade 5x4 marked win game
    minesweeper::Game premadeFiveByFourLostGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json],
                            premadeFiveByFourLostGame));
    EXPECT_NE(markedWinFiveByFourGameStr, "");
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(3, 0), 0);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(3, 3), 0);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(0, 4), 1);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(1, 2), 1);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(3, 1), 1);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(1, 3), 2);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(2, 2), 1);
    EXPECT_EQ(premadeFiveByFourLostGame.numOfMinesAroundCell(0, 1), 3);

    // first checked cell should always have 0 mines around
    struct gameParameters {
        int gridHeight;
        int gridWidth;
        int numOfMines;
        int initialX;
        int initialY;
    };
    gameParameters gameParameterSet[] = {{10, 21, 33, 8, 7},   {14, 45, 300, 34, 11}, {4, 7, 10, 0, 0},
                                         {16, 25, 50, 21, 11}, {31, 45, 500, 44, 10}, {7, 15, 40, 0, 5},
                                         {16, 31, 140, 15, 8}};
    for (const auto& gameParas : gameParameterSet) {
        minesweeper::Game noMinesAroundFirstCheckGame(gameParas.gridHeight, gameParas.gridWidth, gameParas.numOfMines,
                                                      &myRandom);
        EXPECT_EQ(noMinesAroundFirstCheckGame.numOfMinesAroundCell(gameParas.initialX, gameParas.initialY), 0);
        noMinesAroundFirstCheckGame.checkInputCoordinates(gameParas.initialX, gameParas.initialY);
        EXPECT_EQ(noMinesAroundFirstCheckGame.numOfMinesAroundCell(gameParas.initialX, gameParas.initialY), 0);
    }
}

TEST_F(MinesweeperGameTest, SerialiseTest) {

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

TEST_F(MinesweeperGameTest, DeserialiseTest) { // TODO: update once deserialise is more advanced

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

TEST_F(MinesweeperGameTest, SerialiseAndDeserialiseTest) {

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

TEST_F(MinesweeperGameTest, VisualiseCellTest) {

    // negative x and y
    minesweeper::Game negativeXandYVisualiseCellGame(13, 15, 50, &myRandom);
    EXPECT_THROW(negativeXandYVisualiseCellGame.visualiseCell(-8, -3), std::out_of_range);

    // negative x
    minesweeper::Game negativeXVisualiseCellGame(18, 7, 65, &myRandom);
    EXPECT_THROW(negativeXVisualiseCellGame.visualiseCell(-2, 8), std::out_of_range);

    // negative y
    minesweeper::Game negativeYVisualiseCellGame(22, 98, &myRandom);
    EXPECT_THROW(negativeYVisualiseCellGame.visualiseCell(11, -8), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYVisualiseCellGame(123, 3'514, &myRandom);
    EXPECT_THROW(tooLargeXandYVisualiseCellGame.visualiseCell(140, 132), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXVisualiseCellGame(44, 18, 652, &myRandom);
    EXPECT_THROW(tooLargeXVisualiseCellGame.visualiseCell(18, 23), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYVisualiseCellGame(54, 617, &myRandom);
    EXPECT_THROW(tooLargeYVisualiseCellGame.visualiseCell(13, 261), std::out_of_range);

    // too large x and negative y
    minesweeper::Game tooLargeXandNegativeYVisualiseCellGame(23, 12, 94, &myRandom);
    EXPECT_THROW(tooLargeXandNegativeYVisualiseCellGame.visualiseCell(15, -2), std::out_of_range);

    // valid/expected template types
    minesweeper::Game validTemplateTypes(12, 32, &myRandom);
    EXPECT_NO_THROW(validTemplateTypes.checkInputCoordinates(5, 6));
    minesweeper::VisualMinesweeperCell cellVisualisation;
    EXPECT_NO_THROW(cellVisualisation = validTemplateTypes.visualiseCell<minesweeper::VisualMinesweeperCell>(4, 6));
    int cellIntVisualisation;
    EXPECT_NO_THROW(cellIntVisualisation = validTemplateTypes.visualiseCell<int>(4, 6));
    char cellCharVisualisation;
    EXPECT_NO_THROW(cellCharVisualisation = validTemplateTypes.visualiseCell<char>(4, 6));
    EXPECT_EQ(static_cast<int>(cellVisualisation), cellIntVisualisation);
    EXPECT_EQ(static_cast<int>(cellCharVisualisation), cellIntVisualisation);
    EXPECT_EQ(static_cast<minesweeper::VisualMinesweeperCell>(cellCharVisualisation), cellVisualisation);

    // custom template type
    class VisualTestCell {
      public:
        VisualTestCell(minesweeper::VisualMinesweeperCell x) {}
    };
    EXPECT_NO_THROW(validTemplateTypes.visualiseCell<VisualTestCell>(4, 6));

    // unfinished 7x7 game
    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    EXPECT_NE(unfinishedSevenBySevenGameStr, "");
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(1, 3), minesweeper::VisualMinesweeperCell::ONE);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(0, 0), minesweeper::VisualMinesweeperCell::UNCHECKED);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(1, 4), minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(5, 4), minesweeper::VisualMinesweeperCell::THREE);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(3, 3), minesweeper::VisualMinesweeperCell::MARKED);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCell(3, 1), minesweeper::VisualMinesweeperCell::TWO);

    // premade lost 5x3 game
    minesweeper::Game premadeFiveByThreeLossGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeFiveByThreeLossGame));
    EXPECT_NE(lossFiveByThreeGameStr, "");
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(0, 0), minesweeper::VisualMinesweeperCell::ONE);
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(2, 2), minesweeper::VisualMinesweeperCell::MARKED);
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(2, 0), minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(1, 2), minesweeper::VisualMinesweeperCell::THREE);
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(0, 3), minesweeper::VisualMinesweeperCell::MINE);
    EXPECT_EQ(premadeFiveByThreeLossGame.visualiseCell(1, 1), minesweeper::VisualMinesweeperCell::TWO);
}

TEST_F(MinesweeperGameTest, VisualiseTest) {

    // valid/expected template types
    minesweeper::Game validTemplateTypes(13, 8, 48, &myRandom);
    EXPECT_NO_THROW(validTemplateTypes.checkInputCoordinates(2, 7));
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<minesweeper::VisualMinesweeperCell>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<std::vector<minesweeper::VisualMinesweeperCell>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<int>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<std::vector<int>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::string>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<std::string>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::list<int>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::list<minesweeper::VisualMinesweeperCell>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::vector<std::list<int>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualise<std::list<std::vector<minesweeper::VisualMinesweeperCell>>>());

    // custom template type (with minimal requirements)
    class CustomExpandSeqContainer {
      public:
        // needed to deduce type of values to cast from minesweeper::VisualMinesweeperCell
        using value_type = int;

        // needed to push_back values
        void push_back(value_type x) {}

        // not needed for anything, but expected for any serious container
        size_t size() { return 0; }
    };
    EXPECT_NO_THROW(validTemplateTypes.visualise<CustomExpandSeqContainer>());

    // premade lost 5x3 game
    minesweeper::Game premadeFiveByThreeLossGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeFiveByThreeLossGame));
    EXPECT_NE(lossFiveByThreeGameStr, "");
    std::vector<std::vector<int>> premadeFiveByThreeVisualMatrixExpected{
        {1, 1, 0}, {10, 2, 1}, {2, 3, 10}, {9, -1, 3}, {-1, -1, -1}};
    EXPECT_EQ(premadeFiveByThreeLossGame.visualise<std::vector<std::vector<int>>>(),
              premadeFiveByThreeVisualMatrixExpected);
    std::vector<int> premadeFiveByThreeVisualVectorExpected{1, 1, 0, 10, 2, 1, 2, 3, 10, 9, -1, 3, -1, -1, -1};
    EXPECT_EQ(premadeFiveByThreeLossGame.visualise<std::vector<int>>(), premadeFiveByThreeVisualVectorExpected);

    // empty and unstarted 8x4 game
    minesweeper::Game emptyGame(8, 4, 12, &myRandom);
    std::list<minesweeper::VisualMinesweeperCell> emptyVisualListExpected(
        32, minesweeper::VisualMinesweeperCell::UNCHECKED);
    EXPECT_EQ(emptyGame.visualise<std::list<minesweeper::VisualMinesweeperCell>>(), emptyVisualListExpected);
    std::vector<std::list<minesweeper::VisualMinesweeperCell>> emptyVisualVectorListExpected(
        8, std::list<minesweeper::VisualMinesweeperCell>(4, minesweeper::VisualMinesweeperCell::UNCHECKED));
    EXPECT_EQ(emptyGame.visualise<std::vector<std::list<minesweeper::VisualMinesweeperCell>>>(),
              emptyVisualVectorListExpected);
}

TEST_F(MinesweeperGameTest, VisualiseCellSolutionTest) {

    // negative x and y
    minesweeper::Game negativeXandYVisualiseCellSolutionGame(10, 16, 0.24, &myRandom);
    EXPECT_THROW(negativeXandYVisualiseCellSolutionGame.visualiseCellSolution(-5, -7), std::out_of_range);

    // negative x
    minesweeper::Game negativeXVisualiseCellSolutionGame(15, 24, &myRandom);
    EXPECT_THROW(negativeXVisualiseCellSolutionGame.visualiseCellSolution(-11, 7), std::out_of_range);

    // negative y
    minesweeper::Game negativeYVisualiseCellSolutionGame(7, 8, 0.4455, &myRandom);
    EXPECT_THROW(negativeYVisualiseCellSolutionGame.visualiseCellSolution(2, -1), std::out_of_range);

    // too large x and y
    minesweeper::Game tooLargeXandYVisualiseCellSolutionGame(12, 7, 43, &myRandom);
    EXPECT_THROW(tooLargeXandYVisualiseCellSolutionGame.visualiseCellSolution(9, 22), std::out_of_range);

    // too large x
    minesweeper::Game tooLargeXVisualiseCellSolutionGame(11, 0.1232, &myRandom);
    EXPECT_THROW(tooLargeXVisualiseCellSolutionGame.visualiseCellSolution(11, 6), std::out_of_range);

    // too large y
    minesweeper::Game tooLargeYVisualiseCellSolutionGame(8, 7, 18, &myRandom);
    EXPECT_THROW(tooLargeYVisualiseCellSolutionGame.visualiseCellSolution(0, 14), std::out_of_range);

    // negative x and too large y
    minesweeper::Game negativeXandTooLargeYVisualiseCellSolutionGame(24, 7, 0.34, &myRandom);
    EXPECT_THROW(negativeXandTooLargeYVisualiseCellSolutionGame.visualiseCellSolution(-1, 27), std::out_of_range);

    // valid/expected template types
    minesweeper::Game validTemplateTypes(9, 12, 0.423, &myRandom);
    EXPECT_NO_THROW(validTemplateTypes.checkInputCoordinates(6, 3));
    minesweeper::VisualMinesweeperCell cellSolutionVisualisation;
    EXPECT_NO_THROW(cellSolutionVisualisation =
                        validTemplateTypes.visualiseCellSolution<minesweeper::VisualMinesweeperCell>(7, 4));
    int cellIntSolutionVisualisation;
    EXPECT_NO_THROW(cellIntSolutionVisualisation = validTemplateTypes.visualiseCellSolution<int>(7, 4));
    char cellCharSolutionVisualisation;
    EXPECT_NO_THROW(cellCharSolutionVisualisation = validTemplateTypes.visualiseCellSolution<char>(7, 4));
    EXPECT_EQ(static_cast<int>(cellSolutionVisualisation), cellIntSolutionVisualisation);
    EXPECT_EQ(static_cast<int>(cellCharSolutionVisualisation), cellIntSolutionVisualisation);
    EXPECT_EQ(static_cast<minesweeper::VisualMinesweeperCell>(cellCharSolutionVisualisation),
              cellSolutionVisualisation);

    // custom template type
    class VisualTestCell {
      public:
        VisualTestCell(minesweeper::VisualMinesweeperCell x) {}
    };
    EXPECT_NO_THROW(validTemplateTypes.visualiseCellSolution<VisualTestCell>(7, 4));

    // unfinished 7x7 game
    minesweeper::Game premadeUnfinishedSevenBySevenGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::unfinishedGame_sevenBySeven_serialisation____json],
                            premadeUnfinishedSevenBySevenGame));
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(0, 0), minesweeper::VisualMinesweeperCell::MINE);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(6, 4), minesweeper::VisualMinesweeperCell::THREE);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(1, 4), minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(3, 2), minesweeper::VisualMinesweeperCell::TWO);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(3, 5), minesweeper::VisualMinesweeperCell::ONE);
    EXPECT_EQ(premadeUnfinishedSevenBySevenGame.visualiseCellSolution(5, 6), minesweeper::VisualMinesweeperCell::FOUR);

    // started 5x6 game
    minesweeper::Game premadeStartedFiveBySixGame;
    ASSERT_NO_THROW(deserialiseFromFile(
        minesweeper::TEST_DATA[minesweeper::startedGame_fiveBySix_serialisation____json], premadeStartedFiveBySixGame));
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(4, 3), minesweeper::VisualMinesweeperCell::FOUR);
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(2, 1), minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(5, 0), minesweeper::VisualMinesweeperCell::ONE);
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(1, 3), minesweeper::VisualMinesweeperCell::MINE);
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(3, 3), minesweeper::VisualMinesweeperCell::THREE);
    EXPECT_EQ(premadeStartedFiveBySixGame.visualiseCellSolution(1, 2), minesweeper::VisualMinesweeperCell::TWO);

    // lost 5x3 game
    minesweeper::Game premadeLostFiveByThreeGame;
    ASSERT_NO_THROW(deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::lossGame_fiveByThree_serialisation____json],
                                        premadeLostFiveByThreeGame));
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(2, 1), minesweeper::VisualMinesweeperCell::ONE);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(0, 1), minesweeper::VisualMinesweeperCell::MINE);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(1, 3), minesweeper::VisualMinesweeperCell::FIVE);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(2, 4), minesweeper::VisualMinesweeperCell::MINE);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(1, 1), minesweeper::VisualMinesweeperCell::TWO);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(2, 0), minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(premadeLostFiveByThreeGame.visualiseCellSolution(1, 2), minesweeper::VisualMinesweeperCell::THREE);
}

TEST_F(MinesweeperGameTest, VisualiseSolutionTest) {

    // valid/expected template types
    minesweeper::Game validTemplateTypes(12, 0.4, &myRandom);
    EXPECT_NO_THROW(validTemplateTypes.checkInputCoordinates(8, 3));
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::vector<minesweeper::VisualMinesweeperCell>>());
    EXPECT_NO_THROW(
        validTemplateTypes.visualiseSolution<std::vector<std::vector<minesweeper::VisualMinesweeperCell>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::vector<int>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::vector<std::vector<int>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::string>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::vector<std::string>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::list<int>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::list<minesweeper::VisualMinesweeperCell>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::vector<std::list<int>>>());
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<std::list<std::vector<minesweeper::VisualMinesweeperCell>>>());

    // custom template type (with minimal requirements)
    class CustomExpandSeqContainer {
      public:
        // needed to deduce type of values to cast from minesweeper::VisualMinesweeperCell
        using value_type = int;

        // needed to push_back values
        void push_back(value_type x) {}

        // not needed for anything, but expected for any serious container
        size_t size() { return 0; }
    };
    EXPECT_NO_THROW(validTemplateTypes.visualiseSolution<CustomExpandSeqContainer>());

    // premade marked-win 5x4 game
    minesweeper::Game premadeFiveByFourMarkedWinGame;
    ASSERT_NO_THROW(
        deserialiseFromFile(minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json],
                            premadeFiveByFourMarkedWinGame));
    std::vector<std::vector<int>> premadeFiveByFourSolutionMatrixExpected{
        {9, 9, 3, 9}, {3, 9, 3, 1}, {1, 1, 1, 0}, {2, 2, 1, 0}, {9, 9, 1, 0}};
    EXPECT_EQ(premadeFiveByFourMarkedWinGame.visualiseSolution<std::vector<std::vector<int>>>(),
              premadeFiveByFourSolutionMatrixExpected);
    std::vector<int> premadeFiveByFourSolutionVectorExpected{9, 9, 3, 9, 3, 9, 3, 1, 1, 1,
                                                             1, 0, 2, 2, 1, 0, 9, 9, 1, 0};
    EXPECT_EQ(premadeFiveByFourMarkedWinGame.visualiseSolution<std::vector<int>>(),
              premadeFiveByFourSolutionVectorExpected);

    // empty and unstarted 7x3 game
    minesweeper::Game emptyGame(7, 3, 0.33, &myRandom);
    std::list<minesweeper::VisualMinesweeperCell> emptySolutionListExpected(21,
                                                                            minesweeper::VisualMinesweeperCell::EMPTY);
    EXPECT_EQ(emptyGame.visualiseSolution<std::list<minesweeper::VisualMinesweeperCell>>(), emptySolutionListExpected);
    std::vector<std::list<minesweeper::VisualMinesweeperCell>> emptySolutionVectorListExpected(
        7, std::list<minesweeper::VisualMinesweeperCell>(3, minesweeper::VisualMinesweeperCell::EMPTY));
    EXPECT_EQ(emptyGame.visualiseSolution<std::vector<std::list<minesweeper::VisualMinesweeperCell>>>(),
              emptySolutionVectorListExpected);
}

TEST_F(MinesweeperGameTest, DefaultRandomTest) {

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

TEST(MinesweeperGameStaticTest, MaxNumOfMinesMethodTest) {

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

    // very large grid
    EXPECT_EQ(minesweeper::Game::maxNumOfMines(10'020, 20'458), 204'989'151);
}

TEST(MinesweeperGameStaticTest, MaxProportionOfMinesMethodTest) {

    // completely negative grid
    EXPECT_THROW(minesweeper::Game::maxProportionOfMines(-4, -8), std::out_of_range);

    // negative height grid
    EXPECT_THROW(minesweeper::Game::maxProportionOfMines(-5, 8), std::out_of_range);

    // negative width grid
    EXPECT_THROW(minesweeper::Game::maxProportionOfMines(4, -9), std::out_of_range);

    // zero size grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(0, 0), 0);

    // zero height grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(0, 7), 0);

    // zero width grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(412, 0), 0);

    // 1x1 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(1, 1), 0);

    // 2x2 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(2, 2), 0);

    // 3x3 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(3, 3), 0);

    // 4x4 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(4, 4), 0.4375);

    // 5x5 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(5, 5), 0.64);

    // 10x10 grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(10, 10), 0.91);

    // normal grid
    EXPECT_EQ(minesweeper::Game::maxProportionOfMines(10, 9), 0.9);

    // another normal grid
    EXPECT_TRUE(std::abs(minesweeper::Game::maxProportionOfMines(13, 15) - 0.953'846'153'846'154) <
                0.000'000'000'000'001);

    // large grid
    EXPECT_TRUE(std::abs(minesweeper::Game::maxProportionOfMines(416, 154) - 0.999'859'515'484'515) <
                0.000'000'000'000'001);

    // very large grid
    EXPECT_TRUE(std::abs(minesweeper::Game::maxProportionOfMines(21'067, 13'259) - 0.999'999'967'779'740) <
                0.000'000'000'000'001);
}

TEST(MinesweeperGameStaticTest, MinNumOfMinesMethodTest) {

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

TEST(MinesweeperGameStaticTest, MinProportionOfMinesMethodTest) {

    // unspecified grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(), 0);

    // completely negative grid
    EXPECT_THROW(minesweeper::Game::minProportionOfMines(-4, -6), std::out_of_range);

    // negative height grid
    EXPECT_THROW(minesweeper::Game::minProportionOfMines(-7, 4), std::out_of_range);

    // negative width grid
    EXPECT_THROW(minesweeper::Game::minProportionOfMines(8, -3), std::out_of_range);

    // zero size grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(0, 0), 0);

    // zero height grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(0, 7), 0);

    // zero width grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(5, 0), 0);

    // 1x1 grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(1, 1), 0);

    // 2x2 grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(2, 2), 0);

    // normal grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(11, 9), 0);

    // large grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(134, 157), 0);

    // very large grid
    EXPECT_EQ(minesweeper::Game::minProportionOfMines(25'221, 18'476), 0);
}
