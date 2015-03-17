/**
 * @author Stefan Brandle and Jonathan Geisler
 * @date August, 2004
 * Main driver for BattleShipsV1 implementations.
 * Please type in your name[s] below:
 *
 *
 */

#ifndef AITESTER_H
#define AITESTER_H

// BattleShips project specific includes.
#include "defines.h"
#include "Message.h"
#include "BoardV2.h"
#include "BoardV3.h"
#include "PlayerV2.h"

using namespace std;

class AITester {
  public:
    AITester(PlayerV2* player, BoardV2* playerBoard, string playerName, int boardSize, bool silent);
    void play(int delay, int& totalMoves);

    static const int baseRow = 4;
    static const int baseCol = 20;

  private:
    bool placeShips(PlayerV2* player, BoardV2* board, BoardV3* placingBoard );
    void showBoard(BoardV2* board, bool ownerView, string playerName);
    void showBoard(BoardV3* board, bool ownerView, string playerName);
    void clearScreen();
    void updateAI(PlayerV2* player, BoardV2* board);
    void snooze(int seconds);
    bool processShot(string playerName, PlayerV2* player, BoardV2* board, int row, int col);

    // Data
    PlayerV2* player;
    BoardV2* playerBoard;
    BoardV3* testingBoard;
    string playerName;
    int boardSize;
    int placedShipsBoardRow;
    int placedShipsBoardCol;
    bool silent;
    bool playerWon;
};

#endif
