#include "../include/move-rules.h"
#include "../include/assert-toggle.h"
#include "../include/piece-traits.h"

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

