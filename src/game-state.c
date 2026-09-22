#include "../include/move-rules.h"
#include "../include/assert-toggle.h"
#include "../include/piece-traits.h"
#include "../include/chess-move-arr.h"

#include <stdbool.h>
#include <string.h>

bool isCurrInCheck(const ChessGame* game)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    const IntVec2D kingPosition = whereKingIs(game->board, game->player);
    return oppPieceMayMoveTo(game, &kingPosition, true);
}

int getUnstuckPieceCount(const ChessGame* game, const Piece* p)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    int count = 0;

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAt(game->board, x, y), p) &&
                isPieceStuckAtVec(game, &(IntVec2D){x,y}, false))
                count++;

    return count;
}

bool canBlockAttack(const ChessGame* game, const ChessMove* attack)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    const Piece possibleMover = attack->pieceMove.mover;
    ASSERT(!isBlankSpace(&possibleMover), "This move is not a valid move!");
    ASSERT(isMultStep(&possibleMover), "Piece mover is not a multi-step piece!");

    const IntVec2D direcVec = getDirecVec(&attack->boardMove.move,
                                          &attack->pieceMove.mover);

    for (int i = 1; i < 8; i++)
    {
        const IntVec2D loopVec = multNumByVec(i,&direcVec);
        const IntVec2D positionAtLoopVec = addVecs(&attack->boardMove.position,&loopVec);

        if (!isVecInBoardBounds(&positionAtLoopVec)) continue;

        if (currPieceMayMoveTo(game, &positionAtLoopVec, true)) return true;
    }

    return false;
}

bool canBlockCheck(const ChessGame* game)
{
    const ChessMove move = getAttackToKing(game);

    ASSERT(!isBlankSpace(&move.pieceMove.mover), "There is no attack move to the king so idk if it can be blocked");

    if (!isMultStep(&move.pieceMove.mover)) return false;

    return canBlockAttack(game, &move);
}

bool canCaptureKingAttacker(const ChessGame* game, const ChessMove* move)
{
    ASSERT(equalPiece(&move->pieceMove.captured, &(Piece){game->player, KING}), "Attack is not to king");
    ASSERT(!isBlankSpace(&move->pieceMove.mover), "There is no mover in attack");
    ASSERT(!isBlankSpace(&move->pieceMove.captured), "Move is not an attack");

    const IntVec2D attackerPosition = move->boardMove.position;

    if (currPieceMayMoveTo(game, &attackerPosition, true)) return true;

    return false;
}

// or isLossForCurrPlayer
// or isCheckmateForCurrPlayer
// or isCheckmate
bool isWinForOppPlayer(const ChessGame* game)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    const IntVec2D kingPosition = whereKingIs(game->board,game->player);

    if (!isCurrInCheck(game)) return false;

    if (!isPieceStuckAtVec(game,&kingPosition, false)) return false;

    if (canBlockCheck(game)) return false;

    const ChessMove attack = getAttackToKing(game);
    if (canCaptureKingAttacker(game, &attack)) return false;

    return true;
}

bool doesMoveCauseCheck(const ChessGame* game, const BoardMove* boardMove)
{
    return isCurrInCheck(movedKChessGamePtr(game,boardMove));
}

bool areAllPiecesAreStuck(const ChessGame* game)
{
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (!isBlankSpace(getKPiecePtrAt(game->board, x, y))     &&
                getPieceAt(game->board, x, y).colour == game->player &&
                !isPieceStuckAtVec(game, &(IntVec2D){x,y}, false))
                return false;

    return true;
}

int getPieceCount(const Board board, const Piece* p)
{
    ASSERT(isBlankSpace(p), "Cannot count blank pieces!");

    int count = 0;

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAt(board, x, y), p))
                count++;

    return count;
}

typedef struct
{
    const int pawnCount;
    const int knightCount;
    const int bishopCount;
    const int rookCount;
    const int queenCount;
    const int kingCount;
} PieceCount;

bool equalPieceCount(const PieceCount* pieceCount1, const PieceCount* pieceCount2)
{
    return memcmp(pieceCount1, pieceCount2, sizeof(PieceCount)) == 0;
}

PieceCount getAllOfColourPieceCount(const Board board, const Colour c)
{
    const PieceCount pieceCount = {
        .pawnCount   = getPieceCount(board, &(Piece){c, PAWN}),
        .knightCount = getPieceCount(board, &(Piece){c, KNIGHT}),
        .bishopCount = getPieceCount(board, &(Piece){c, BISHOP}),
        .rookCount   = getPieceCount(board, &(Piece){c, ROOK}),
        .queenCount  = getPieceCount(board, &(Piece){c, QUEEN}),
        .kingCount   = getPieceCount(board, &(Piece){c, KING}),
    };

    return pieceCount;
}

bool isInsufMaterial(const ChessGame* game)
{
    const PieceCount currPieceCount = getAllOfColourPieceCount(game->board, game->player);
    const PieceCount oppPieceCount  = getAllOfColourPieceCount(game->board, oppositeColour(game->player));

    const PieceCount OnlyKing = {
        .kingCount = 1,
    };

    const PieceCount KingAndKnight = {
        .kingCount = 1,
        .knightCount = 1,
    };

    const PieceCount KingAndBishop = {
        .kingCount = 1,
        .bishopCount = 1,
    };

    if (equalPieceCount(&currPieceCount, &OnlyKing) && equalPieceCount(&oppPieceCount, &OnlyKing))
        return true;

    if ((equalPieceCount(&currPieceCount, &OnlyKing) && equalPieceCount(&oppPieceCount,  &KingAndKnight)) ||
        (equalPieceCount(&oppPieceCount,  &OnlyKing) && equalPieceCount(&currPieceCount, &KingAndKnight)))
        return true;

    if ((equalPieceCount(&currPieceCount, &OnlyKing) && equalPieceCount(&oppPieceCount,  &KingAndBishop)) ||
        (equalPieceCount(&oppPieceCount,  &OnlyKing) && equalPieceCount(&currPieceCount, &KingAndBishop)))
        return true;

    if (equalPieceCount(&currPieceCount, &KingAndBishop) && equalPieceCount(&oppPieceCount, &KingAndBishop))
        return true;

    return false;
}

bool isGameDraw(const ChessGame* game)
{
    if (isCurrInCheck(game))        return false;
    if (!isInsufMaterial(game))     return false;
    if (!areAllPiecesAreStuck(game)) return false;

    return false;
}
