#include "../include/piece-traits.h"
#include "../include/move-rules.h"
#include "../include/assert-toggle.h"
#include "../include/game-state.h"

IntVec2D getNextPos(const BoardMove* b)
{
    return addVecs(&b->position, &b->move);
}

PieceMove getAsPieceMove(const Board board, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = getNextPos(boardMove);

    return (PieceMove)
    {
        .mover = getPieceAtVec(board,&boardMove->position),
        .captured = getPieceAtVec(board,&nextPosition)
    };
}

ChessMove getAsChessMove(const Board board, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = getNextPos(boardMove);

    return (ChessMove)
    {
        .boardMove = *boardMove,
        .pieceMove =
        {
            .mover = getPieceAtVec(board,&boardMove->position),
            .captured = getPieceAtVec(board,&nextPosition)
        }        
    };
}

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
    case MOVE_CAUSES_CHECK:    return "This move leads you to be in check!\n";
    case IS_STILL_CHECK:       return "This move won't take you out of check!\n";

    case SAME_PLAYER_ATTACK:   return "You can't attack your own pieces!\n";
    }

    EXIT_MSG(!"There are no more move errors!");
}

void movePieceUncond(ChessGame* game, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = getNextPos(boardMove);
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

    const IntVec2D nextPosition = getNextPos(boardMove);

    if (!isVecInBoardBounds(&nextPosition))
        return OOB_MOVE;

    return NO_MOVE_ERR;
}

// each cond func returns whether a move is or not valid

IntVec2D getPawnPromotionPosition(const ChessGame* game)
{
    for (int x = 0; x < 8; x++)
        if (equalPiece(&(Piece){game->player,PAWN}, getKPiecePtrAt(game->board,x,0)))
            return (IntVec2D){x,0};

    return (IntVec2D){-1,-1};
}

bool isPawnPromotion(const ChessGame* game)
{
    const IntVec2D promPos = getPawnPromotionPosition(game);
    return !equalVecs(&promPos,&(IntVec2D){-1,-1});
}

bool isPawnAttackMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){1,-1}) ||
           equalVecs(move,&(IntVec2D){-1,-1});
}

// and promotion
// AND en passent? (i may not know how to spell)
MoveErr pawnCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    if (isPawnAttackMove(&chessMove->boardMove.move))
        if (isBlankSpace(&chessMove->pieceMove.captured))
            return NO_PIECE_TO_ATTACK;

    if (equalVecs(&chessMove->boardMove.move,&(IntVec2D){0,-1}))
        if (!isBlankSpace(&chessMove->pieceMove.captured))
            return CANT_ATTACK_FRONT;

    // the middle position is between the "long" initial pawn move (at X)
                            /*   |   */
                            /*   X   */
                            /*   p   */
    const IntVec2D middlePosition = addVecs(&chessMove->boardMove.position,&(IntVec2D){0,-1});
    const Piece pieceAtMiddlePosition = getPieceAtVec(game->board,&middlePosition);

    if (equalVecs(&chessMove->boardMove.move,&(IntVec2D){0,-2}))
    {
        if (!initPositionAtVec(game,&chessMove->boardMove.position)) return NOT_INIT_POSITION;
        if (!isBlankSpace(&chessMove->pieceMove.captured))           return CANT_ATTACK_FRONT;
        if (!isBlankSpace(&pieceAtMiddlePosition))                   return PATH_NOT_CLEAR;
    }

    return NO_MOVE_ERR;
}

// this always returns NO_MOVE_ERR because of
// not having any piece-specific/special moves
MoveErr knightCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    (void)game,(void)chessMove;
    return NO_MOVE_ERR;
}

bool isPathClear(const ChessGame* game, const BoardMove* boardMove)
{
    const Piece pieceAtPosition = getPieceAtVec(game->board,&boardMove->position);

    const IntVec2D direcVec = getDirecVec(&boardMove->move,&pieceAtPosition);

    const IntVec2D nextPosition = getNextPos(boardMove);

    for (int i = 1; i < 8; i++)
    {
        const IntVec2D loopVec = multNumByVec(i,&direcVec);
        const IntVec2D positionAtLoopVec = addVecs(&boardMove->position,&loopVec);
        const Piece pieceAtLoopVec = getPieceAtVec(game->board,&positionAtLoopVec);

        if (equalVecs(&positionAtLoopVec,&nextPosition)) return true;

        if (!isBlankSpace(&pieceAtLoopVec)) return false;
    }

    return true;
}

MoveErr multStepCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    return isPathClear(game, &chessMove->boardMove) ? NO_MOVE_ERR : PATH_NOT_CLEAR;
}

bool isKingCastleMove(const IntVec2D* move)
{
    return equalVecs(move,&(IntVec2D){-2,0}) ||
           equalVecs(move,&(IntVec2D){2,0});
}

MoveErr kingCondFunc(const ChessGame* game, const ChessMove* chessMove)
{
    const bool moveCausesCheck = doesMoveCauseCheck(game, &chessMove->boardMove);
    if (moveCausesCheck) return MOVE_CAUSES_CHECK;

    if (isKingCastleMove(&chessMove->boardMove.move)) return CASTLING_CHECK;
    return NO_MOVE_ERR;
}

// This is a function that returns a pointer
// to a function to determine if it is valid
MoveErr(*getCondFunc(const Piece* p))(const ChessGame*,const ChessMove*)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank space does not have moves!");

    return (p->type == PAWN)   ? pawnCondFunc   :
           (p->type == KNIGHT) ? knightCondFunc :
           (p->type == KING)   ? kingCondFunc   :
           multStepCondFunc;
}

MoveErr getPieceSpecificMoveErr(const ChessGame* game, const ChessMove* chessMove)
{
    return getCondFunc(
               getKPiecePtrAtVec(game->board,&chessMove->boardMove.position)
           )(game,chessMove);
}

MoveErr getMoveErrOpts(const ChessGame* game, const ChessMove* chessMove, const bool ignoreCheck)
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

    if (!ignoreCheck)
        if (doesMoveCauseCheck(game, &chessMove->boardMove) &&
            isCurrInCheck(game))
            return IS_STILL_CHECK;

    if (ignoreCheck && chessMove->pieceMove.mover.type == KING)
        return NO_MOVE_ERR;

    return getPieceSpecificMoveErr(game, chessMove);
}

bool isValidMoveOpts(const ChessGame *game, const ChessMove *chessMove, const bool ignoreCheck)
{
    return getMoveErrOpts(game, chessMove, ignoreCheck) == NO_MOVE_ERR;
}

MoveErr getMoveErr(const ChessGame* game, const ChessMove* chessMove)
{
    return getMoveErrOpts(game, chessMove, false);
}

bool isValidMove(const ChessGame* game, const ChessMove* chessMove)
{
    return isValidMoveOpts(game, chessMove, false);
}
