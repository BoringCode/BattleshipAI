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
#include <queue>

// DumbPlayer inherits from/extends PlayerV2

//Probability numbers
const int P_HISTORY = 0;
const int P_SHIPFOUND = 100;
const int P_DEFAULT = 0; 


class BJZPlayer: public PlayerV2 {
    public:
	BJZPlayer( int boardSize );
	~BJZPlayer();
	void newRound();
	Message placeShip(int length);
	Message getMove();
	void update(Message msg);
    
    private:
	
	queue<int> shipLengths;

	//functions
	void initializeBoard();
	void initializePboard();
	void boardP(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);
	void highestP();
	int cellP(int row,int col, int boardSize, char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]);
	void updateEnemyShipPlacementHistory(Message msg);
	
	//data
	int numShipsPlaced;
        char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	int Pboard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	int HPBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	int returnRow;
	int returnCol;
        int lastRow;
        int lastCol;
	queue<Message> enemyShipPlacementHistory;
	int enemyShotBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];	
	
	int placedShipsBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	
	int minShipLength();
	void reportKill();
	int killCount;
	int prevKillCount;
	bool reportingKill;
};

#endif
