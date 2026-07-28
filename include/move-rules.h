#ifndef MOVE_RULES_H
#define MOVE_RULES_H

#include "../include/int-vec-2d.h"
#include "../include/chess-game.h"

typedef struct 
{
    const IntVec2D position;
    const IntVec2D move;
} ChessMove;

typedef struct
{
    const Piece moving;
    const Piece capturing;
} PieceMove;

bool isValidMove(ChessGame* game, const ChessMove* chessMove);

#endif
