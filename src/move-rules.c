#include "../include/piece-traits.h"
#include "../include/move-rules.h"

// each cond func returns whether a move is or not valid

bool isPawnAttackMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){1,-1}) ||
           equalVecs(move,&(IntVec2D){-1,-1});
}

bool isKingRoqueMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){-2,0}) ||
           equalVecs(move,&(IntVec2D){2,0});
}

// and promotion
// AND en passent? (i may not know how to spell)
bool pawnCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);
    const Piece pieceAtNextPosition = getCurrPieceAtVec(&nextPosition);

    if (isPawnAttackMove(&chessMove->move))
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
    const Piece pieceAtMiddlePosition = getPieceAtVec(game->board,&middlePosition);

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

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece)
{
    const PieceTraits* traits = getTraits(piece);

    for (int i = 0; i < traits->numMoves; i++)
        if (isVecDivByVec(move,&traits->moves[i]))
            return traits->moves[i];

    assert(!"Direction vector not found!");
    return (IntVec2D){};
}

bool isPathClear(ChessGame* game, const ChessMove* chessMove)
{
    const Piece pieceAtPosition = getPieceAtVec(game->board,&chessMove->position);

    IntVec2D direcVec = getDirecVec(&chessMove->move,&pieceAtPosition);

    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);

    for (int i = 1; i < 8; i++)
    {
        const IntVec2D loopVec = multNumByVec(i,&direcVec);
        const IntVec2D positionAtLoopVec = addVecs(&chessMove->position,&loopVec);
        const Piece pieceAtLoopVec = getPieceAtVec(game->board,&positionAtLoopVec);

        if (equalVecs(&positionAtLoopVec,&nextPosition)) return true;

        if (!isBlankSpace(&pieceAtLoopVec)) return false;
    }

    return true;
}

bool pieceMayMoveTo(const ChessGame* game, const IntVec2D* nextPosition, const Piece* piece)
{
    assertPiece(piece);
    assert(!isBlankSpace(piece) && "Blank space does not have moves!");

    const PieceTraits* traits = getTraits(piece);

    for (int iMoves = 0; iMoves < traits->numMoves; iMoves++)
    {
        if (traits->multSteps)
            for (int iVec = 1; iVec < 8; iVec++)
            {
                const IntVec2D direcVec = traits->moves[iMoves];
                const IntVec2D loopVec = multNumByVec(iVec,&direcVec);
                const IntVec2D positionAtLoopVec = addVecs(nextPosition,&loopVec);

                if (!isVecInBoardBounds(&positionAtLoopVec)) return false;

                const Piece pieceAtLoopVec = getPieceAtVec(game->board,&positionAtLoopVec);

                if (equalPiece(&pieceAtLoopVec,piece)) return true;
                else if (!isBlankSpace(&pieceAtLoopVec)) return false;
            }

        const IntVec2D loopMove = invertY(&traits->moves[iMoves]);
        const IntVec2D positionAtLoopMove = addVecs(nextPosition,&loopMove);

        if (!isVecInBoardBounds(&positionAtLoopMove)) continue;

        const Piece pieceAtLoopMove = getPieceAtVec(game->board,&positionAtLoopMove);

        if (piece->type == KNIGHT)
            if (equalPiece(&pieceAtLoopMove,piece))
                return true;

        if (piece->type == PAWN)
            if (isPawnAttackMove(&loopMove))
                if (equalPiece(&pieceAtLoopMove,piece))
                    return true;

        if (piece->type == KING)
            if (!isKingRoqueMove(&loopMove))
                if (equalPiece(&pieceAtLoopMove,piece))
                    return true;
    }

    return false;

    assert(!"There are no more types to check for!");
}

IntVec2D whereKingIs(const ChessGame* game, const Colour c)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAtVec(game->board,&(IntVec2D){x,y}),
                           &(Piece){c,KING}))
                return (IntVec2D){x,y};
    
    assert(!"King was not found!");
}

bool isInCheck(const ChessGame* game, const IntVec2D* nextPosition, const Colour colour)
{
    for (Type pieceType = PAWN; pieceType <= KING; pieceType++)
        if (pieceMayMoveTo(game,nextPosition,&(Piece){oppositeColour(colour),pieceType}))
            return true;

    return false;
}

bool kingCondFunc(ChessGame* game, const ChessMove* chessMove)
{
    (void)chessMove;
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);
    return !isInCheck(game,&nextPosition,playerToColour(game->player));
}

// This is a function that returns a pointer
// to a function to determine if it is valid
bool(*getCondFunc(const Piece* p))(ChessGame*,const ChessMove*)
{
    assertPiece(p);
    assert(!isBlankSpace(p) && "Blank space does not have moves!");

    return (p->type == PAWN)   ? pawnCondFunc   :
           (p->type == KNIGHT) ? knightCondFunc :
           (p->type == KING)   ? kingCondFunc   :
           isPathClear;
}

bool isValidMove(ChessGame* game, const ChessMove* chessMove)
{
    return getCondFunc(getPiecePtrAtVec(game->board,&chessMove->position))(game,chessMove);
}
