#include "../include/move-rules.h"
#include "../include/assert-toggle.h"
#include "../include/piece-traits.h"
#include <string.h>

typedef struct
{
    ChessMove* data;
    int length;
    int capacity;
    // Colour player;
} ChessMoveArr;

ChessMoveArr* initMoveArr(void)
{
    static ChessMoveArr moves = {};
    
    moves.data = calloc(1, sizeof(ChessMove));
    ASSERT(moves.data != NULL, "Could not allocate in initMoveArr");
    moves.length = 0;
    moves.capacity = 1;
    
    return &moves;
}

ChessMoveArr* initLegalMovesTo(const ChessGame* game, const IntVec2D* position);
ChessMoveArr* initLegalMovesFrom(const ChessGame* game, const IntVec2D* position);

void freeMoveArr(ChessMoveArr* moves)
{
    free(moves->data);
    moves->length = 0;
    moves->capacity = 0;
}

void pushMove(ChessMoveArr* moves, const ChessMove* move)
{
    if (moves->capacity == 0 && moves->length == 0) {
        moves->capacity++;
        goto end;
    }

    if (moves->length + 1 > moves->capacity)
    {
        moves->data = realloc(moves->data, moves->capacity * 2);
        ASSERT(moves->data != NULL, "Could not re-allocate in pushMove");
        moves->capacity *= 2;
    }
    
end:
    moves->length++;
    memcpy(&moves->data[moves->length], move, sizeof(ChessMove));
}

const ChessMove* getMovePtr(const ChessMoveArr* moves, const int i)
{
    ASSERT_FMT(i >= 0 && i < moves->length, "Acessing index of ChessMoveArr at %d, length is: %d", i, moves->length);
    return &moves->data[i];
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

int getUnstuckPieceCount(const ChessGame* game, const Piece* p)
{
    int count = 0;

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (equalPiece(getKPiecePtrAt(game->board, x, y), p) &&
                isPieceStuckAtVec(game, &(IntVec2D){x,y}))
                count++;

    return count;
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

bool areAllPiecesAreStuck(const ChessGame* game)
{
    for (int y=0; y<8; y++)
        for (int x=0; x<8; x++)
            if (!isBlankSpace(getKPiecePtrAt(game->board, x, y))     &&
                getPieceAt(game->board, x, y).colour == game->player &&
                !isPieceStuckAtVec(game, &(IntVec2D){x,y}))
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

    if ((equalPieceCount(&currPieceCount, &OnlyKing) && equalPieceCount(&oppPieceCount, &KingAndKnight)) ||
        (equalPieceCount(&oppPieceCount, &OnlyKing) && equalPieceCount(&currPieceCount, &KingAndKnight)))
        return true;

    if ((equalPieceCount(&currPieceCount, &OnlyKing) && equalPieceCount(&oppPieceCount, &KingAndBishop)) ||
        (equalPieceCount(&oppPieceCount, &OnlyKing) && equalPieceCount(&currPieceCount, &KingAndBishop)))
        return true;

    if (equalPieceCount(&currPieceCount, &KingAndBishop) && equalPieceCount(&oppPieceCount, &KingAndBishop))
        return true;

    return false;
}

bool isGameDraw(const ChessGame* game)
{
    if (areAllPiecesAreStuck(game)) return true;

    if (isInsufMaterial(game)) return true;

    return false;
}
