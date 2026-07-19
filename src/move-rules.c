#include "../include/piece-traits.h"
#include "../include/move-rules.h"

// each cond func returns whether a move is or not valid

// add first move (2 spaces)
// and the attack move
// and promotion
bool pawnCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);

    if (equalVecs(&chessMove->move,&(IntVec2D){1,-1}) ||
        equalVecs(&chessMove->move,&(IntVec2D){-1,-1}))
        if (!isBlankSpace(getPieceAtVec(game->board,&nextPosition)))
            return true;

    if (equalVecs(&chessMove->move,&(IntVec2D){0,-1}))
        if (isBlankSpace(getPieceAtVec(game->board,&nextPosition)))
            return true;

    const IntVec2D middlePosition = addVecs(&chessMove->position,&(IntVec2D){0,-1});

    if (equalVecs(&chessMove->move,&(IntVec2D){0,-2}))
        if (isBlankSpace(getPieceAtVec(game->board,&nextPosition)) &&
            isBlankSpace(getPieceAtVec(game->board,&middlePosition)) &&
            initPositionAtVec(game,&chessMove->position))
            return true;

    return false;
}

// this is always true because of
// not having any piece-specific/special moves
bool knightCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

bool isPathClear(ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

bool kingCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return true;
}

// This is a function that returns a pointer
// to a function to determine if it is valid
bool(*getCondFunc(const Piece* p))(ChessGame*,const ChessMove*)
{
    bool(*condFuncArr[])(ChessGame*,const ChessMove*) = {
        pawnCondFunc,
        knightCondFunc,
        isPathClear,
        isPathClear,
        isPathClear,
        kingCondFunc,
    };

    return condFuncArr[p->type-1];
}

bool isValidMove(ChessGame* game, const ChessMove* chessMove)
{
    return getCondFunc(getPieceAtVec(game->board,&chessMove->position))(game,chessMove);
};
