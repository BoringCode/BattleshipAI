/**
 * @author Stefan Brandle and Jonathan Geisler
 * @date August, 2004
 * Main driver for BattleShipsV3 implementations.
 * Please type in your name[s] below:
 *
 *
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include <unistd.h>

// Next 2 to access and setup the random number generator.
#include <cstdlib>
#include <ctime>

// BattleShips project specific includes.
#include "BoardV3.h"
#include "AIContest.h"
#include "PlayerV2.h"
#include "conio.h"

// Include your player here
//	Professor's contestants
#include "DumbPlayerV2.h"
#include "CleanPlayerV2.h"
//#include "OrigGamblerPlayer.h"


PlayerV2* getPlayer( int playerId, int boardSize );

using namespace std;
using namespace conio;

int main() {
    //int numGames;
    int boardSize;	// BoardSize
    //bool silent = false;
    float secondsPerMove = 1;
    int totalGames = 0;
    int totalCountedMoves = 0;
    bool player1Won=false, player2Won=false;
    PlayerV2 *player1, *player2;
    AIContest *game;

    // Adjust based on the number of players!
    const int NumPlayers = 2;
    int wins[NumPlayers][NumPlayers];
    int winCount[NumPlayers];
    int statsShotsTaken[NumPlayers];
    int statsGamesCounted[NumPlayers];
    string playerNames[NumPlayers] = { 
	"Dumb Player",
	"Clean Player",
    };

    // Initialize various win statistics 
    for(int i=0; i<NumPlayers; i++) {
	statsShotsTaken[i] = 0;
	statsGamesCounted[i] = 0;
	winCount[i] = 0;
	for(int j=0; j<NumPlayers; j++) {
	    wins[i][j] = 0;
    	}
    }


    // Seed (setup) the random number generator.
    // This only needs to happen once per program run.
    srand(time(NULL));

    // Now to get the board size.
    cout << "Welcome to the AI Bot contest." << endl << endl;
    cout << "What size board would you like? [Anything other than numbers 3-10 exits.] ";
    cin >> boardSize;
    // If have invalid board size input (non-number, or 0-2, or > 10).
    if ( !cin || boardSize < 3 || boardSize > 10 ) {
	cout << "Exiting" << endl;
	return 1;
    }

    // Find out how many times to test the AI.
    cout << "How many times should I test the game AI? ";
    cin >> totalGames;

    cout << "The first game of each AI match is played at the specified speed," << endl
	 << "all subsequent games are done without visual display." << endl
	 << "How many seconds per move? (E.g., 1, 0.5, 1.3) : ";
    cin >> secondsPerMove;
    /*
    } else {
        milliSecondsPerMove = 0;
    }
    */

    // And now it's show time!
    int player1Ties=0, player2Ties=0;
    for( int player1Id=0; player1Id<NumPlayers; player1Id++ ) {
	for( int player2Id=player1Id+1; player2Id<NumPlayers; player2Id++ ) {

	    player1 = getPlayer(player1Id, boardSize);
	    player2 = getPlayer(player2Id, boardSize);
	    // Reset match scores
	    //player1Wins=0; player2Wins=0; 
	    player1Ties=0; player2Ties=0;
	    for( int count=0; count<totalGames; count++ ) {
		player1Won = false; player2Won = false;
		player1->newRound();
		player2->newRound();

		//if( count==0 ) {
		    game = new AIContest( player1, playerNames[player1Id], 
					  player2, playerNames[player2Id],
					  boardSize, false /* !silent */ );
		    game->play( secondsPerMove, totalCountedMoves, player1Won, player2Won );
		/*
		} 
		else {
		    game = new AIContest( player1, playerNames[player1Id], 
					  player2, playerNames[player2Id],
			      boardSize, true );
		    game->play( 0, totalCountedMoves, player1Won, player2Won );
		}
	        */
		if((player1Won && player2Won) || !(player1Won || player2Won)) {
		    player1Ties++;
		    player2Ties++;
		    statsShotsTaken[player1Id] += totalCountedMoves;
		    statsGamesCounted[player1Id]++;
		    statsShotsTaken[player2Id] += totalCountedMoves;
		    statsGamesCounted[player2Id]++;
		} else if( player1Won ){
		    wins[player1Id][player2Id]++;
		    statsShotsTaken[player1Id] += totalCountedMoves;
		    statsGamesCounted[player1Id]++;
		} else {
		    wins[player2Id][player1Id]++;
		    statsShotsTaken[player2Id] += totalCountedMoves;
		    statsGamesCounted[player2Id]++;
		}
		delete game;
	    }
	    delete player1;
	    delete player2;

	    cout << endl << "********************" << endl;
	    cout << playerNames[player1Id] << ": " << setTextStyle( NEGATIVE_IMAGE ) << "wins=" << wins[player1Id][player2Id] << resetAll()
		 << " losses=" << totalGames-wins[player1Id][player2Id]-player1Ties 
		 << " ties=" << player1Ties << " (cumulative avg. shots/game = "
		 << (statsGamesCounted[player1Id]==0 ? 0.0 : 
		    (float)statsShotsTaken[player1Id]/(float)statsGamesCounted[player1Id])
		 << ")" << endl;
	    cout << playerNames[player2Id] << ": " << setTextStyle( NEGATIVE_IMAGE ) << "wins=" << wins[player2Id][player1Id] << resetAll()
		 << " losses=" << totalGames-wins[player2Id][player1Id]-player2Ties 
		 << " ties=" << player2Ties << " (cumulative avg. shots/game = "
		 << (statsGamesCounted[player2Id]==0 ? 0.0 : 
		    (float)statsShotsTaken[player2Id]/(float)statsGamesCounted[player2Id])
		 << ")" << endl;
	    cout << "********************" << endl;
	    usleep(10000000);	// Pause 10 seconds to let viewers see stats
	}
    }
    cout << endl << endl;
    int mostWins = -1;
    int winnerCount = 0;
    for( int i=0; i<NumPlayers; i++ ) {
	for( int j=0; j<NumPlayers; j++ )
	    winCount[i]+= wins[i][j];
	if( winCount[i] > mostWins ) {
	    mostWins = winCount[i];
	    winnerCount = 1;
	}
	else if( winCount[i] == mostWins ) {
	    winnerCount++;
	}
    }

    for( int i=0; i<NumPlayers; i++ ) {
	if( winCount[i] == mostWins ) {
	    cout << setTextStyle( BOLD );
	}
	cout << playerNames[i] << ": wins = " << winCount[i];
	     //<< " (avg. shots/game = " << (statsGamesCounted[i]==0 ? 0.0 : 
	     // (float)statsShotsTaken[i]/(float)statsGamesCounted[i]) << ") ";

	if( winCount[i] == mostWins ) {
	    if( winnerCount == 1 ) {
		cout << "(The WINNER!)";
	    } else {	// tied win count
		cout << "(Tied for First!!)";
	    }
	}
	cout << resetAll() << endl;
    }

    return 0;
}

PlayerV2* getPlayer( int playerId, int boardSize ) {
    switch( playerId ) {
	// Professor provided
	default:
	case 0: return new DumbPlayerV2( boardSize );
	case 1: return new CleanPlayerV2( boardSize );
	//case 2: return new OrigGamblerPlayer( boardSize );
    }
}

