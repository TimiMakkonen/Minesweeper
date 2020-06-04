

#include <gtest/gtest.h>

#include <minesweeper/game.h>

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
};

using MinesweeperDeathTest = MinesweeperTest;

TEST_F(MinesweeperDeathTest, test_name) { // TODO


	EXPECT_EQ(1, 1) << "Test failed message.";

	EXPECT_EQ(0, 0);

}

TEST_F(MinesweeperDeathTest, another_test_name) { // TODO

	int x = 1;
	EXPECT_EQ(x, 1) << "Test failed message.";

	EXPECT_EQ(0, 0);

}