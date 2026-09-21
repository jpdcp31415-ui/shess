#include "../include/chess-move-arr.h"
#include "../include/assert-toggle.h"
#include "../include/piece-traits.h"
#include "../include/chess-game.h"

#include <string.h>

ChessMoveArr initMoveArr(void)
{
    ChessMoveArr moves = {};

    moves.data = calloc(1, sizeof(ChessMove));
    ASSERT(moves.data != NULL, "Could not allocate in initMoveArr");
    moves.length = 0;
    moves.capacity = 1;

    return moves;
}

void freeMoveArr(ChessMoveArr* moves)
{
    ASSERT_FMT(moves->data != NULL, "Trying to free a ChessMoveArr twice at address %p", moves->data);

    moves->length = 0;
    moves->capacity = 0;
    free(moves->data);
    moves->data = NULL;
}

void pushMove(ChessMoveArr* moves, const ChessMove* move)
{
    ASSERT_FMT(moves->data != NULL, "Trying to re-use/push to a ChessMoveArr twice at address %p", moves->data);

    if (moves->capacity == 0 && moves->length == 0)
    {
        moves->capacity++;
        goto end;
    }

    if (moves->length + 1 >= moves->capacity)
    {
        moves->data = realloc(moves->data, moves->capacity * 2 * sizeof(ChessMove));
        ASSERT(moves->data != NULL, "Could not re-allocate in pushMove");
        moves->capacity *= 2;
    }

end:
    moves->length++;
    memcpy(&moves->data[moves->length - 1], move, sizeof(ChessMove));
}

const ChessMove* getMovePtr(const ChessMoveArr* moves, const int i)
{
    ASSERT_FMT(i >= 0 && i < moves->length, "Acessing index of ChessMoveArr at %d, length is: %d", i, moves->length);
    return &moves->data[i];
}

ChessMoveArr initAllLegalMovesCurrOpts(const ChessGame* game, const bool ignoreCheck)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    ChessMoveArr moveArr = initMoveArr();

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            const Piece mover = getPieceAt(game->board, x, y);
            if (game->player != mover.colour || isBlankSpace(&mover)) continue;
            const IntVec2D* moves = getMoves(&mover);

            for (int i = 0; i < getNumMoves(&mover); i++)
            {
                const BoardMove possibleBoardMove =
                {
                    .position = (IntVec2D){x,y},
                    .move = moves[i]
                };

                const IntVec2D nextPosition = getNextPos(&possibleBoardMove);
                if (!isVecInBoardBounds(&nextPosition)) continue;

                const ChessMove possibleChessMove = getAsChessMove(game->board, &possibleBoardMove);

                if (isValidMoveOpts(game,&possibleChessMove,ignoreCheck))
                    pushMove(&moveArr, &possibleChessMove);
            }
        }

    return moveArr;
}

ChessMoveArr initLegalMovesTo(const ChessGame* game, const IntVec2D* positionTo, const bool ignoreCheck)
{
    ChessMoveArr allMoves = initAllLegalMovesCurrOpts(game, ignoreCheck);

    ChessMoveArr movesToPos = initMoveArr();
    
    for (int i = 0; i < allMoves.length; i++)
    {
        const IntVec2D nextPosition = getNextPos(&getMovePtr(&allMoves, i)->boardMove);

        if (equalVecs(&nextPosition, positionTo))
            pushMove(&movesToPos, getMovePtr(&allMoves, i));
    }

    freeMoveArr(&allMoves);

    return movesToPos;
}

ChessMoveArr initLegalMovesFrom(const ChessGame* game, const IntVec2D* positionFrom, const bool ignoreCheck)
{
    ChessMoveArr allMoves = initAllLegalMovesCurrOpts(game, ignoreCheck);

    ChessMoveArr movesToPos = initMoveArr();

    for (int i = 0; i < allMoves.length; i++)
    {
        const IntVec2D posFromEach = getMovePtr(&allMoves, i)->boardMove.position;

        if (equalVecs(&posFromEach, positionFrom))
            pushMove(&movesToPos, getMovePtr(&allMoves, i));
    }

    freeMoveArr(&allMoves);

    return movesToPos;
}

ChessMove getAttackToKing(const ChessGame* game)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    ChessGame flippedGame = {.player = NULL_PLAYER};
    setChessGame(flippedKChessGamePtr(game), &flippedGame);
    flippedGame.player = oppositeColour(game->player);

    const IntVec2D kingPosition = whereKingIs(game->board, game->player);
    const IntVec2D flippedKingPos = {kingPosition.x, 7 - kingPosition.y};

    ChessMoveArr moveToKing = initLegalMovesTo(&flippedGame, &flippedKingPos, true);

    ChessMove attackToKing = {.pieceMove = {.mover = {NULL_COLOUR, NULL_TYPE}}};

    ASSERT(moveToKing.length == 0 || moveToKing.length == 1, "More than one piece cannot check a king at the same time");

    if (moveToKing.length == 1) attackToKing = moveToKing.data[0];

    freeMoveArr(&moveToKing);

    return attackToKing;
}

bool isPieceStuckAtVec(const ChessGame* game, const IntVec2D* position, const bool ignoreCheck)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    ChessMoveArr moves = initLegalMovesFrom(game, position, ignoreCheck);
    const int length = moves.length;
    freeMoveArr(&moves);

    return length == 0;
}

bool currPieceMayMoveTo(const ChessGame* game, const IntVec2D* position, const bool ignoreCheck)
{
    ChessMoveArr moves = initLegalMovesTo(game, position, ignoreCheck);
    const int length = moves.length;
    freeMoveArr(&moves);

    return length != 0;
}

// the possibility of an opposite player piece moving to a specific position
bool oppPieceMayMoveTo(const ChessGame* game, const IntVec2D* position, const bool ignoreCheck)
{
    ASSERT(game->player != NULL_PLAYER, "Cannot check for piece moves without current player!");

    ChessGame flippedGame = {.player = NULL_PLAYER};
    setChessGame(flippedKChessGamePtr(game), &flippedGame);
    flippedGame.player = oppositeColour(game->player);
    const IntVec2D flippedPos = {.x = position->x, .y = 7-position->y};

    ChessMoveArr moves = initLegalMovesTo(&flippedGame, &flippedPos, ignoreCheck);
    const int length = moves.length;
    freeMoveArr(&moves);

    return length != 0;
}
