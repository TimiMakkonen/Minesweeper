

#include <gtest/gtest.h>

#include <minesweeper/game.h>
#include <minesweeper/random.h>

class MinesweeperTest : public ::testing::Test {
protected:

	// used for setting up fields for testing
	void SetUp() override {

		this->setUpField1();
	}

	void setUpField1() {

		field1 = 13;
	}



	// declarations:

	int field1;
	minesweeper::Random myRandom;
};

using MinesweeperDeathTest = MinesweeperTest;


TEST_F(MinesweeperTest, ConstructorTest) {

	// most basic/expected constructor
	minesweeper::Game myGame(10, 11, 20, &myRandom);
	EXPECT_EQ(myGame.getGridHeight(), 10);
	EXPECT_EQ(myGame.getGridWidth(), 11);
	EXPECT_EQ(myGame.getNumOfMines(), 20);

	// square grid size constructor
	minesweeper::Game mySquareGame(23, 14, &myRandom);
	EXPECT_EQ(mySquareGame.getGridHeight(), 23);
	EXPECT_EQ(mySquareGame.getGridWidth(), 23);
	EXPECT_EQ(mySquareGame.getNumOfMines(), 14);

	// basic constructor with random statically set or not set at all
	minesweeper::Game myGame3(8, 5, 4);
	EXPECT_EQ(myGame3.getGridHeight(), 8);
	EXPECT_EQ(myGame3.getGridWidth(), 5);
	EXPECT_EQ(myGame3.getNumOfMines(), 4);

	// square grid size constructor with random statically set or not set at all
	minesweeper::Game myGame4(6, 7);
	EXPECT_EQ(myGame4.getGridHeight(), 6);
	EXPECT_EQ(myGame4.getGridWidth(), 6);
	EXPECT_EQ(myGame4.getNumOfMines(), 7);

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

TEST_F(MinesweeperTest, CreateMinesAndNumsTest) { // TODO

}

TEST_F(MinesweeperTest, CheckInputCoordinatesTest) { // TODO

}

TEST_F(MinesweeperTest, MarkInputCoordinatesTest) { // TODO

}

TEST_F(MinesweeperTest, PlayerHasWonTest) { // TODO

}

TEST_F(MinesweeperTest, PlayerHasLostTest) { // TODO

}

TEST_F(MinesweeperTest, IsCellVisibleTest) { // TODO

}

TEST_F(MinesweeperTest, DoesCellHaveMineTest) { // TODO

}

TEST_F(MinesweeperTest, IsCellMarkedTest) { // TODO

}

TEST_F(MinesweeperTest, NumOfMinesAroundCellTest) { // TODO

}

TEST_F(MinesweeperTest, ResetTest) { // TODO

	minesweeper::Game myGame(10, 20, &myRandom);

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