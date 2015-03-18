/**
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004
 *
 * Please type in your name[s] here:
 *
 */

#ifndef DUMBPLAYERV2_H		// Double inclusion protection
#define DUMBPLAYERV2_H

using namespace std;

#include "PlayerV2.h"
#include "Message.h"
#include "defines.h"

// DumbPlayer inherits from/extends PlayerV2

class BJZPlayer: public PlayerV2 {
    public:
	BJZPlayer( int boardSize );
	~BJZPlayer();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);

    private:
	void initializeBoard();
	void initializePboard();
	int numShipsPlaced;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	
	//functions
	void boardP(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);
	void highestP();
	int cellP(int row,int col, int boardSize, char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);

	// FTW data
	int Pboard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	int returnRow;
	int returnCol;
        int lastRow;
        int lastCol;
};

#endif
