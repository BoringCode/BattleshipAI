/**
 * Authors Stefan Brandle and Jonathan Geisler
 * Date    November, 2004
 * Declaration source file for BoardV3.
 * Please type in your name[s] below:
 *
 */

#ifndef BOARDV3_H
#define BOARDV3_H

#include "defines.h"

using namespace std;

class BoardV3 {
    public:
	// Constructors and such
	BoardV3( int width);		
	BoardV3(const BoardV3& other);
	void operator=(const BoardV3& other);
	// General public access functions.
	bool placeShip(int row, int col, int length, Direction dir);
    	char getOpponentView(int row, int col);
    	char getOwnerView(int row, int col);
    	char processShot(int row, int col);
    	bool hasWon();

        const static int MaxBoardSize = 10;     // Maximum board size.

    private:
	// Put prototypes for your private helper functions here.
	void initialize(char board[MaxBoardSize][MaxBoardSize]);
	bool positionOk(int row, int col, int length, bool horiz);
	void markShip(int row, int col, int length, bool horiz);
        bool isSunk(int row, int col);
        void markSunk(int row, int col);

	// Suggested data variables. Although you can ignore these, it
	// is suggested that you use them. If you change the array definitions,
	// you may have to change some of the prototypes or code that 
	// you are given as part of your starter kit.
	char shipBoard[MaxBoardSize][MaxBoardSize];    // Tracks the placed ships and shots
	char shotBoard[MaxBoardSize][MaxBoardSize];    // Tracks where ships are through whole game
	int boardSize;		    // Tracks how many rows/cols are actually being used.

	// Put your private data variables here.
	char shipMark;		    // Used to mark the ships on shipBoard
};

#endif	// End of multiple inclusion control.
