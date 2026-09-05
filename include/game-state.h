#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "chess-game.h"
#include "move-rules.h"

typedef enum
{
    DEF_GAME_STATE,
    CHECK_STATE,
    LOSS_STATE,
    DRAW_STATE,
    WIN_STATE,
} GameState;

GameState getChessGameState(const ChessGame* game);

IntVec2D whereKingIs(const Board board, const Colour kingColour);

bool isCurrInCheck(const ChessGame* game);

bool canBlockAttackMove(const ChessGame* game, const ChessMove* possibleAttack);

// or isLossCurrPlayer
// or isCheckmateForCurrPlayer
// or isCheckmate
bool isWinForOppPlayer(const ChessGame* game);

bool doesMoveCauseCheck(const ChessGame* game, const BoardMove* boardMove);

#endif
