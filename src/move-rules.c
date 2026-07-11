#include "../include/piece-traits.h"
#include "../include/move-rules.h"

// each cond func returns whether a move is or not valid

// add first move (2 spaces)
// and the attack move
// and promotion
bool pawnCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    if (equalVecs(&chessMove->move,&(IntVec2D){1,-1}) &&
        equalVecs(&chessMove->move,&(IntVec2D){1,1}))
        if (getPieceAtVec(game->board,chessMove->))
}

// this is always true because of
// not having any piece-specific/special moves
bool knightCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

bool isPathClear(const ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

bool kingCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

bool isValidMove(const ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}
