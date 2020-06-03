
#include <iostream>

#include <minesweeper/Grid.h>
#include <minesweeper/Random.h>

// Used for showing off the Minesweeper library and maybe writing some preliminary tests

// function declarations:
void preliminaryTests();
void examples();
void exampleOfSimplestSetup();
void examplesOfRandomControllingSetup();
void usageExamples();


// (This can be used as a playground to see and use this library in action.)
void preliminaryTests() {

	// You can write some preliminary tests here
}

void examples() {

	// You can write some examples here

	exampleOfSimplestSetup();

	examplesOfRandomControllingSetup();

	usageExamples();
}



int main() {

	preliminaryTests();

	examples();
	
	return 0;
}


void exampleOfSimplestSetup() {

	// start by initialising instance of 'Random' class
	// (can be initialised to stack or heap)
	Minesweeper::Random myRandom;

	// pass grid size (10x10), number of mines(20) and pointer to 'Random'(&myRandom)
	// (can be initialised to stack or heap)
	Minesweeper::Grid myGrid(10, 20, &myRandom);

	// you can also seperately specify width and height of the grid:
	// pass grid height (9), grid width (12), number of mines(22) and pointer to 'Random'(&myRandom)
	Minesweeper::Grid myGrid2(9, 12, 22, &myRandom);




	// alternatively you can initialise 'Random' or 'Grid' to heap:
	Minesweeper::IRandom* myRandomPtr = new Minesweeper::Random();
	Minesweeper::Grid* myGridPtr = new Minesweeper::Grid(10, 20, myRandomPtr);

	delete myGridPtr;
	delete myRandomPtr;




	// or use better C++ 'std::unique_ptr' alternative to this: (takes care of garbage collection)

	auto myRandomUniquePtr = std::make_unique<Minesweeper::Random>();
	// std::unique_ptr<Minesweeper::IRandom> myRandomUniquePtr = std::make_unique<Minesweeper::Random>();

	auto myGridUniquePtr = std::make_unique<Minesweeper::Grid>(10, 20, myRandomUniquePtr.get());
	// std::unique_ptr<Minesweeper::Grid> myGridUniquePtr 
	//		= std::make_unique<Minesweeper::Grid>(10, 20, myRandomUniquePtr.get());


	// As of writing this, 'Grid' does not ( and hopefully never will) delete the 'IRandom' object it has a pointer for
	// but beware that exposing the raw pointer from 'std::unique_ptr' is still dangerous.
	// So it might be advisable to use raw pointer for your 'Random' class instead.



	// instead off assigning 'IRandom' for each instance of 'Grid', 
	// you can assign a default 'Random' to all instances of 'Grid' class
	// this gets used if it is not specifically set for an instance of 'Random' class
	Minesweeper::Random myStaticRandom;

	Minesweeper::Grid::setDefaultRandom(&myStaticRandom);

	// grid that uses static random:
	Minesweeper::Grid myGridUsingStaticRandom(10, 20);

	// grid that manually overrides static random:
	Minesweeper::Random myRandom2;
	Minesweeper::Grid myGridOverridingStaticRandom(10, 20, &myRandom2);


	// be free to use any of these ways to setup your Minesweeper 'Grid'
	// just make sure that 'Random' variable is alive throughout your usage of the Minesweeper 'Grid' :)
}



void examplesOfRandomControllingSetup() {

	// I recommend quickly reading through 'exampleOfSimplestSetup()' first to learn basic setup options.

	// 'Random' uses 'std::mt19937'
	// This is automatically seeded by 'std::random_device' when 'Random' gets contructed.

	// Instead of using provided 'Random' class, you can write your own class which implements abstract 'IRandom' class.
	// This allows you to take control of how you handle randomness.
	// This is especially useful if you need randomness anywhere else in your program, and you want to make sure that
	// 	this behaviour stays consistent throught your program. (Same RNG engine used everywhere, etc.)

	// As of writing this, the only method you need to implement from 'IRandom' is:
	// void shuffleVector(std::vector<int>& vec)



	// for example:
	class MyRandomGen : public Minesweeper::IRandom {
		public:
			// just an example, shuffle properly in actual use
			void shuffleVector(std::vector<int>& vec) {
				std::reverse(vec.begin(), vec.end());
			}
	};

	MyRandomGen myRandomGen;
	auto myGrid = std::make_unique<Minesweeper::Grid>(10, 20, &myRandomGen);

}



void usageExamples() {

	// Here we are going to present basic usage of the minesweeper library.
	// For more detailed public interface, check 'Grid.h' file. (include/minesweeper/Grid.h as of writing this)

	// Let us start with a basic setup of our Minesweeper game:
	// (check exampleOfSimplestSetup() and examplesOfRandomControllingSetup() methods for more information on this)

	Minesweeper::Random myRandom;
	// Grid with: grid height (10), grid width (12), number of mines(22) and pointer to 'Random'(&myRandom)
	auto myGrid = std::make_unique<Minesweeper::Grid>(10, 12, 22, &myRandom);

	// A minesweeper game is automatically initialised into an empty game of specialised size.
	// However, the grid does not generate mines before the first move has been made.
	// This is to make sure that the player cannot lose straight away.
	
	// The grid uses common programming coordinate system with: 
	//			top left corner (0, 0)
	//			top right corner (gridWidth - 1, 0)
	//			bottom left corner (0, gridHeight - 1)
	//			bottom right corner (gridWidth - 1, gridHeight - 1)

	// When coordinates are needed as an argument to methods, 
	// 	x-coordinate is always given first, followed by y-coordinate

	// For reference, specific spots on the grid are known as 'Cells'.

	// For now, (until this feature gets updated), creation of mines has to be done manually.
	myGrid->createMinesAndNums(5, 6);

	// Player can check/click coordinates:
	myGrid->checkInputCoordinates(5,6);

	// Player can mark coordinates:
	myGrid->markInputCoordinates(3, 6);


	// We can do a number of checks: (to assist with visualisation of game etc.)
	// Small macro to print method names and values:
	#define PRINT_FUNC_RESULT(func) std::cout << #func << ": " << func << std::endl

	PRINT_FUNC_RESULT(myGrid->playerHasWon());
	PRINT_FUNC_RESULT(myGrid->playerHasLost());
	PRINT_FUNC_RESULT(myGrid->isCellVisible(5, 6));
	PRINT_FUNC_RESULT(myGrid->doesCellHaveMine(1, 3));
	PRINT_FUNC_RESULT(myGrid->isCellMarked(3, 6));
	PRINT_FUNC_RESULT(myGrid->numOfMinesAroundCell(4, 6));

	#undef PRINT_FUNC_RESULT

	// All of these are quite self-explanatory.
	// There is quite a few improvements and additions on the way.
	// Again, for more info, check 'Grid.h' file. (include/minesweeper/Grid.h as of writing this.
}

