/**
 * @author Stefan Brandle
 * @author Jonathan Geisler
 * @date September, 2014
 *
 */

#ifndef PLAYERV2_CPP		// Double inclusion protection
#define PLAYERV2_CPP

#include <iostream>

#include "PlayerV2.h"

/**
 * @brief Constructor: initializes any game-long data structures.
 *
 * The player should reinitialize any inter-round data structures that will be used
 * for the entire game.
 * I.e., any intra-round learning data structures should be initialized in the here, 
 * while inter-round data structures need to be initialized in newRound().
 */
PlayerV2::PlayerV2( int boardSize ) {
    // Set boardsize
    this->boardSize = boardSize;
}

#endif
