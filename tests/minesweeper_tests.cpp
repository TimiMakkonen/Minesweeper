#include <fstream> // std::ifstream
#include <sstream> // std::stringstream

#include <gtest/gtest.h>

#include <test_config.h>

#include <minesweeper/game.h>
#include <minesweeper/random.h>

class MinesweeperTest : public ::testing::Test {
protected:

	// used for setting up fields for testing
	void SetUp() override {

		setSerialisationStrings();
	}



	void setSerialisationStrings() {

		startedFiveBySixGameStr = fileContentsToString(
			minesweeper::TEST_DATA[minesweeper::startedGame_fiveBySix_serialisation____json]
		);
	}

	// declarations:

	int field1;
	minesweeper::Random myRandom;
	std::string startedFiveBySixGameStr;



	// utility methods

	std::string fileContentsToString(std::string filePath) {

		std::ifstream file(filePath);
		std::ostringstream ss;
		ss << file.rdbuf();
		
		return ss.str();
	}


	std::string serialiseToString(minesweeper::Game& game) {
		
		std::ostringstream oss;
		game.serialise(oss);

		return oss.str();
	}

	// print functionality just for 
	std::string printGameToString(minesweeper::Game& gameToPrint) {

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


TEST_F(MinesweeperTest, CreateMinesAndNumsTest) { // TODO

}


TEST_F(MinesweeperTest, CheckInputCoordinatesTest) { // TODO

}


TEST_F(MinesweeperTest, MarkInputCoordinatesTest) { // TODO

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
		if (countOfNonTrivialEasyWins > 1 && countOfNonTrivialCase1Wins > 1
			&& countOfNonTrivialCase2Wins > 1 && countOfNonTrivialCase3Wins > 1
			&& countOfNonTrivialCase4Wins > 1) {
			allNonTrivialCasesExplored = true;
		}

		// if cases were not explored at least twice, repeat test
	} while (!allNonTrivialCasesExplored);


	// premade marked win
	minesweeper::Game premadeMarkedWinGame;
	std::ifstream premadeMarkedWinGameStream(
		minesweeper::TEST_DATA[minesweeper::markedWinGame_fiveByFour_serialisation____json]
	);
	premadeMarkedWinGame.deserialise(premadeMarkedWinGameStream);
	EXPECT_TRUE(premadeMarkedWinGame.playerHasWon()) << printGameToString(premadeMarkedWinGame);


	// premade checked win
	minesweeper::Game premadeCheckedWinGame;
	std::ifstream premadeCheckedWinGameStream(
		minesweeper::TEST_DATA[minesweeper::checkedWinGame_fourByEight_serialisation____json]
	);
	premadeCheckedWinGame.deserialise(premadeCheckedWinGameStream);
	EXPECT_TRUE(premadeCheckedWinGame.playerHasWon()) << printGameToString(premadeCheckedWinGame);


	// fully checked non-win
	minesweeper::Game fullyCheckedNonWinGame(9, 20, &myRandom);
	fullyCheckedNonWinGame.checkInputCoordinates(5, 7);
	for(int y = 0; y < 9; ++y) {
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


TEST_F(MinesweeperTest, PlayerHasLostTest) { // TODO : add more testcases when serialisation is available

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
											{1, 11, 2, 7, 0},
											{48, 50, 47, 28, 39},
											{13, 9, 10, 8, 7},
											{45, 46, 38, 24, 32},
											{6, 19, 20, 15, 4},
											{8, 16, 25, 4, 3},
											{2, 22, 25, 20, 1},
											{45, 37, 33, 30, 27},
											{44, 31, 26, 23, 2},
											{4, 7, 11, 3, 1}
										};

	for (auto gameParas : gameParameterSet ) {
		
		minesweeper::Game notFirstMoveLossGame(gameParas.gridHeight, gameParas.gridWidth,
				gameParas.numOfMines, &myRandom);
		
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



}


TEST_F(MinesweeperTest, IsCellVisibleTest) { // TODO

}


TEST_F(MinesweeperTest, DoesCellHaveMineTest) { // TODO

}


TEST_F(MinesweeperTest, IsCellMarkedTest) { // TODO

}


TEST_F(MinesweeperTest, NumOfMinesAroundCellTest) { // TODO

}


TEST_F(MinesweeperTest, SerialiseTest) { // TODO

	// empty game serialisation
	minesweeper::Game emptyGame(0, 0, &myRandom);
	std::string emptyGameStr = serialiseToString(emptyGame);
	std::string emptyGameFileStr = fileContentsToString(
		minesweeper::TEST_DATA[minesweeper::emptyGame_serialisation____json]
	);
	EXPECT_EQ(emptyGameStr, emptyGameFileStr);




	// simple game serialisation
	minesweeper::Game simpleGame(7, 4, 13, &myRandom);
	std::string simpleGameStr = serialiseToString(simpleGame);
	std::string simpleGameFileStr = fileContentsToString(
		minesweeper::TEST_DATA[minesweeper::unstartedGame_sevenByFour_serialisation____json]
	);
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
	simpleStartedGame.checkInputCoordinates(2,1);
	std::string simpleStartedGameStr = serialiseToString(simpleStartedGame);
	EXPECT_EQ(simpleStartedGameStr, startedFiveBySixGameStr);

}


TEST_F(MinesweeperTest, DeserialiseTest) { // TODO

}


TEST_F(MinesweeperTest, SerialiseAndDeserialiseTest) { // TODO

	// +---------------------------+
	// | empty game serialisation: |
	// +---------------------------+
	minesweeper::Game emptyGame(0, 0, &myRandom); //initialise empty game
	std::stringstream emptyGameStream; // create a stringstream
	emptyGame.serialise(emptyGameStream); // serialise empty game
	std::string emptyGameStr = emptyGameStream.str(); // make string version of this serialisation

	minesweeper::Game emptyGameCopy; // make new empty game
	emptyGameCopy.deserialise(emptyGameStream); // deserialise this new game with serialisation of the previous one
	std::ostringstream emptyGameCopyStream; // create a new ostringstream
	emptyGameCopy.serialise(emptyGameCopyStream); // serialise new empty game
	std::string emptyGameCopyStr = emptyGameCopyStream.str(); // make string version of this new serialisation

	EXPECT_EQ(emptyGame.getGridHeight(), emptyGameCopy.getGridHeight());
	EXPECT_EQ(emptyGame.getGridWidth(), emptyGameCopy.getGridWidth());
	EXPECT_EQ(emptyGame.getNumOfMines(), emptyGameCopy.getNumOfMines());
	EXPECT_EQ(emptyGame.playerHasLost(), emptyGameCopy.playerHasLost());
	EXPECT_EQ(emptyGame.playerHasWon(), emptyGameCopy.playerHasWon());
	EXPECT_EQ(emptyGameStr, emptyGameCopyStr);

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

