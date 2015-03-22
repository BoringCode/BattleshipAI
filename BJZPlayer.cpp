/**
 * @author Stefan Brandle, Jonathan Geisler
 * @date September, 2004 Updated 2015 for multi-round play.
 *
 * Please type in your name[s] here:
 */

#include <iostream>
#include <cstdio>
#include "conio.h"
#include "BJZPlayer.h"

using namespace conio;

/**
 * @brief Constructor that initializes any inter-round data structures.
 * @param boardSize Indication of the size of the board that is in use.
 */
BJZPlayer::BJZPlayer( int boardSize ):
    PlayerV2(boardSize)
{
	//Initialize enemy shot board for ship placement
	for(int row=0; row<boardSize; row++) {
		for(int col=0; col<boardSize; col++) {
			this->enemyShotBoard[row][col] = 0;
		}
	}
}

/**
 * @brief Destructor placeholder.
 */
BJZPlayer::~BJZPlayer( ) {}

/*
 * Private internal function that initializes a MAX_BOARD_SIZE 2D array of char to water.
 */
void BJZPlayer::initializeBoard() {
    for(int row=0; row<boardSize; row++) {
	for(int col=0; col<boardSize; col++) {
	    this->board[row][col] = WATER;
	}
    }
}

void BJZPlayer::initializePboard() {
    for (int i=0; i < boardSize; i++) {
	    for (int j=0; j < boardSize; j++) Pboard[i][j] = 0;
    }
	if (!enemyShipPlacementHistory.empty()) {
		cout << "Enemy ship board " <<  this->enemyShipPlacementHistory.size() << endl;
		int* hit = this->enemyShipPlacementHistory.front();
		this->enemyShipPlacementHistory.pop();
		cout << "Row " << hit[0] << " Col " << hit[1] << endl;
/*		if (!enemyShipPlacementHistory.empty()) {
			int* hit2 = enemyShipPlacementHistory.front();
			enemyShipPlacementHistory.pop();
			cout << "Row " << hit2[0] << " Col " << hit2[1] << endl;
		}*/
	}
/*
	for (size_t i=0; i < enemyShipPlacementHistory.size(); i++) {
		hit = enemyShipPlacementHistory.front();
		enemyShipPlacementHistory.pop();
		cout << "Row " << hit[0] << " Col " << hit[1] << endl;
		//Pboard[hit[0]][hit[1]]++;
		//enemyShipPlacementHistory.push(hit);
	}*/	
	for (int row=0; row < boardSize; row++) {
		cout << endl;
		for (int col=0; col<boardSize; col++) {
			cout << Pboard[row][col] << " ";
		}
	}
}


/**
 * Gets the computer's shot choice. This is then returned to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 * Position 0 of the int array should hold the row, position 1 the column.
 */
Message BJZPlayer::getMove() {
    boardP(board);
    highestP();
    Message result( SHOT, returnRow, returnCol, "Bang", None, 1 );
    return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void BJZPlayer::newRound() {
    /* DumbPlayer is too simple to do any inter-round learning. Smarter players reinitialize any
     * round-specific data structures here.
     */
    this->lastRow = 0;
    this->lastCol = -1;
    this->numShipsPlaced = 0;

    this->initializeBoard();
    this->initializePboard();
}

/**
 * Gets the AI's ship placement choice. This is then returned to the caller.
 * @param length The length of the ship to be placed.
 * @return Message The most important parts of the returned message are 
 * the direction, row, and column values. 
 * Position 0 of the int array should hold the row, position 1 the column.
 */
Message BJZPlayer::placeShip(int length) {
    char shipName[10];
    // Create ship names each time called: Ship0, Ship1, Ship2, ...
    snprintf(shipName, sizeof shipName, "Ship%d", numShipsPlaced);

    // parameters = mesg type (PLACE_SHIP), row, col, a string, direction (Horizontal/Vertical)
    Message response( PLACE_SHIP, numShipsPlaced, 0, shipName, Horizontal, length );
    numShipsPlaced++;

    return response;
}

/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void BJZPlayer::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
		board[msg.getRow()][msg.getCol()] = msg.getMessageType();
		updateEnemyShipPlacementHistory(msg);
		break;
	case KILL:
		board[msg.getRow()][msg.getCol()] = msg.getMessageType();
		break;
	case MISS:
	    board[msg.getRow()][msg.getCol()] = msg.getMessageType();
	    break;
	case OPPONENT_SHOT:
		enemyShotBoard[msg.getRow()][msg.getCol()]++;
		break;
    }
}




// boardP
void BJZPlayer::boardP(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]) {
	for (int row=0; row < boardSize; row++) {
		for (int col=0; col < boardSize; col++){
			returnRow=row;
			returnCol=col;
			if (board[row][col] == WATER) {
				Pboard[row][col] = cellP(row,col,boardSize, board);
			}
			else {
				Pboard[row][col] = 0;
			}
		}
	}
}

// cellP
int BJZPlayer::cellP(int row,int col, int boardSize, char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]){
	int totalP=0;
	int badSpots;
	int hitCount;
	for (int shipSize=3; shipSize<= 4; shipSize++) {
		for (int end=0; end < shipSize; end++) {
			badSpots = 0;
			hitCount = 0;
			for (int forward=0; forward < (shipSize - end); forward++) {
				if ((forward+col) > (boardSize-1)) badSpots++;
				else {
					if (board[row][col+forward] == HIT) hitCount = hitCount + 100;
					else if (board[row][col+forward] != WATER) badSpots++;
				}
			}
			for (int back=0; back <= end; back++) {
				if ((col-back) < 0) badSpots++;
				else {
					if (board[row][col-back] == HIT) hitCount = hitCount + 100;
					else if (board[row][col-back] != WATER) badSpots++;
				}
			}
			if (badSpots == 0) totalP = totalP + 1 + hitCount;
		}
		for (int end=0; end < shipSize; end++) {
			badSpots = 0;
			hitCount = 0;
			for (int up=0; up < (shipSize - end); up++) {
				if ((row-up) < 0) badSpots++;
				else {
					if (board[row-up][col] == HIT) hitCount = hitCount + 100;
					else if (board[row-up][col] != WATER) badSpots++;
				}
			}
			for (int down=0; down <= end; down++) {
				if ((row+down) > boardSize-1) badSpots++;
				else {
					if (board[row+down][col] == HIT) hitCount = hitCount + 100;
					else if (board[row+down][col] != WATER) badSpots++;
				}
			}
			if (badSpots == 0) totalP = totalP + 1 + hitCount;
		}
	}
	return totalP;
}

// highestP
void BJZPlayer::highestP() {
	int highestP = 0;
	for (int row=0; row < boardSize; row++) {
		cout << endl;
		for (int col=0; col < boardSize; col++){
			if (Pboard[row][col] > highestP) {
				returnRow = row;
				returnCol = col;
				highestP = Pboard[row][col];
			}
		}
	}
}

// updateEnemyShipPlacementHistory
void BJZPlayer::updateEnemyShipPlacementHistory(Message msg) {
	int hit[2] = {msg.getRow(), msg.getCol()};
	cout << endl << "                                                Hit row " << hit[0] << " hit col " << hit[1] << endl;
	enemyShipPlacementHistory.push(hit);
	int* a = enemyShipPlacementHistory.front();
	cout << endl << "                                                enemyShipPlacementHistory recorded row " << a[0] << " and col " << a[1] << endl;
}


