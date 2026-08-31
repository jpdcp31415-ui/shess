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

PieceMove getAsPieceMove(const Board board, const BoardMove* boardMove)
{
    const IntVec2D nextPosition = addVecs(&boardMove->position, &boardMove->move);

    return (PieceMove)
    {
        .mover = getPieceAtVec(board,&boardMove->position),
        .captured = getPieceAtVec(board,&nextPosition)
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

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece)
{
    const PieceTraits* traits = getTraits(piece);

    for (int i = 0; i < traits->numMoves; i++)
        if (isVecDivByVec(move,&traits->moves[i]))
            return traits->moves[i];

    EXIT_MSG(!"Direction vector not found!");
}

bool isPathClear(const ChessGame* game, const BoardMove* boardMove)
{
    const Piece pieceAtPosition = getPieceAtVec(game->board,&boardMove->position);

    const IntVec2D direcVec = getDirecVec(&boardMove->move,&pieceAtPosition);

    const IntVec2D nextPosition = addVecs(&boardMove->position,&boardMove->move);

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

IntVec2D whereKingIs(const Board board, const Colour kingColour)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAt(board,x,y),&(Piece){kingColour,KING}))
                return (IntVec2D){x,y};

    EXIT_MSG(!"King was not found!");
}

ChessMove getPossibleMoveTo(const ChessGame* game, const IntVec2D* position)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    const Piece pieceAtPosition = getPieceAtVec(game->board,position);

    ASSERT(isBlankSpace(&pieceAtPosition) || game->player != pieceAtPosition.colour, "Cannot attack piece of same colour!");

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            const Piece possibleMover = getPieceAt(game->board,x,y);

            if (isBlankSpace(&possibleMover) || game->player != possibleMover.colour) continue;

            const BoardMove possibleBoardMove =
            {
                .position = (IntVec2D){x,y},
                .move = subVecs(position,&(IntVec2D){x,y})
            };

            const ChessMove possibleChessMove =
            {
                .boardMove = possibleBoardMove,
                .pieceMove = getAsPieceMove(game->board,&possibleBoardMove)
            };

            if (isValidMove(game,&possibleChessMove)) return possibleChessMove;
        }

    // sentinel value for cases when there is
    // no possibility that a piece moves to the position (flipped)
    return (ChessMove)
    {
        .boardMove = 
        {
            .position = {-1,-1},
            .move = {0,0},
        },
        .pieceMove = {
            .mover = {NULL_COLOUR,NULL_TYPE},
            .captured = pieceAtPosition
        }
    };
}

bool currPlayerPieceMayMoveTo(const ChessGame* game, const IntVec2D* position)
{
    const Piece moverPiece = getPossibleMoveTo(game,position).pieceMove.mover;
    return !isBlankSpace(&moverPiece);
}

// the possibility of a piece moving to a specific position
bool oppPieceMayMoveTo(const ChessGame* game, const IntVec2D* position)
{
    // flip board for the opposite player's perspective
    const ChessGame* _gameFlippedPtr = flippedKChessGamePtr(game);

    ChessGame gameFlipped = {.player = NULL_PLAYER};
    setChessGame(_gameFlippedPtr,&gameFlipped);
    gameFlipped.player = oppositeColour(game->player);

    const IntVec2D flippedPosition = {position->x, 7-position->y};

    const Piece moverPiece = getPossibleMoveTo(&gameFlipped,&flippedPosition).pieceMove.mover;
    return !isBlankSpace(&moverPiece);
}

bool isCurrInCheck(const ChessGame* game)
{
    const IntVec2D kingPosition = whereKingIs(game->board,game->player);
    return oppPieceMayMoveTo(game,&kingPosition);
}

bool isPieceStuckAtVec(const ChessGame* game, const IntVec2D* position)
{
    const ChessGame* flippedGamePtr = flippedKChessGamePtr(game);

    const Piece _pieceAtPosition = getPieceAtVec(game->board,position);

    const ChessGame* usedGame = game->player == _pieceAtPosition.colour ? game : flippedGamePtr;

    const IntVec2D usedPosition = game->player == _pieceAtPosition.colour ?
                                                  *position :
                                                  (IntVec2D) {position->x, 7-position->y};

    const Piece pieceAtPosition = getPieceAtVec(usedGame->board,&usedPosition);

    ASSERT(!isBlankSpace(&pieceAtPosition), "Blank space cannot get stuck!");

    const PieceTraits* traits = getTraits(&pieceAtPosition);

    if (!isMultStep(&pieceAtPosition))
        for (int i = 0; i < traits->numMoves; i++)
        {
            const BoardMove possibleBoardMove =
            {
                .position = usedPosition,
                .move = traits->moves[i]
            };

            const IntVec2D nextPosition = addVecs(&usedPosition,&traits->moves[i]);
            if (!isVecInBoardBounds(&nextPosition)) continue;
            
            const ChessMove possibleChessMove =
            {
                .boardMove = possibleBoardMove,
                .pieceMove = getAsPieceMove(usedGame->board,&possibleBoardMove)
            };

            if (isValidMove(usedGame,&possibleChessMove))
                return false;
        }
    else
        for (int i = 0; i < traits->numMoves; i++)
            for (int j = 0; j < 8; j++)
            {
                const IntVec2D multMove = multNumByVec(j,&traits->moves[i]);

                const BoardMove possibleBoardMove =
                {
                    .position = usedPosition,
                    .move = multMove
                };

                const ChessMove possibleChessMove =
                {
                    .boardMove = possibleBoardMove,
                    .pieceMove = getAsPieceMove(usedGame->board,&possibleBoardMove)
                };

                if (isValidMove(usedGame,&possibleChessMove))
                    return false;
            }

    return true;
}

bool canBlockAttackMove(const ChessGame* game, const ChessMove* possibleAttack)
{
    const Piece possibleMover = possibleAttack->pieceMove.mover;
    ASSERT(!isBlankSpace(&possibleMover), "This move is not a valid move!");

    ASSERT(isMultStep(&possibleMover), "Piece mover is not a multi-step piece!");

    const IntVec2D direcVec = getDirecVec(&possibleAttack->boardMove.move,
                                          &possibleAttack->pieceMove.mover);

    for (int i = 1; i < 8; i++)
    {
        const IntVec2D loopVec = multNumByVec(i,&direcVec);
        const IntVec2D positionAtLoopVec = addVecs(&possibleAttack->boardMove.position,&loopVec);

        if (!isVecInBoardBounds(&positionAtLoopVec)) continue;

        if (currPlayerPieceMayMoveTo(game,&positionAtLoopVec)) return true;
    }

    return false;
}

// or isLossCurrPlayer
// or isCheckmateForCurrPlayer
// or isCheckmate
bool isWinForOppPlayer(const ChessGame* game)
{
    const IntVec2D kingPosition = whereKingIs(game->board,game->player);

    if (!isCurrInCheck(game)) return false;

    if (!isPieceStuckAtVec(game,&kingPosition)) return false;

    // flip board for the opposite player's perspective
    const ChessGame* _gameFlippedPtr = flippedKChessGamePtr(game);

    ChessGame gameFlipped = {.player = NULL_PLAYER};
    setChessGame(_gameFlippedPtr,&gameFlipped);
    gameFlipped.player = oppositeColour(game->player);

    const IntVec2D flippedPosition = {kingPosition.x, 7-kingPosition.y};
    const ChessMove possibleAttack = getPossibleMoveTo(&gameFlipped,&flippedPosition);

    if (isBlankSpace(&possibleAttack.pieceMove.mover)) return false;

    if (isMultStep(&possibleAttack.pieceMove.mover))
        if (canBlockAttackMove(game,&possibleAttack)) return false;

    const IntVec2D nextPosition = addVecs(&possibleAttack.boardMove.position,
                                          &possibleAttack.boardMove.move);
    if (currPlayerPieceMayMoveTo(game, &nextPosition)) return false;

    return true;
}

bool doesMoveCauseCheck(const ChessGame* game, const BoardMove* boardMove)
{
    return isCurrInCheck(movedKChessGamePtr(game,boardMove));
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
    return getCondFunc(getKPiecePtrAtVec(game->board,&chessMove->boardMove.position))(game,chessMove);
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

    return getCondFunc(getKPiecePtrAtVec(game->board,&chessMove->boardMove.position))(game,chessMove);
}

bool isValidMove(const ChessGame* game, const ChessMove* chessMove)
{
    return getMoveErr(game,chessMove) == NO_MOVE_ERR;
}
