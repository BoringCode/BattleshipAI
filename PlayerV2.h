/**
 * @author Stefan Brandle
 * @author Jonathan Geisler
 * @date September, 2004
 *
 */

#ifndef PLAYERV2_H
#define PLAYERV2_H		// Double inclusion protection

#include "Message.h"

class PlayerV2 {
    public:
        PlayerV2( int boardSize );

	/**
	 * @brief Tells the player to prepare for a new round in a game against the same opponent. 
	 *
	 * The player should reinitialize any round-specific data structures. This is not the same
	 * as the constructor, which must initialize all data structures that will last the 
	 * duration of the entire game. I.e., any inter-round learning data structures should be
	 * initialized in the constructor and not here, while intra-round data structures
	 * need to be initialized here.
	 */
	virtual void newRound() = 0;

	/**
	 * @brief Gets the player's move choice. This is then returned to the caller.
	 * @return Message move
	 * This is a pure virtual function. The Player class declares, but does
	 * not define it. That allows a class to force all derived classes
	 * to implement the functionality.
	 */
	virtual Message getMove() = 0;

	/**
	 * @brief Informs the player of the results of move's by player or opponent.
	 * The player updates its internal representation of the
	 * opponent's board to reflect the result.
	 * @param msg The message will have the shot coordinates
	 * (row, col) and the shot result available via the messageType.
	 */
	virtual void update( Message msg ) = 0;

	/**
	 * @brief Used to inform the game controller of where the player wishes to place a ship.
	 * The player updates its internal representation of the
	 * opponent's board to reflect the result.
	 * @return Message Indicates the row, col, and direction information for the ship to be placed.
	 */
	virtual Message placeShip(int length) = 0;

	// Nothing to do, so declare the destructor and define the empty body here.
        virtual ~PlayerV2() {};

    protected:

	// Standard data that all players have access to.
        int boardSize;
};

#endif
