#ifndef CHESS_MOVE_ARR_H
#define CHESS_MOVE_ARR_H

#include "../include/chess-game.h"
#include "../include/move-rules.h"

typedef struct
{
    ChessMove* data;
    int length;
    int capacity;
    Colour player;
} ChessMoveArr;

ChessMoveArr initMoveArr(void);

void freeMoveArr(ChessMoveArr* moves);

ChessMoveArr initLegalMovesFrom(const ChessGame* game, const IntVec2D* positionFrom, const bool ignoreCheck);

ChessMoveArr initLegalMovesTo(const ChessGame* game, const IntVec2D* positionTo, const bool ignoreCheck);

ChessMove initAttackToKing(const ChessGame* game);

bool currPieceMayMoveTo(const ChessGame* game, const IntVec2D* position, const bool ignoreCheck);

bool oppPieceMayMoveTo(const ChessGame* game, const IntVec2D* position, const bool ignoreCheck);

#endif
