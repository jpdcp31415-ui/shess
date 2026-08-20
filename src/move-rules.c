#include "../include/piece-traits.h"
#include "../include/move-rules.h"
#include "../include/assert-toggle.h"

const char* getMoveErrReason(const MoveErr mvErr)
{
    switch (mvErr)
    {
    case NO_MOVE_ERR:          return "There were no errors!\n";

    case OOB_POSITION:         return "Position is Out of Bounds!\n";
    case OOB_MOVE:             return "Position + Move is Out of Bounds!\n";

    case MOVER_IS_BLANK:       return "Can't move blank piece!\n";
    case OPPOSITE_PLAYER_MOVE: return "Can't move piece of opposite player!\n";
    case DOESNT_HAVE_MOVE:     return "This piece does not have that move!\n";

    case NO_PIECE_TO_ATTACK:   return "There are no pieces to attack (diagonally) with pawn!\n";
    case NOT_INIT_POSITION:    return "Pawn cannot move 2 spaces unless has not moved before!\n";
    case CANT_ATTACK_FRONT:    return "There is a piece in front of this pawn that you cannot attack!\n";
    case PATH_NOT_CLEAR:       return "There are pieces in the way of your move!\n";

    case CASTLING_CHECK:       return "Can't castle because of check in the middle or where king is!\n";
    case CANT_EN_PASSENT:      return "Can't en passent!\n";
    case IS_STILL_CHECK:       return "This move won't take you out of check!\n";
    case MOVE_CAUSES_CHECK:    return "This move leads you to be in check!\n";

    case SAME_PLAYER_ATTACK:   return "You can't attack your own pieces!\n";
    }

    EXIT_MSG(!"There are no more move errors!");
}

PieceMove getAsPieceMove(const ChessGame* game, const BoardMove* boardMove)
{
    return (PieceMove)
    {
        .mover = getPieceAtVec(game->board,&boardMove->position),
        .captured = getPieceAtVec(game->board,&boardMove->position)
    };
}

void movePieceUncond(ChessGame* game, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = addVecs(&boardMove->position, &boardMove->move);
    *getPiecePtrAtVec(game->board,&nextPosition) = *getPiecePtrAtVec(game->board,&boardMove->position);
    *getPiecePtrAtVec(game->board,&boardMove->position) = (Piece){NULL_COLOUR, NULL_TYPE};

    if ((boardMove->position.x >= 0 && boardMove->position.x <= 7) &&
        (boardMove->position.y  < 2 && boardMove->position.y > 5)
         && !isBlankSpace(getPiecePtrAtVec(game->board,&boardMove->position)))
        *initPositionPtrAtVec(game,&boardMove->position) = false;
}

const ChessGame* movedKChessGamePtr(const ChessGame* game, const BoardMove* boardMove)
{
    static ChessGame gameCopy = {.player = NULL_PLAYER};
    setChessGame(game,&gameCopy);
    movePieceUncond(&gameCopy,boardMove);
    return &gameCopy;
}

MoveErr getOOBMoveErr(const BoardMove* boardMove)
{
    if (!isVecInBoardBounds(&boardMove->position))
        return OOB_POSITION;

    const IntVec2D nextPosition = addVecs(&boardMove->position,&boardMove->move);

    if (!isVecInBoardBounds(&nextPosition))
        return OOB_MOVE;

    return NO_MOVE_ERR;
}

// each cond func returns whether a move is or not valid

bool isPawnAttackMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){1,-1}) ||
           equalVecs(move,&(IntVec2D){-1,-1});
}

// and promotion
// AND en passent? (i may not know how to spell)
MoveErr pawnCondFunc(const ChessGame* game, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = addVecs(&boardMove->position,&boardMove->move);
    const Piece pieceAtNextPosition = getPieceAtVec(game->board,&nextPosition);

    if (isPawnAttackMove(&boardMove->move))
        if (isBlankSpace(&pieceAtNextPosition))
            return NO_PIECE_TO_ATTACK;

    if (equalVecs(&boardMove->move,&(IntVec2D){0,-1}))
        if (!isBlankSpace(&pieceAtNextPosition))
            return CANT_ATTACK_FRONT;

    // the middle position is between the "long" initial pawn move (at X)
                            /*   |   */
                            /*   X   */
                            /*   p   */
    const IntVec2D middlePosition = addVecs(&boardMove->position,&(IntVec2D){0,-1});
    const Piece pieceAtMiddlePosition = getPieceAtVec(game->board,&middlePosition);

    if (equalVecs(&boardMove->move,&(IntVec2D){0,-2}))
    {
        if (!initPositionAtVec(game,&boardMove->position)) return NOT_INIT_POSITION;
        if (!isBlankSpace(&pieceAtNextPosition))           return CANT_ATTACK_FRONT;
        if (!isBlankSpace(&pieceAtMiddlePosition))         return PATH_NOT_CLEAR;
    }

    return NO_MOVE_ERR;
}

// this is always true because of
// not having any piece-specific/special moves
MoveErr knightCondFunc(const ChessGame* game, const BoardMove* boardMove)
{
    (void)game,(void)boardMove;
    return NO_MOVE_ERR;
}

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece)
{
    const PieceTraits* traits = getTraits(piece);

    for (int i = 0; i < traits->numMoves; i++)
        if (isVecDivByVec(move,&traits->moves[i]))
            return traits->moves[i];

    EXIT_MSG(!"Direction vector not found!");
}

MoveErr isPathClear(const ChessGame* game, const BoardMove* boardMove)
{
    const Piece pieceAtPosition = getPieceAtVec(game->board,&boardMove->position);

    IntVec2D direcVec = getDirecVec(&boardMove->move,&pieceAtPosition);

    const IntVec2D nextPosition = addVecs(&boardMove->position,&boardMove->move);

    for (int i = 1; i < 8; i++)
    {
        const IntVec2D loopVec = multNumByVec(i,&direcVec);
        const IntVec2D positionAtLoopVec = addVecs(&boardMove->position,&loopVec);
        const Piece pieceAtLoopVec = getPieceAtVec(game->board,&positionAtLoopVec);

        if (equalVecs(&positionAtLoopVec,&nextPosition)) return NO_MOVE_ERR;

        if (!isBlankSpace(&pieceAtLoopVec)) return PATH_NOT_CLEAR;
    }

    return NO_MOVE_ERR;
}

bool isKingCastleMove(const IntVec2D* move)
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

    EXIT_MSG(!"King was not found!");
}

ChessMove getPossibleAttack(const ChessGame* game, const IntVec2D* position)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    const ChessGame* gameFlippedPtr = flippedKChessGamePtr(game);

    const IntVec2D flippedPosition = (IntVec2D){position->x,7-position->y};

    const Piece pieceAtFlippedPos = getPieceAtVec(gameFlippedPtr->board,&flippedPosition);

    ASSERT(game->player == pieceAtFlippedPos.colour, "Cannot move piece from opposite player!");

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            const Piece loopPiece = getPieceAt(gameFlippedPtr->board,x,y);

            if (isBlankSpace(&loopPiece) || game->player == loopPiece.colour) continue;

            const BoardMove possibleBoardMove =
            {
                .position = (IntVec2D){x,y},
                .move = subVecs(&flippedPosition,&(IntVec2D){x,y})
            };
            
            const ChessMove possibleChessMove =
            {
                .boardMove = possibleBoardMove,
                .pieceMove = getAsPieceMove(game,&possibleBoardMove)
            };

            if (hasMove(&possibleBoardMove.move,&loopPiece) &&
                getMoveErr(gameFlippedPtr,&possibleChessMove) != NO_MOVE_ERR)
                return possibleChessMove;
        }

    // sentinel value for cases when there is
    // no possibility that a piece moves to the position (flipped)
    return (ChessMove)
    {
        .boardMove = 
        {
            .position = flippedPosition,
            .move = {0,0},
        },
        .pieceMove = {
            .mover = {NULL_COLOUR,NULL_TYPE},
            .captured = pieceAtFlippedPos
        }
    };
}

// the possibility of a piece moving to a specific position
bool pieceMayMoveTo(const ChessGame* game, const IntVec2D* position)
{
    const Piece moverPiece = getPossibleAttack(game,position).pieceMove.mover;
    return !isBlankSpace(&moverPiece);
}

bool isCurrInCheck(const ChessGame* game)
{
    const IntVec2D kingPosition = whereKingIs(game->board,game->player);
    return pieceMayMoveTo(game,&kingPosition);
}

bool isPieceStuckAtVec(const ChessGame* game, const IntVec2D* position)
{
    const ChessGame* flippedGamePtr = flippedKChessGamePtr(game);

    const Piece _pieceAtPosition = getPieceAtVec(game->board,position);

    const ChessGame* usedGame = game->player == _pieceAtPosition.colour ? game : flippedGamePtr;

    const Piece pieceAtPosition = getPieceAtVec(usedGame->board,position);

    ASSERT(isBlankSpace(&pieceAtPosition), "Blank space cannot get stuck!");

    const PieceTraits* traits = getTraits(&pieceAtPosition);

    if (!traits->multSteps)
        for (int i = 0; i < traits->numMoves; i++)
        {
            const BoardMove possibleBoardMove =
            {
                .position = *position,
                .move = traits->moves[i]
            };
            
            const ChessMove possibleChessMove =
            {
                .boardMove = possibleBoardMove,
                .pieceMove = getAsPieceMove(usedGame,&possibleBoardMove)
            };

            if (getMoveErr(usedGame,&possibleChessMove) == NO_MOVE_ERR)
                return false;
        }
    else
        for (int i = 0; i < traits->numMoves; i++)
            for (int j = 0; j < 8; j++)
            {
                const IntVec2D multMove = multNumByVec(j,&traits->moves[i]);

                const BoardMove possibleBoardMove =
                {
                    .position = *position,
                    .move = multMove
                };

                const ChessMove possibleChessMove =
                {
                    .boardMove = possibleBoardMove,
                    .pieceMove = getAsPieceMove(usedGame,&possibleBoardMove)
                };

                if (getMoveErr(usedGame,&possibleChessMove) == NO_MOVE_ERR)
                    return false;
            }

    return true;
}

bool isWinFor(const ChessGame* game, const Colour c)
{
    const IntVec2D kingPosition = whereKingIs(game->board,game->player);

    if (!isCurrInCheck(game)) return false;

    if (!isPieceStuckAtVec(game,&kingPosition)) return false;

    /* if () */

    (void)c;
    return false;

}

MoveErr kingCondFunc(const ChessGame* game, const BoardMove* boardMove)
{
    const bool isInCheck = isCurrInCheck(movedKChessGamePtr(game,boardMove));
    if (isInCheck) return MOVE_CAUSES_CHECK;
    return NO_MOVE_ERR;
}

// This is a function that returns a pointer
// to a function to determine if it is valid
MoveErr(*getCondFunc(const Piece* p))(const ChessGame*,const BoardMove*)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank space does not have moves!");

    return (p->type == PAWN)   ? pawnCondFunc   :
           (p->type == KNIGHT) ? knightCondFunc :
           (p->type == KING)   ? kingCondFunc   :
           isPathClear;
}

MoveErr getPieceSpecificMoveErr(const ChessGame* game, const BoardMove* boardMove)
{
    return getCondFunc(getKPiecePtrAtVec(game->board,&boardMove->position))(game,boardMove);
}

MoveErr getMoveErr(const ChessGame* game, const ChessMove* chessMove)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot move when no player is playing!");

    if (getOOBMoveErr(&chessMove->boardMove) != NO_MOVE_ERR)
        return getOOBMoveErr(&chessMove->boardMove);

    if (isBlankSpace(&chessMove->pieceMove.mover))
        return MOVER_IS_BLANK;
    if (game->player == oppositeColour(chessMove->pieceMove.mover.colour))
        return OPPOSITE_PLAYER_MOVE;
    if (!hasMove(&chessMove->boardMove.move, &chessMove->pieceMove.mover))
        return DOESNT_HAVE_MOVE;

    if (game->player == chessMove->pieceMove.captured.colour)
        return SAME_PLAYER_ATTACK;

    return getCondFunc(getKPiecePtrAtVec(game->board,&chessMove->boardMove.position))(game,&chessMove->boardMove);
}
