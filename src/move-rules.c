#include "../include/piece-traits.h"
#include "../include/move-rules.h"

// each cond func returns whether a move is or not valid

bool isPawnAttackMove(const ChessMove* chessMove)
{
    return equalVecs(&chessMove->move,&(IntVec2D){1,-1}) ||
           equalVecs(&chessMove->move,&(IntVec2D){-1,-1});
}

// and promotion
// AND en passent? (i may not know how to spell)
bool pawnCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);
    const Piece pieceAtNextPosition = *getCurrPieceAtVec(&nextPosition);

    if (isPawnAttackMove(chessMove))
        if (!isBlankSpace(&pieceAtNextPosition))
            return true;

    if (equalVecs(&chessMove->move,&(IntVec2D){0,-1}))
        if (isBlankSpace(&pieceAtNextPosition))
            return true;

    // the middle position is between the "long" initial pawn move (at X)
                        /*   |   */
                        /*   X   */
                        /*   p   */
    const IntVec2D middlePosition = addVecs(&chessMove->position,&(IntVec2D){0,-1});
    const Piece pieceAtMiddlePosition = *getPieceAtVec(game->board,&middlePosition);

    if (equalVecs(&chessMove->move,&(IntVec2D){0,-2}))
        if (isBlankSpace(&pieceAtNextPosition) &&
            isBlankSpace(&pieceAtMiddlePosition) &&
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

#include <stdio.h>

bool isPathClear(ChessGame* game, const ChessMove* chessMove)
{
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);

    const Piece pieceAtPosition = *getPieceAtVec(game->board,&chessMove->position);
    const PieceTraits* traits = getTraits(&pieceAtPosition);

    IntVec2D direcVec = {0,0};

    for (int i = 0; i < traits->numMoves; i++)
        if (isVecDivByVec(&chessMove->move,&traits->moves[i]) &&
           (traits->moves[i].x >= 0 || traits->moves[i].y >= 0))
        {
            direcVec = traits->moves[i];
            break;
        }

    IntVec2D loopVec = direcVec;
    printf("%d %d", loopVec.x, loopVec.y);

    for (int i = 0; i < 8; i++)
    {
        const IntVec2D positionAtLoopVec = addVecs(&chessMove->position,&loopVec);
        const Piece pieceAtLoopVec = *getPieceAtVec(game->board,&positionAtLoopVec);

        if (isBlankSpace(&pieceAtLoopVec)) loopVec = addVecs(&direcVec,&loopVec);
        else return false;
        
        if (equalVecs(&positionAtLoopVec,&nextPosition))
            break;
    }

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
