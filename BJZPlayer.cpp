/**
 * @author Bradley Rosenfeld, Jacob Jin, Zach Alford
 * @date April, 2015
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
 * Also sets a board for where we set our own ships
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

/*
 * Intializes our propability board for the round
 */
void BJZPlayer::initializePboard() {
	//Set each cell to the default value
	for (int i=0; i < boardSize; i++) {
		for (int j=0; j < boardSize; j++) {
			Pboard[i][j] = P_DEFAULT;
			HPBoard[i][j] = P_DEFAULT;
		}
	}
	//Gets enemy ship history, and increments each cell accordingly
	for (size_t i = 0; i < this->enemyShipPlacementHistory.size(); i++) {
		Message a = enemyShipPlacementHistory.front();
		enemyShipPlacementHistory.pop();
		enemyShipPlacementHistory.push(a);
		HPBoard[a.getRow()][a.getCol()] = HPBoard[a.getRow()][a.getCol()] + P_HISTORY;
	}
	//Print probablity board at beginning of round, for testing purposes only
	/*for (int row=0; row < boardSize; row++) {
		cout << endl;
		for (int col=0; col<boardSize; col++) {
			cout << Pboard[row][col] << " ";
		}
	  }
	*/
}


/**
 * Gets the computer's shot choice. This is then returned to the caller.
 * @return Message The most important parts of the returned message are 
 * the row and column values. 
 * Position 0 of the int array should hold the row, position 1 the column.
 */
Message BJZPlayer::getMove() {
	//Recalculates the propability for each cell
	boardP(board);
	//Gets the cell with the highest propability
	highestP();
	//Returns the shot
	Message result( SHOT, returnRow, returnCol, "Bang", None, 1 );
	return result;
}

/**
 * @brief Tells the AI that a new round is beginning.
 * The AI show reinitialize any intra-round data structures.
 */
void BJZPlayer::newRound() {
	this->lastRow = 0;
	this->lastCol = -1;
	this->numShipsPlaced = 0;

	this->killCount = 0;
	this->prevKillCount = 0;
	this->reportingKill = false;

	//Resets the ship lengths queue
	while(!shipLengths.empty()) {
		shipLengths.pop();
	}	
	//Initializes our game boards
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
	//Set return variables
	int minShipScore = -1;
	int row = 0;
	int col = 0;
	Direction Orientation = Horizontal;
	//Used to calculate the socre of each possible ship location
	int eachScore;
	bool validSpot = true;
	//Loop through each possible cell
	for(int i = 0; i < boardSize; i++) {
		for(int j = 0; j < boardSize; j++) {
			//If a ship already exists at this location, don't continue
			if (placedShipsBoard[i][j] == 1) { continue; }
			// check horizontal
			//Make sure that ship could fit from the current location to the edge of the board
			if (j+length-1 < boardSize) {
				//Calculate the socre
				eachScore = 0;
				for(int cl = j; cl < j+length; cl++) {
					//Ship already exists at this location, stop searching
					if (placedShipsBoard[i][cl] == 1) { validSpot = false; break; }
					//Score comes from how many times our enemy has shot at this location
					eachScore += enemyShotBoard[i][cl];
				}
				//Make sure I can place a ship here and that the score is lower than the current minimum
				if (validSpot && (eachScore <= minShipScore || minShipScore == -1)) {
					row = i;
					col = j;
					minShipScore = eachScore;
					Orientation = Horizontal;
				}
				//Reset valid spot
				validSpot = true;
			}

			// check vertical	
			//Make sure that I can fit vertically at the current row
			if (i+length-1 < boardSize) {
				eachScore = 0;
				for(int rw = i; rw < i+length; rw++) {
					if (placedShipsBoard[rw][j] == 1) { validSpot = false; break; }
					eachScore += enemyShotBoard[rw][j];
				}
				//Ensure validity and minumum score
				if (validSpot && (eachScore <= minShipScore || minShipScore == -1)) {
					row = i;
					col = j;
					minShipScore = eachScore;
					Orientation = Vertical;
				}
				//Reset valid spot
				validSpot = true;
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
	if (msg.getMessageType() != KILL && reportingKill) {
		this->reportKill();
	}
	switch(msg.getMessageType()) {
		case HIT:
			updateEnemyShipPlacementHistory(msg);
			board[msg.getRow()][msg.getCol()] = msg.getMessageType();
			break;
		case KILL:
			reportingKill = true;
			killCount++;
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

/* Report kills and remove them from the ship lengths array */
void BJZPlayer::reportKill() {
	int length = 0;
	for (size_t i = 0; i < this->shipLengths.size(); i++) {
		length = this->shipLengths.front();
		this->shipLengths.pop();
		if (length == killCount) break;
		this->shipLengths.push(length);
	}
	this->reportingKill = false;
	this->prevKillCount = this->prevKillCount + this->killCount;
	this->killCount = -(this->prevKillCount);
}

/* Get minimum ship length */
int BJZPlayer::minShipLength() {
	int minLength = -1;
	int length = 0;
	for (size_t i = 0; i < this->shipLengths.size(); i++) {
		length = this->shipLengths.front();
		this->shipLengths.pop();
		if (minLength < 0 || length < minLength) {
			minLength = length;
		}
		this->shipLengths.push(length);
	}
	return minLength;
}

/**
 * Sets propability board for each move
 *
 */
void BJZPlayer::boardP(char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]) {
	for (int row=0; row < boardSize; row++) {
		for (int col=0; col < boardSize; col++){
			returnRow=row;
			returnCol=col;
			//Only calculate the cell's propability if it's an empty spot
			if (board[row][col] == WATER) {
				Pboard[row][col] = cellP(row,col,boardSize, board);
			} else {
				Pboard[row][col] = P_DEFAULT;
			}
		}
	}
}

/**
 * Calculates cell propability
 */
int BJZPlayer::cellP(int row,int col, int boardSize, char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE]){
	int totalP=HPBoard[row][col];
	int badSpots;
	int hitCount;
	//Loop through possible ship sizes, this should be changed to the min and max lengths that were passed to us
	for (int shipSize=minShipLength(); shipSize<= 4; shipSize++) {
		for (int end=0; end < shipSize; end++) {
			badSpots = 0;
			hitCount = 0;
			for (int forward=0; forward < (shipSize - end); forward++) {
				if ((forward+col) > (boardSize-1)) badSpots++;
				else {
					if (board[row][col+forward] == HIT) hitCount = hitCount + (P_SHIPFOUND / forward);
					else if (board[row][col+forward] != WATER) badSpots++;
				}
			}
			for (int back=0; back <= end; back++) {
				if ((col-back) < 0) badSpots++;
				else {
					if (board[row][col-back] == HIT) hitCount = hitCount + (P_SHIPFOUND / back);
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
					if (board[row-up][col] == HIT) hitCount = hitCount + (P_SHIPFOUND / up);
					else if (board[row-up][col] != WATER) badSpots++;
				}
			}
			for (int down=0; down <= end; down++) {
				if ((row+down) > boardSize-1) badSpots++;
				else {
					if (board[row+down][col] == HIT) hitCount = hitCount + (P_SHIPFOUND / down);
					else if (board[row+down][col] != WATER) badSpots++;
				}
			}
			if (badSpots == 0) totalP = totalP + 1 + hitCount;
		}
	}
	return totalP;
}

/*
 * Calculates the highest cell propability
 */
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

/*
 * Updates the enemy ship placement history on each shot
 */
void BJZPlayer::updateEnemyShipPlacementHistory(Message msg) {
	//To prevent ship history from overpowering our propability algorithim, dump history after a certain point
	if (this->enemyShipPlacementHistory.size() > 30*shipLengths.size()) {
		this->enemyShipPlacementHistory.pop();
	}
	//On kill, we will be messaged the previous spots again. Since we only want unique spots in the queue we must make sure we haven't proccessed it yet
	if (board[msg.getRow()][msg.getCol()] == WATER) {
		this->enemyShipPlacementHistory.push(msg);
	}
}

