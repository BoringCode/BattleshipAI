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
		// set place ships to 0s
		this->placedShipsBoard[row][col] = 0;
	}
    }
}

void BJZPlayer::initializePboard() {
    for (int i=0; i < boardSize; i++) {
	    for (int j=0; j < boardSize; j++) Pboard[i][j] = P_DEFAULT;
    }
	for (size_t i = 0; i < this->enemyShipPlacementHistory.size(); i++) {
		Message a = enemyShipPlacementHistory.front();
		enemyShipPlacementHistory.pop();
		enemyShipPlacementHistory.push(a);
		Pboard[a.getRow()][a.getCol()] = Pboard[a.getRow()][a.getCol()] + P_HISTORY;
	}
	//Print probablity board at beginning of round
	/*for (int row=0; row < boardSize; row++) {
		cout << endl;
		for (int col=0; col<boardSize; col++) {
			cout << Pboard[row][col] << " ";
		}
	}*/
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
	while(!shipLengths.empty()) {
		shipLengths.pop();
	}	
	
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
	shipLengths.push(length);

	int minShipScore = -1;
	int row = 0;
	int col = 0;
	Direction Orientation = Horizontal;
	
	int eachScore;

	bool validSpot = true;
	for(int i = 0; i < boardSize; i++) {
		for(int j = 0; j < boardSize; j++) {
			if (placedShipsBoard[i][j] == 1) { break; }
			// check horizontal
			if (j+length-1 < boardSize) {
				eachScore = 0;
				for(int cl = j; cl < j+length; cl++) {
					if (placedShipsBoard[i][cl] == 1) { validSpot = false; break; }
					eachScore += enemyShotBoard[i][cl];
				}
				if (!validSpot) {
					validSpot = true;
					break;
				}
				if (eachScore <= minShipScore || minShipScore == -1) {
					row = i;
					col = j;
					minShipScore = eachScore;
					Orientation = Horizontal;
				}
			}

			// check vertical	
			if (i+length-1 < boardSize) {
				eachScore = 0;
				for(int rw = i; rw < i+length; rw++) {
					if (placedShipsBoard[rw][j] == 1) { validSpot = false; break; }
					eachScore += enemyShotBoard[rw][j];
				}
				if (!validSpot) {
					validSpot = true;
					break;
				}
				if (eachScore <= minShipScore || minShipScore == -1) {
					row = i;
					col = j;
					minShipScore = eachScore;
					Orientation = Vertical;
				}
			}		
		}
	}

	//Update placed ships board
	if (Orientation == Horizontal) {
		for (int j = col; j < col + length; j++) {
			placedShipsBoard[row][j] = 1;
		}
	} else {
		for (int i = row; i < row + length; i++) {
			placedShipsBoard[i][col] = 1;
		}
	}
	//Create response
	Message response(PLACE_SHIP, row, col, shipName, Orientation, length);
	return response;
}
/**
 * @brief Updates the AI with the results of its shots and where the opponent is shooting.
 * @param msg Message specifying what happened + row/col as appropriate.
 */
void BJZPlayer::update(Message msg) {
    switch(msg.getMessageType()) {
	case HIT:
		updateEnemyShipPlacementHistory(msg);
		board[msg.getRow()][msg.getCol()] = msg.getMessageType();
		break;
	case KILL:
		updateEnemyShipPlacementHistory(msg);
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
				Pboard[row][col] = P_DEFAULT;
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
					if (board[row][col+forward] == HIT) hitCount = hitCount + P_SHIPFOUND;
					else if (board[row][col+forward] != WATER) badSpots++;
				}
			}
			for (int back=0; back <= end; back++) {
				if ((col-back) < 0) badSpots++;
				else {
					if (board[row][col-back] == HIT) hitCount = hitCount + P_SHIPFOUND;
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
					if (board[row-up][col] == HIT) hitCount = hitCount + P_SHIPFOUND;
					else if (board[row-up][col] != WATER) badSpots++;
				}
			}
			for (int down=0; down <= end; down++) {
				if ((row+down) > boardSize-1) badSpots++;
				else {
					if (board[row+down][col] == HIT) hitCount = hitCount + P_SHIPFOUND;
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
	//To prevent ship history from overpowering our propability algorithim, dump history after a certain point
	if (this->enemyShipPlacementHistory.size() > 30*shipLengths.size()) {
		this->enemyShipPlacementHistory.pop();
	}
	if (board[msg.getRow()][msg.getCol()] == WATER) {
		this->enemyShipPlacementHistory.push(msg);
	}
}


