#include "../include/piece-traits.h"
#include "../include/move-rules.h"

void movePieceUncond(ChessGame* game, const ChessMove* chMove)
{
    const IntVec2D nextPosition = addVecs(&chMove->position, &chMove->move);
    *getPiecePtrAtVec(game->board,&nextPosition) = *getPiecePtrAtVec(game->board,&chMove->position);
    *getPiecePtrAtVec(game->board,&chMove->position) = (Piece){NULL_COLOUR, NULL_TYPE};

    if ((chMove->position.x >= 0 && chMove->position.x <= 7) &&
        (chMove->position.y == 0 || chMove->position.y == 1  ||
         chMove->position.y == 6 || chMove->position.y == 7)
         && !isBlankSpace(getPiecePtrAtVec(game->board,&chMove->position)))
        *initPositionPtrAtVec(game,&chMove->position) = false;
}

// each cond func returns whether a move is or not valid

bool isPawnAttackMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){1,-1}) ||
           equalVecs(move,&(IntVec2D){-1,-1});
}

// and promotion
// AND en passent? (i may not know how to spell)
bool pawnCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    const IntVec2D nextPosition = addVecs(&chessMove->position,&chessMove->move);
    const Piece pieceAtNextPosition = getPieceAtVec(game->board,&nextPosition);

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
bool knightCondFunc(const ChessGame* game, const ChessMove* chessMove)
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
}

bool isPathClear(const ChessGame* game, const ChessMove* chessMove)
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

bool isKingRoqueMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){-2,0}) ||
           equalVecs(move,&(IntVec2D){2,0});
}

IntVec2D whereKingIs(const Board board, const Colour kingColour)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAt(board,x,y),&(Piece){kingColour,KING}))
                return (IntVec2D){x,y};

    assert(!"King was not found!");
}

bool isCurrInCheck(const ChessGame* game)
{
    const ChessGame gameFlipped = *flippedChessGame(game);

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            const Piece loopPiece = getPieceAt(gameFlipped.board,x,y);

            if (isBlankSpace(&loopPiece) || equalPlayer(game->player,loopPiece.colour)) continue;

            const IntVec2D kingPosition = whereKingIs(gameFlipped.board,
                                                      playerToColour(game->player));

            const ChessMove possibleAttack =
            {
                .position = kingPosition,
                .move = subVecs(&(IntVec2D){x,y},&kingPosition)
            };

            if (hasMove(&possibleAttack.move,&loopPiece)  &&
                isValidMove(&gameFlipped,&possibleAttack) &&
                loopPiece.colour == oppositeColour(playerToColour(game->player)))
                return true;
        }

    return false;
}

bool kingCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    ChessGame gameMoved = {.player = NO_PLAYER};

    setChessGame(game,&gameMoved);

    movePieceUncond(&gameMoved,chessMove);

    return !isCurrInCheck(&gameMoved);
}

// This is a function that returns a pointer
// to a function to determine if it is valid
bool(*getCondFunc(const Piece* p))(const ChessGame*,const ChessMove*)
{
    assertPiece(p);
    assert(!isBlankSpace(p) && "Blank space does not have moves!");

    return (p->type == PAWN)   ? pawnCondFunc   :
           (p->type == KNIGHT) ? knightCondFunc :
           (p->type == KING)   ? kingCondFunc   :
           isPathClear;
}

bool isValidMove(const ChessGame* game, const ChessMove* chessMove)
{
    return getCondFunc(getKPiecePtrAtVec(game->board,&chessMove->position))(game,chessMove);
}
