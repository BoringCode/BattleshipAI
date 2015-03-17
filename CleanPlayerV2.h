/**
 * @author Stefan Brandle
 * @date April, 2004
 * Lab 15: CleanPlayer AI. Decent player with very simple logic.
 *
 * Please type in your name[s] here:
 */

#ifndef CLEANPLAYERV2_H		// Double inclusion protection
#define CLEANPLAYERV2_H

#include "PlayerV2.h"

using namespace std;

class CleanPlayerV2: public PlayerV2 {
    public:
	CleanPlayerV2( int boardSize );
	~CleanPlayerV2() {};
	void newRound();
	Message placeShip( int length );
	Message getMove( );
	void update( Message msg );

    private:
	// Local functions
	void getFollowUpShot( int& row, int& col );
	bool search( int& startRow, int& startCol, int rowDelta, int colDelta );
	void scan( int& row, int& col );
	bool isOnBoard( int row, int col );
	void chooseValues( int& row, int& col, int& length, Direction& dir );
	bool positionOk( int row, int col, int length, Direction dir );
	void markShip( int row, int col, int length, Direction dir );
	void initializeBoard(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);

	// Local data
        int scanRow;
        int scanCol;
	int numShipsPlaced;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	char shipBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	int opponentShots[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	char shipMark;

};	// end of class CleanPlayerV2

#endif
